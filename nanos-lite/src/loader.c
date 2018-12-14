#include "proc.h"
#include <unistd.h>
#define DEFAULT_ENTRY 0x8048000

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
  int fd = fs_open(filename,0,0);
  Log("1");
  void *va = (void *)DEFAULT_ENTRY;
  Log("2");
  int fz = fs_filesz(fd);
  Log("3");
  for(;va <= (void *)DEFAULT_ENTRY+fz;va+=PGSIZE){
	  Log("va: %x",(uint32_t)va);
	void *pa = new_page(1);
	Log("pa: %x",(uint32_t)pa);
	_map(&pcb->as,va,pa,1);
	Log("dd");
	fs_read(fd,pa,PGSIZE);
	Log("cd");
  }
  Log("4");
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
assert(0);
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);
  Log("%x",(uint32_t)stack.start);
  Log("%x",(uint32_t)stack.end);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
