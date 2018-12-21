#include "cpu/exec.h"
//#include "device/port-io.h"

void difftest_skip_ref();
void difftest_skip_dut();

make_EHelper(lidt) {
  TODO();

  print_asm_template1(lidt);
}

make_EHelper(mov_r2cr) {
  TODO();

  print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
}

make_EHelper(mov_cr2r) {
  TODO();

  print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}

make_EHelper(int) {
  TODO();

  print_asm("int %s", id_dest->str);

#if defined(DIFF_TEST) && defined(DIFF_TEST_QEMU)
  difftest_skip_dut();
#endif
}

make_EHelper(iret) {
  TODO();

  print_asm("iret");
}

uint32_t pio_read_l(ioaddr_t addr);
uint32_t pio_read_w(ioaddr_t addr);
uint32_t pio_read_b(ioaddr_t addr);

make_EHelper(in) {
  //TODO();
  //operand_write(id_dest,&id_src->val);
  if(id_dest->width==1){
	  rtl_li(&at,pio_read_b(id_src->val));
	  operand_write(id_dest,&at);
  }
  else if(id_dest->width==2){
	  rtl_li(&at,pio_read_w(id_src->val));
	  operand_write(id_dest,&at);
  } 
   else{
	  rtl_li(&at,pio_read_l(id_src->val));
	  operand_write(id_dest,&at);
  }
  print_asm_template2(in);

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}

void pio_write_b(ioaddr_t, uint32_t data);
void pio_write_w(ioaddr_t, uint32_t data);
void pio_write_l(ioaddr_t, uint32_t data);

make_EHelper(out) {
  //TODO();
  //operand_write(id_dest,&id_src->val);
  if(id_dest->width==1){
	  pio_write_b(id_dest->val,id_src->val);
	  //operand_write(id_dest,&at);
  }
  else if(id_dest->width==2){
	  pio_write_w(id_dest->val,id_src->val);
	  //operand_write(id_dest,&at);
  }
  else{
	  pio_write_l(id_dest->val,id_src->val);
	  //operand_write(id_dest,&at);
  }

  print_asm_template2(out);

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}
