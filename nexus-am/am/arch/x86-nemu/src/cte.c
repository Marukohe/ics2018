#include <am.h>
#include <x86.h>
#include <klib.h>
static _Context* (*user_handler)(_Event, _Context*) = NULL;

void vectrap();
void vecnull();
void vecsys();

_Context* irq_handle(_Context *tf) {
  _Context *next = tf;
  if (user_handler) {
    _Event ev={0};
	//
	/*
	printf("eax: %x\n",tf->eax);
	printf("ecx: %x\n",tf->ecx);
	printf("edx: %x\n",tf->edx);
	printf("ebx: %x\n",tf->ebx);
	printf("esp: %x\n",tf->esp);
	printf("ebp: %x\n",tf->ebp);
	printf("esi: %x\n",tf->esi);
	printf("edi: %x\n",tf->edi);
	printf("eflags: %x\n",tf->eflags);
	printf("cs: %x\n",tf->cs);
	printf("eip %x\n",tf->eip);
	*/
	//
    switch (tf->irq) {
	  case 0x81: ev.event=_EVENT_YIELD; break;
	  case 0x80: ev.event=_EVENT_SYSCALL; break;
      default: ev.event = _EVENT_ERROR; break;
    }

    next = user_handler(ev, tf);
    if (next == NULL) {
      next = tf;
    }
  }

  return next;
}

static GateDesc idt[NR_IRQ];

int _cte_init(_Context*(*handler)(_Event, _Context*)) {
  // initialize IDT
  for (unsigned int i = 0; i < NR_IRQ; i ++) {
    idt[i] = GATE(STS_TG32, KSEL(SEG_KCODE), vecnull, DPL_KERN);
  }

  // -------------------- system call --------------------------
  idt[0x81] = GATE(STS_TG32, KSEL(SEG_KCODE), vectrap, DPL_KERN);
  //mytodo
  idt[0x80] = GATE(STS_TG32, KSEL(SEG_KCODE), vecsys, DPL_KERN);
  set_idt(idt, sizeof(idt));

  // register event handler
  user_handler = handler;

  return 0;
}

_Context *_kcontext(_Area stack, void (*entry)(void *), void *arg) {
	printf("%x\n\n",(uint32_t)entry);
	uint32_t *rstack = (uint32_t *)stack.end;
	*(--rstack) = 0x0;              //eflags
	*(--rstack) = 0x8;              //cs
	*(--rstack) = (uint32_t)entry;  //eip
	*(--rstack) = 0x0;              //err
	*(--rstack) = 0x81;  //irq
	*(--rstack) = 0x0;   //eax
	*(--rstack) = 0x0;   //ecx
	*(--rstack) = 0x0;   //edx
	*(--rstack) = 0x0;   //ebx
	*(--rstack) = (uint32_t)stack.end;   //esp
	*(--rstack) = 0x0;   //ebp
	*(--rstack) = 0x0;   //esi
	*(--rstack) = 0x0;   //edi
  return (_Context *)rstack;
}

void _yield() {
  asm volatile("int $0x81");
}

int _intr_read() {
  return 0;
}

void _intr_write(int enable) {
}
