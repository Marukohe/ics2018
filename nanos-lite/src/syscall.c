#include "common.h"
#include "syscall.h"
#include <unistd.h>
#include "proc.h"

void _halt(int code);
int sys_yield();
int sys_write(_Context *c);
int sys_brk(_Context *c);
extern int fs_open(const char *pathname, int flags,int mode);
extern ssize_t fs_read(int fd,void *buf,size_t len);
extern ssize_t fs_write(int fd,const void *buf,size_t len);
extern off_t fs_lseek(int fd,off_t offset,int whence);
extern int fs_close(int fd);
_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  //printf("at_syscall.c ebx %d\n",c->GPR2);
  switch (a[0] ) {
	case SYS_exit: _halt(a[1]); break;
	case SYS_yield: c->GPRx=sys_yield(); break;
	case SYS_open: c->GPRx=fs_open((const char *)a[1],a[2],a[3]); break;
	case SYS_read: c->GPRx=fs_read(a[1],(void *)a[2],a[3]);break;
	case SYS_write: c->GPRx=fs_write(a[1],(void *)a[2],a[3]); break;
	case SYS_close: c->GPRx=fs_close(a[1]);break;
	case SYS_lseek: c->GPRx=fs_lseek(a[1],a[2],a[3]);break;
    case SYS_brk: c->GPRx=sys_brk(c); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  } 

  return NULL;
}

int sys_yield(){
	_yield();
	return 0;
}

void naive_uload(PCB *pcb,const char *filename);
int	sys_execve(const char *filename,char *const argv[],char *const envp[]){
	naive_uload(NULL,filename);
	return -1;
}
/*
int sys_exit(int code){
	_halt(code);
	return 0;
}
*/
/*
int sys_write(_Context *c){
	assert(c->GPR2<=2);
	char *buf=(char *)c->GPR3;
	for(int i=0;i<c->GPR4;i++)
		_putc(buf[i]);
	return SYS_write; 
}
*/
int sys_brk(_Context *c){
	return 0;
}
