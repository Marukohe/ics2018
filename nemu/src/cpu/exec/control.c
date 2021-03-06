#include "cpu/exec.h"
#include "cpu/cc.h"

make_EHelper(jmp) { 
  // the target address is calculated at the decode stage
  rtl_j(decoding.jmp_eip);

  print_asm("jmp %x", decoding.jmp_eip);
}

make_EHelper(jcc) { 
  // the target address is calculated at the decode stage
  uint32_t cc = decoding.opcode & 0xf;
  rtl_setcc(&t0, cc);
  rtl_li(&t1, 0);
  rtl_jrelop(RELOP_NE, &t0, &t1, decoding.jmp_eip);

  print_asm("j%s %x", get_cc_name(cc), decoding.jmp_eip);
}

make_EHelper(jmp_rm) {
  rtl_jr(&id_dest->val);

  print_asm("jmp *%s", id_dest->str);
} 

make_EHelper(call) {
  // the target address is calculated at the decode stage
  rtl_push(&decoding.seq_eip);
  //rtl_push(&cpu.eip);
  /*cpu.eip=id_dest->simm+decoding.seq_eip;*/
  rtl_j(decoding.jmp_eip);
 /* if(cpu.eip==0x10000a)
	  cpu.eip+=6;
  else
	  cpu.eip+=10;*/
  /*decoding_set_jmp(true);*/

  print_asm("call %x", decoding.jmp_eip);
}

make_EHelper(ret) {
  //rtl_pop(&decoding.seq_eip);
  //rtl_pop(&decoding.seq_eip);
  rtl_pop(&decoding.jmp_eip);
  rtl_j(decoding.jmp_eip);
  //rtl_pop(&cpu.eip);
  print_asm("ret");
}

make_EHelper(call_rm) {
  //TODO();
  rtl_push(&decoding.seq_eip);
  rtl_jr(&id_dest->val);
  print_asm("call *%s", id_dest->str);
}
