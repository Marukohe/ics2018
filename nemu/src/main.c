#include "monitor/expr.h"
int init_monitor(int, char *[]);
void ui_mainloop(int);

int main(int argc, char *argv[]) {
  uint32_t expr(char *e, bool *success);
  FILE *fp;
  fp=fopen("./tools/gen-expr/input","w+");
  bool success =true;
  uint32_t cnt;
  while(~fscanf(fp,"%u",&cnt)){
	 char str1[65536];
	 fscanf(fp,"%[^n]%*c",str1);
     uint32_t tmp =  expr(str1, &success);
	 if(cnt==tmp)
		 printf("true\n");
	 else
		 printf("ying ying ying\n");
  }
  fclose(fp);
  /* Initialize the monitor. */
  	
  int is_batch_mode = init_monitor(argc, argv);

  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}
