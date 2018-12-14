#include "nemu.h"

#define PMEM_SIZE (128 * 1024 * 1024)

#define pmem_rw(addr, type) *(type *)({\
    Assert(addr < PMEM_SIZE, "physical address(0x%08x) is out of bound", addr); \
    guest_to_host(addr); \
    })

uint8_t pmem[PMEM_SIZE];

/* Memory accessing interfaces */
int is_mmio(paddr_t addr);
uint32_t mmio_read(paddr_t addr, int len, int map_NO);
void mmio_write(paddr_t addr, int len, uint32_t data, int map_NO);

uint32_t paddr_read(paddr_t addr, int len) {
  if(is_mmio(addr)==-1)
	return pmem_rw(addr, uint32_t) & (~0u >> ((4 - len) << 3));
  else
	return mmio_read(addr,len,is_mmio(addr));
}

void paddr_write(paddr_t addr, uint32_t data, int len) {
  if(is_mmio(addr)==-1)
	memcpy(guest_to_host(addr), &data, len);
  else
	mmio_write(addr,len,data,is_mmio(addr));
}


paddr_t page_translate(vaddr_t addr){
	//Log("addr: %x",addr);
	PDE *pagde,pde;
	PTE *pagte,pte;
	paddr_t paddr = addr;
	if(cpu.cr0.paging){ 
		//assert(0);
		pagde = (PDE*)(uintptr_t)(cpu.cr3.page_directory_base<<12);
		pde.val = paddr_read((uintptr_t)&pagde[addr>>22],4);
		assert(pde.present==0);
		pagte = (PTE*)(uintptr_t)(pde.page_frame<<12);
		pte.val = paddr_read((uintptr_t)&pagte[(addr<<10)>>22],4);
		assert(pte.present);
		paddr = (pte.page_frame<<12)+(addr&0xfff);
	}
	return paddr;
}

uint32_t vaddr_read(vaddr_t addr, int len) {
	if(((addr+len-1)&(~PAGE_MASK))!=(addr&(~PAGE_MASK))){
		/*This is a special case, you can handle it later*/
		//assert(0);
		union{
			uint8_t databytes[4];
			uint32_t datawords;
		}data = {0};
		for(int i=0;i<len;i++){
			paddr_t paddr = page_translate(addr+i);
			data.databytes[i] = (uint8_t)paddr_read(paddr,1);
		}
		return data.datawords;
	}
	paddr_t paddr = page_translate(addr);
  return paddr_read(paddr, len);
}

void vaddr_write(vaddr_t addr, uint32_t data, int len) {
	if(((addr+len-1)&(~PAGE_MASK))!=(addr&(~PAGE_MASK))){
		/*This is a special case, you can handle it later*/
		//assert(0);
		for(int i=0;i<len;i++){
			paddr_write(addr+i,data,1);
			data>>=8;
		}
	}else{
		paddr_t paddr = page_translate(addr);
		paddr_write(paddr, data, len);
	}
}
