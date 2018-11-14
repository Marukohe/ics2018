#include "common.h"
#include "syscall.h"

void sys_yield();
_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  printf("yingyingying %d\n",a[0]);
  switch (a[0]) {
	case 1: sys_yield();
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}

void sys_yield(){
	_yield();
}
