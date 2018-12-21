#include "cpu/exec.h"
#include "cpu/cc.h"

make_EHelper(test) {
 // TODO();
  //Log("%x %x", id_dest->val,id_src->val,id->width);
  rtl_and(&at,&id_dest->val,&id_src->val);
  //operand_write(id_dest,&at);
  //Log("%x",at);
  cpu.CF=0;
  cpu.OF=0;
  rtl_update_ZFSF(&at,id_dest->width);

  print_asm_template2(test);
}

make_EHelper(and) {
  /*TODO();*/
  //printf("%x\n",id_src->val);
  //rtl_sext(&t1,&id_src->val,id_src->width);
  //printf("at: %x\n",at);
  rtl_and(&at,&id_dest->val,&id_src->val);
  //printf("t0: %x\n",t0);
  operand_write(id_dest,&at);
  cpu.CF=0;
  cpu.OF=0;
  rtl_update_ZFSF(&at,id_dest->width);

  print_asm_template2(and);
}

make_EHelper(xor) {
  rtl_xor(&at,&id_dest->val,&id_src->val);
  operand_write(id_dest,&at);
  cpu.CF=0;
  cpu.OF=0;
  rtl_update_ZFSF(&at,id_dest->width);

  print_asm_template2(xor);
}

make_EHelper(or) {
  rtl_or(&at,&id_dest->val,&id_src->val); 
  operand_write(id_dest,&at);
  cpu.CF=0;
  cpu.OF=0;
  rtl_update_ZFSF(&at,id_dest->width);

  print_asm_template2(or);
}

make_EHelper(sar) {
  //TODO();
  // unnecessary to update CF and OF in NEMU
  rtl_sar(&at,&id_dest->val,&id_src->val);
  operand_write(id_dest,&at);
  rtl_update_ZFSF(&at,id_dest->width);
  print_asm_template2(sar);
}

make_EHelper(shl) {
  //TODO();
  // unnecessary to update CF and OF in NEMU
  rtl_shl(&at,&id_dest->val,&id_src->val);
  operand_write(id_dest,&at);
  rtl_update_ZFSF(&at,id_dest->width);

  print_asm_template2(shl);
}

make_EHelper(shr) {
  //TODO();
  // unnecessary to update CF and OF in NEMU
  rtl_shr(&at,&id_dest->val,&id_src->val);
  operand_write(id_dest,&at);
  rtl_update_ZFSF(&at,id_dest->width);

  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint32_t cc = decoding.opcode & 0xf;

  rtl_setcc(&t2, cc);
  operand_write(id_dest, &t2);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

make_EHelper(not) {
  //TODO
  rtl_not(&at,&id_dest->val);
  operand_write(id_dest,&at);
  print_asm_template1(not);
}

make_EHelper(rol){
	//DOmyself
	int tmp=id_src->val;
	t0=id_dest->val;
	while(tmp!=0){
		int tmpcf=((t0>>(id_dest->width*8-1))==1);
		t0=t0*2+tmpcf;
		tmp--;
	}
	operand_write(id_dest,&t0);
	int cf=((t0>>(id_dest->width*8-1))==1);
	if(id_src->val==1){
		if(cf!=cpu.CF)
			cpu.OF=1;
		else
			cpu.OF=0;
	}
}

make_EHelper(ror){
	int tmp=id_src->val;
	t0=id_dest->val;
	while(tmp!=0){
		int tmpcf=t0-(t0/2)*2;
		t0=t0/2+(tmpcf*(2^id_dest->width));
		tmp--;
	}
	operand_write(id_dest,&t0);
	int cf=((t0>>(id_dest->width*8-1))==1);
	int next_cf=(((t0<<1)>>(id_dest->width*8-1))==1);
	if(id_src->val==1){
		if(cf!=next_cf)
			cpu.OF=1;
		else
			cpu.OF=0;
	}
}
