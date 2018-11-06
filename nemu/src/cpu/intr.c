#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */

 // TODO();
    union{
		GateDesc Gatedesc;
		uint32_t val[2];
	}GD;

    /*push eflags*/
	rtl_push(&cpu.eflags);

	/*push CS*/
	rtl_push(&cpu.CS);

	/*push eip*/
	rtl_push(&ret_addr);

	GD.val[0]=vaddr_read(cpu.IDTR.base+NO*8,4);
	GD.val[1]=vaddr_read(cpu.IDTR.base+NO*8+4,4);
	TODO();
	decoding.seq_eip=GD.Gatedesc.offset_15_0+(GD.Gatedesc.offset_31_16<<16);


}

void dev_raise_intr() {
}
