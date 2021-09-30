#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <memory/vaddr.h>
#include "sdb.h"

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
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
	//wait to be solved
  cpu_exec(-1);
	return -1;
}

static int cmd_help(char *args);


static int cmd_si(char *args) {
	/* extract the times */
	char *arg = strtok(NULL, " ");
	int i;

	if (arg == NULL) {
		/* default 1 time */
		cpu_exec(1);
		return 0;
	}
	
	/* get the times */
	sscanf(arg, "%d", &i);

	if (i >= 0) {
	/* normal i times*/
		for(; i > 0; -- i) {
			cpu_exec(1);
		 }
		return 0;
	} 
	if (i == -1) {
	/* infinite */
		cpu_exec(-1);
		return 0;
	}
	if (i < -1) {
	/* invalid times */
		printf("Error: Invalid times\n");
		return 0;
	}
	printf("Error: Amazing\n");
	return 0;
}

static int cmd_info(char *args) {
	char *arg = strtok(NULL, " ");
  if (arg == NULL) {
    printf("Incomplete command\n");
    return 0;
  }
	if (strcmp(arg,  "r") == 0) {
	/* print register info*/
		isa_reg_display();
		return 0;
	}
	if (strcmp(arg,  "w") == 0) {
		list_wp();
		return 0;
	}
	printf("Error: Invalid info choice\n");
	return 0;
}

static int cmd_x(char *args) {
	char *arg1 = strtok(NULL, " ");
  if (arg1 == NULL) {
    printf("Incomplete command\n");
    return 0;
  }
	int len;
  sscanf(arg1, "%d", &len);
	char *arg2 = strtok(NULL, "\n");
  if (arg2 == NULL) {
    printf("Incomplete command\n");
    return 0;
  }
	vaddr_t addr;
  bool is_expr_right = true;
  addr = expr(arg2, &is_expr_right);
  if ( !is_expr_right ) {
      printf("Attention: wrong expression\n");
      return 0;
  }
	for (; len > 0; -- len) {
		printf("0x%x:\t0x%08x\n", addr, vaddr_read(addr, 4) & 0xffffffff);
		addr+=4;
	}
	return 0;
}

static int cmd_p(char *args) {
	if (args == NULL) {
    printf("Please input the expression\n");
    return 0;
  }
  vaddr_t val;
  bool is_expr_right = true;
  val = expr(args, &is_expr_right);
  if ( !is_expr_right ) {
      printf("Attention: wrong expression\n");
      return 0;
  }
  printf("0x%x\n",val);
	return 0;
}

static int cmd_w(char *args) {
	if (args == NULL) {
    printf("Please input the exxpression\n");
  }
  else {
    int No = set_wp(args);
    if (No == -1) {
      printf("Wrong expression:cmd_w\n");
    }
    else {
      printf("Watchpoint set successfully\tNo = %d\n",No);
    }
  }
	return 0;
}

static int cmd_d(char *args) {
	char *arg = strtok(NULL, " ");
  if (arg == NULL){
    printf("Please input the No");
  }
  else {
    int No;
    sscanf(args, "%d", &No);
    if ( !delete_wp(No) ) {
      printf("Error: No such watchpoint\n");
    }
  }
	return 0;
}

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
	{ "si", "Step N times", cmd_si},
	{ "info", "Print info which you want", cmd_info},
	{ "x", "Print N info start from register EXPR", cmd_x},
	{ "p", "Determine the value of expression EXPR", cmd_p},
	{ "w", "Set the watchpoint", cmd_w},
	{ "d", "Delete the watchpoint", cmd_d},

	/* TODO: Add more commands */

};

#define NR_CMD ARRLEN(cmd_table)

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

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
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

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
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

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
