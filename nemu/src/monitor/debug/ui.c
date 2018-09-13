#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args){
	if(args==NULL)
	{
		cpu_exec(1);
		return 0;
	}
	uint64_t cnt=0;
	int len=strlen(args);
	if(!len)
		cnt=1;
	else
	{
		if(len>2){
			printf("Unexpected expression.\n");
			return 0;
		}
		else{
			for(int i=0;i<len;i++)
			{
				if('0'<=args[i] && args[i]<='9'){
					cnt=cnt*10+args[i]-'0';
				}
				else
				{	
					printf("Unexpected expression.\n");
					return 0;
				}
			}
		}
	}
	cpu_exec(cnt);
	return 0;
}

static int cmd_info(char *args)
{
	 if(strcmp(args,"r")==0){
			printf("\t%x",cpu.eax);
			printf("\t%x",cpu.ecx);
			printf("\t%x",cpu.edx);
			printf("\t%x",cpu.ebx);
			printf("\t%x",cpu.esp);
			printf("\t%x",cpu.ebp);
			printf("\t%x",cpu.esi);
			printf("\t%x",cpu.edi);
			return 0;
	 }
	 else{
		/*case "w": watchpoint*/ 
		printf("Unexpected expression\n");
		return 0;
	 }
}

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "Do the next step for N times.Expression should be si N; if not N,just do one step",cmd_si},
  {"info", "print the state of program",cmd_info},
  
  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  while (1) {
    char *str = rl_gets();
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
