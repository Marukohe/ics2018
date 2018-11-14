#include "common.h"
#include "syscall.h"

void _halt(int code);
int sys_yield();
int sys_exit();
int sys_write(_Context *c);

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  printf("at syscall.c %d\n",c->GPR1);
  //printf("at_syscall.c ebx %d\n",c->GPR2);
  switch (a[0]) {
	case 0: c->GPRx=sys_exit(c->GPR2); break;
	case 1: c->GPRx=sys_yield(); break;
	case 4: c->GPRx=sys_write(c); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  } 

  //c->GPRx=c->GPR1;
  return NULL;
  //return c->GPRx;
}

int sys_yield(){
	_yield();
	return 0;
}

int sys_exit(int code){
	_halt(code);
	return 0;
}

int sys_write(_Context *c){
	assert(c->GPR2<=2);
	char *buf=(char *)c->GPR3;
	for(int i=0;i<c->GPR4;i++)
		_putc(buf[i]);
	return c->GPR1; 
}
