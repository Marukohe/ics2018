#include "proc.h"
#include <unistd.h>
#define DEFAULT_ENTRY 0x4000000

size_t get_ramdisk_size();
size_t ramdisk_write(const void *buf,size_t offset,size_t len);
size_t ramdisk_read(void *buf,size_t offset,size_t len);
extern uint8_t ramdisk_start;
extern int fs_open(const char *pathname,int flags,int mode);
extern ssize_t fs_read(int fd,void *buf,size_t len);
extern int fs_close(int fd);
extern int fs_filesz(int fd);
static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO();
  //void *bufff=NULL;
  //size_t len = get_ramdisk_size();
  //ramdisk_read(bufff,0,len);
  //ramdisk_write(buf,0x4000000,len);
  //memcpy(&ramdisk_start+0x3EFEDA8,bufff,len);
  //below
  //size_t len=get_ramdisk_size();
  //void *bufff=(void *)0x4000000;
  //ramdisk_read(bufff,0,len);
  int fd = fs_open(filename,0,0);
  fs_read(fd,(void *)DEFAULT_ENTRY,fs_filesz(fd));
  fs_close(fd);
  return DEFAULT_ENTRY;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
