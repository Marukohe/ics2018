#include <x86.h>
#include <klib.h>

#define PG_ALIGN __attribute((aligned(PGSIZE)))

static PDE kpdirs[NR_PDE] PG_ALIGN;
static PTE kptabs[PMEM_SIZE / PGSIZE] PG_ALIGN;
static void* (*pgalloc_usr)(size_t);
static void (*pgfree_usr)(void*);

_Area segments[] = {      // Kernel memory mappings
  {.start = (void*)0,          .end = (void*)PMEM_SIZE}
};

#define NR_KSEG_MAP (sizeof(segments) / sizeof(segments[0]))

int _vme_init(void* (*pgalloc_f)(size_t), void (*pgfree_f)(void*)) {
  pgalloc_usr = pgalloc_f;
  pgfree_usr = pgfree_f;

  int i;

  // make all PDEs invalid
  for (i = 0; i < NR_PDE; i ++) {
    kpdirs[i] = 0;
  }

  PTE *ptab = kptabs;
  for (i = 0; i < NR_KSEG_MAP; i ++) {
    uint32_t pdir_idx = (uintptr_t)segments[i].start / (PGSIZE * NR_PTE);
    uint32_t pdir_idx_end = (uintptr_t)segments[i].end / (PGSIZE * NR_PTE);
    for (; pdir_idx < pdir_idx_end; pdir_idx ++) {
      // fill PDE
      kpdirs[pdir_idx] = (uintptr_t)ptab | PTE_P;

      // fill PTE
      PTE pte = PGADDR(pdir_idx, 0, 0) | PTE_P;
      PTE pte_end = PGADDR(pdir_idx + 1, 0, 0) | PTE_P;
      for (; pte < pte_end; pte += PGSIZE) {
        *ptab = pte;
        ptab ++;
      }
    }
  }

  set_cr3(kpdirs);
  set_cr0(get_cr0() | CR0_PG);

  return 0;
}

int _protect(_Protect *p) {
  PDE *updir = (PDE*)(pgalloc_usr(1));
  p->pgsize = 4096;
  p->ptr = updir;
  // map kernel space
  for (int i = 0; i < NR_PDE; i ++) {
    updir[i] = kpdirs[i];
  }

  p->area.start = (void*)0x8000000;
  p->area.end = (void*)0xc0000000;
  return 0;
}

void _unprotect(_Protect *p) {
}

static _Protect *cur_as = NULL;
void get_cur_as(_Context *c) {
  c->prot = cur_as;
}

void _switch(_Context *c) {
	printf("miaomiao");
  set_cr3(c->prot->ptr);
  printf("yingyingying");
  cur_as = c->prot;
}
int _map(_Protect *p, void *va, void *pa, int mode) {

	printf("in map\n");
	if(!mode&PTE_P)
		printf("in map assert 0\n\n\n\n");

	PDE *pagde = (PDE *)((uint32_t*)(p->ptr))[PDX(va)];
	PTE *pte;
	if(!(uint32_t)pagde&PTE_P){   //new page
		//assert(0);
		pte = (PTE*)(pgalloc_usr(1));
		//assert(0);
		for(int i=0;i<NR_PTE;i++)
			pte[i]=0;
		//*pagde = PTE_ADDR(pte) | PTE_P;
		//*pagde = ((uint32_t)(pte)&~0xfff) | PTE_P;
		((uint32_t *)(p->ptr))[PDX(va)] = PTE_ADDR(pte) | PTE_P;
		//assert(0);
	}
	else{
		pte = (PTE*)PTE_ADDR(*pagde);
	}
	//assert(0);

	pte[PTX(va)] = PTE_ADDR(pa) | PTE_P;
    //assert(0);
  return 0;
}


_Context *_ucontext(_Protect *p, _Area ustack, _Area kstack, void *entry, void *args) {
	//stack frame of start()
	uint32_t *rstack = (uint32_t *)ustack.end;
	*(--rstack) = 0x0;
	*(--rstack) = 0x0;
	//context
	*(--rstack) = 0x0;   //eflags
	*(--rstack) = 0x8;   //cs
	*(--rstack) = (uint32_t)entry;  //eip
	*(--rstack) = 0x0;    //err
	*(--rstack) = 0x81;   //irq
	*(--rstack) = 0x0;    //eax
	*(--rstack) = 0x12;   //ecx use for test
	*(--rstack) = 0x0;    //edx
	*(--rstack) = 0x0;    //ebx
	*(--rstack) = 0x0;    //esp
	*(--rstack) = (uint32_t)ustack.end; //ebp
	*(--rstack) = 0x0;    //esi
	*(--rstack) = 0x0;    //edi
	//p->ptr =(void *)((uint32_t)p->ptr|0x1);
	*(--rstack) = (uint32_t)p;
  return (_Context *)rstack;
}
