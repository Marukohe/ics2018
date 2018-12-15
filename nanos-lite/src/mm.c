#include "memory.h"
#include "proc.h"

static void *pf = NULL;

void* new_page(size_t nr_page) {
  void *p = pf;
  pf += PGSIZE * nr_page;
  assert(pf < (void *)_heap.end);
  return p;
}

void free_page(void *p) {
  panic("not implement yet");
}


/* The brk() system call handler. */
int mm_brk(uintptr_t new_brk) {
	Log("cur_brk: %x\nmax_brk: %x\nnew_brk: %x\n",current->cur_brk,current->max_brk,new_brk);
	if(current->cur_brk == 0){
		current->cur_brk = current->max_brk = new_brk;
	}
	else{
		if(new_brk > current->max_brk){
			uintptr_t va;
			void *pa;
			uintptr_t begin = (current->max_brk&~0xfff)==current->max_brk?current->max_brk:(current->max_brk&~0xfff)+PGSIZE;
			for(va = begin;va<=new_brk;va+=PGSIZE){
				pa = new_page(1);
				Log("pa: %x",(uintptr_t)pa);
				_map(&current->as,(void*)va,pa,1);
			}
			current->max_brk = va;
		}
		current->cur_brk = new_brk;
	}
  return 0;
}

void init_mm() {
  pf = (void *)PGROUNDUP((uintptr_t)_heap.start);
  Log("free physical pages starting from %p", pf);

  _vme_init(new_page, free_page);
}
