#include "common.h"
#include "syscall.h"

void _halt(int code);
int sys_yield();
int sys_exit();

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  //printf("yingyingying %d\n",a[0]);
  switch (a[0]) {
	case 0: sys_exit(0); break;
	case 1: sys_yield(); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  } 

  //c->GPRx=c->GPR1;
  return NULL;
  //return c;
}

int sys_yield(){
	_yield();
	return 0;
}

int sys_exit(int code){
	_halt(code);
	return 0;
}
