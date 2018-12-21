#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  rtl_push(&id_dest->val);

  print_asm_template1(push);
}

make_EHelper(pop) {
  rtl_pop(&at);
  operand_write(id_dest,&at);
  print_asm_template1(pop);
}

make_EHelper(pusha) {
  //TODO();
  //if(id_dest->width==2){
  if(decoding.is_operand_size_16){
	  Assert(0,"pusha operand_size is 16");
  }
  else{
	  uint32_t tmp=cpu.esp;
	  rtl_push(&cpu.eax);
	  rtl_push(&cpu.ecx);
	  rtl_push(&cpu.edx);
	  rtl_push(&cpu.ebx);
	  rtl_push(&tmp);
	  rtl_push(&cpu.ebp);
	  rtl_push(&cpu.esi);
	  rtl_push(&cpu.edi);
  }
	
  print_asm("pusha");
}

make_EHelper(popa) {
  //TODO();
  if(decoding.is_operand_size_16){
	  Assert(0,"popa operand size is 16");
  }
  else{
	uint32_t throwaway;
	rtl_pop(&cpu.edi);
	rtl_pop(&cpu.esi);
	rtl_pop(&cpu.ebp);
	rtl_pop(&throwaway);
	rtl_pop(&cpu.ebx);
	rtl_pop(&cpu.edx);
	rtl_pop(&cpu.ecx);
	rtl_pop(&cpu.eax);
  }
  print_asm("popa");
}

make_EHelper(leave) {
  //TODO();
  cpu.esp=cpu.ebp;
  rtl_pop(&cpu.ebp);
  print_asm("leave");
}

make_EHelper(cltd) {
  if (decoding.is_operand_size_16) {
    //TODO();
	rtl_lr(&at,0,2);
	//rtl_li(&t3,15);
	//rtl_shr(&at,&at,&t3);
	at=at>>15;
	if(at==1){
		rtl_li(&t1,0xffff);
		rtl_sr(2,&t1,2);
	}
	else{
		rtl_li(&t1,0);
		rtl_sr(2,&t1,2);
	}
  } 
  else {
    //TODO();
	rtl_lr(&at,0,4);
	//rtl_li(&t3,31);
	//rtl_shr(&at,&at,&t3);
	at=at>>31;
	if(at==1){
		rtl_li(&t1,0xffffffff);
		rtl_sr(2,&t1,4);
	}
	else{
		rtl_li(&t1,0);
		rtl_sr(2,&t1,4);
	}
   }
 
  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decoding.is_operand_size_16) {
    //TODO();
	rtl_lr(&at,0,1);
	rtl_sext(&t1,&at,1);
	rtl_sr(0,&t1,2);
	//rtl_li(&t3,15);
	//rtl_shr(&at,&at,&t3);
  } 
  else {
    //TODO();
	rtl_lr(&at,0,2);
	rtl_sext(&t1,&at,2);
	rtl_sr(0,&t1,4);
	//rtl_li(&t3,31);
	//rtl_shr(&at,&at,&t3);
   }
 
  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(movsx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  //printf("%d\n",id_src->width);
  rtl_sext(&t0, &id_src->val, id_src->width);
  operand_write(id_dest, &t0);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  operand_write(id_dest, &id_src->addr);
  print_asm_template2(lea);
}
