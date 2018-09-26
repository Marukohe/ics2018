#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ,REG,HEX,DEC=260

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
  {"\\*", '*'},         //mutiply
  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"==", TK_EQ},        // equal
  {"/", '/'},         //divide
  {"\\-", '-'},         //minus
  {"\\$.{2,3}", REG},        //register
  {"0x[0-9a-fA-F]",HEX}, //hexidecimal
  {"[0-9]+U*", DEC},       //decimal
  {"\\(",'('},           //left bracket
  {"\\)",')'},           //right bracket
  
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
     }
  } 
}

typedef struct token {
  int type;
  char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
          */

        switch (rules[i].token_type) {
		    case TK_NOTYPE:
				break;
			case '*': case '+': case TK_EQ: case '/': case '-': case REG: case HEX: case DEC: case '(': case ')':
				tokens[nr_token].type=rules[i].token_type;break;
            default: TODO();
         }
        
		if(tokens[nr_token].type==REG||tokens[nr_token].type==HEX||tokens[nr_token].type==DEC)
		{
			strncpy(tokens[nr_token].str,substr_start,substr_len);
			(tokens[nr_token].str)[substr_len]='\0';
	 	}

	    if(rules[i].token_type!=TK_NOTYPE){	
			nr_token++;
	 	}

        break;
       }
    } 

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
     }
  } 

  return true;
}

bool check_parentheses(int p,int q)
{
	if((tokens[p].type!='(')||(tokens[q].type!=')'))
		return false;
	int cnt=0;
	for(int i=p;i<=q;i++)
	{ 
		if(tokens[i].type=='(')
			cnt++;
		if(tokens[i].type==')')
			cnt--;
		if(cnt<=0 && i!=q)
			return false;
	}
	if(cnt==0)
		return true;
	else
	   	return false;
}

uint32_t eval(int p,int q)
{
	if(p>q){
		printf("Unexpected expression\n");
		assert(0);
		return 0;
	} 
	else if(p==q)
	{ 
		int cnt;
	    if(tokens[p].type==HEX)
	 	{
			sscanf(tokens[p].str,"%x",&cnt);
			return cnt;
		}	
		else if(tokens[p].type==DEC)
		{
			sscanf(tokens[p].str,"%d",&cnt);
			return cnt;
	 	}
		else if(tokens[p].type==REG)
		{
			if(tokens[p].str[0]!='$'||tokens[p].str[1]!='e')
			{
				assert(0);
				return 0;
	 		} 
			else
			{
				switch(tokens[p].str[2])
	 			{
					case 'a':
						return cpu.eax;
					case 'b':
						{
							switch(tokens[p].str[3])
							{
								case 'x':
									return cpu.ebx;
								case 'p':
									return cpu.ebp;
	 						}
	 					}
					case 'c':
						return cpu.ecx;
					case 'd':
	 					{
							switch(tokens[p].str[3])
							{
								case 'x':
									return cpu.edx;
								case 'i':
									return cpu.edi;
	 						}
						}
					case 's':
						{
							switch(tokens[p].str[3])
							{
								case 'p':
									return cpu.esp;
								case 'i':
									return cpu.esi;
	 						}
	 					}
					default:{
								assert(0);
								return 0;
	 						}
				}
	 		}
	 	}

	}
	else if(check_parentheses(p,q)==true)
		return eval(p+1,q-1);
	else
	{
		int op=0;
		int op_type=0;
		int flag=1;
		int cnt=0;
		for(int i=p;i<=q;i++)
		{
			if(tokens[i].type=='(')
			{
				cnt++;
			    do{
					i++;
					if(tokens[i].type=='(')
						cnt++;
					else if(tokens[i].type==')')
						cnt--;
	 			}while(cnt!=0);
	 		}
			else if(tokens[i].type=='*'&&flag==1)
	 		{
				op=i;
				op_type='*';
			}
			else if(tokens[i].type=='/'&&flag==1)
			{
				op=i;
				op_type='/';
	 		}
			else if(tokens[i].type=='+')
			{
				op=i;
				op_type='+';
				flag=0;
	 		}
			else if(tokens[i].type=='-')
			{
				op=i;
				op_type='-';
				flag=0;
	 		}
	 	}
		uint32_t val1,val2;
		val1=eval(p,op-1);
		val2=eval(op+1,q);
		
		switch(op_type){
			case '+': return val1+val2;
			case '-': return val1-val2;
			case '*': return val1*val2;
			case '/':
					  if(val2==0)
					  {
						  printf("divide by 0\n");
						  return 0;
	 				  }
					  else
						 return val1/val2;
			default: assert(0);
	 	}
	} 
	assert(0);
	return 0;
}

uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  } 

  /* TODO: Insert codes to evaluate the expression. */
  /*TODO();*/

    return eval(0,nr_token-1);
	return 0;
}
