#include "proc.h"

#define DEFAULT_ENTRY 0x4000000

size_t get_ramdisk_size();
size_t ramdisk_write(const void *buf,size_t offset,size_t len);
size_t ramdisk_read(void *buf,size_t offset,size_t len);
extern uint8_t ramdisk_start;
static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO();
  void *bufff=NULL;
  size_t len = get_ramdisk_size();
  ramdisk_read(bufff,0,len);
  //ramdisk_write(buf,0x4000000,len);
  memcpy(&ramdisk_start+(0x4000000-(uintptr_t)&ramdisk_start),bufff,len);
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

  pcb->tf = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->tf = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
