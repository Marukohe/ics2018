#include "proc.h"

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used));
static PCB pcb_boot;
PCB *current;

extern void context_kload(PCB *pcb, void *entry);

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite for the %dth time!", j);
    j ++;
    _yield();
  }
}

void naive_uload(PCB *pcb,const char *filename);
void init_proc() {
	//naive_uload(NULL,"/bin/init");
	Log("%x",(uint32_t)hello_fun);
	//Log("pcb %x",pcb[0].cp->eip);
	context_kload(&pcb[0],(void *)hello_fun);
	Log("pcb %x",(&pcb[0])->cp->eip);
	switch_boot_pcb();
}

_Context* schedule(_Context *prev) {
	Log("prev %x",prev->eip);
	//save the context point
	current->cp = prev;
	Log("eip %x",current->cp->eip);
	//always select pcb[0] as the new process
	current = &pcb[0];
	//then return the new context
	Log("%x",current->cp->eip);
  return current->cp;
}
