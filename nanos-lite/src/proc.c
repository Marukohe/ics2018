#include "proc.h"

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used));
static PCB pcb_boot;
PCB *current;

extern void context_kload(PCB *pcb, void *entry);
extern void context_uload(PCB *pcb, const char *filename);

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
	//Log("%x",(uint32_t)hello_fun);
	//Log("pcb %x",pcb[0].cp->eip);
	
	//context_kload(&pcb[0],(void *)hello_fun);
	//_protect(&pcb[0].as);
	context_uload(&pcb[0],"/bin/pal");

	Log("pcb ptr %x",(uintptr_t)((&pcb[0])->as.ptr));
/*	Log("pcb edi %x",(&pcb[0])->cp->edi);
	Log("pcb esi %x",(&pcb[0])->cp->esi);
	Log("pcb ebp %x",(&pcb[0])->cp->ebp);
	Log("pcb esp %x",(&pcb[0])->cp->esp);
	Log("pcb ebx %x",(&pcb[0])->cp->ebx);
	Log("pcb edx %x",(&pcb[0])->cp->edx);
	Log("pcb ecx %x",(&pcb[0])->cp->ecx);
	Log("pcb eax %x",(&pcb[0])->cp->eax);
	Log("pcb irq %x",(&pcb[0])->cp->irq);
	Log("pcb err %x",(&pcb[0])->cp->err);
	Log("pcb eip %x",(&pcb[0])->cp->eip);
	Log("pcb cs %x",(&pcb[0])->cp->cs);
	Lo g("pcb eflags %x",(&pcb[0])->cp->eflags);*/
	switch_boot_pcb();
}

_Context* schedule(_Context *prev) {
	//Log("prev %x",prev->eip);
	//save the context point
	current->cp = prev;

	//Log("eip %x",current->cp->eip);

	//always select pcb[0] as the new process
	//current = (current == &pcb[0]?&pcb[1]:&pcb[0]);
	current = &pcb[0];
	Log("%x",(uintptr_t)current->as.ptr);

	//then return the new context
	//Log("%x",current->cp->eip);
  return current->cp;
}
