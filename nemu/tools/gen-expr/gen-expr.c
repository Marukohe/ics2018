#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536];

static uint32_t choose(uint32_t n)
{
	return rand()%n;
}

static inline char *gen_num()
{
	static char s[20];
	int tmp=rand()%1000;
	sprintf(s,"%d",tmp);
	return s;
}

static inline char *gen(char a)
{
	static char b[3];
	b[0]=a;
	return b;
}

static inline char *gen_rand_op()
{
	int temp=rand()%4;
	switch(temp){
		case 0: return "*";
		case 1: return "/";
		case 2: return "+";
		case 3: return "-";
		default: assert(0);
	}
}

static inline void insertspace()
{
	while(rand()%4==1)
	{
		strcat(buf," ");
	}
}

static inline void gen_rand_expr() {
   /* buf[0] = '\0';*/
   switch(choose(3)){
	  case 0: insertspace(); strcat(buf,gen_num());strcat(buf,"U");break;
	  case 1: strcat(buf,gen('('));
			  insertspace();
			  gen_rand_expr();
			  insertspace();
			  strcat(buf,gen(')'));
			  insertspace();
			  break;
	  default: insertspace();
			   gen_rand_expr();
			   insertspace();
			   strcat(buf,gen_rand_op());
			   insertspace();
			   gen_rand_expr();
			   insertspace();
			   break;
  }
  if(strlen(buf)>=65536)
  {	 
	  buf[0]='0';
	  buf[1]='\0';
  }
}

static char code_buf[65536];
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
	buf[0]='\0';
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen(".code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc .code.c -o .expr");
    if (ret != 0) continue;

    fp = popen("./.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp,"%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
