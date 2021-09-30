#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  /* TODO: Add more members if necessary */
  char *expr;
  word_t cur_val;

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

static  WP *new_wp(){
  if (free_ == NULL) assert(0);
  WP *p = free_;
  free_ = free_ -> next;
  return p;
}

static void free_wp(WP *wp){
  
  free_ = wp;
}

int set_wp(char *e) {
  bool success = true;
  word_t val = expr(e, &success);
  if ( !success ) {
    printf("Error: watchpoint wrong expression\n");
    return -1;
  }

  WP *p = new_wp();
  p -> expr = strdup(e);
  p -> cur_val = val;
  p -> next =head;
  head = p;

  return p->NO;
}

bool delete_wp(int NO){
  WP *p = head, *pre = NULL;
  for (; p != NULL && p -> NO != NO; pre = p, p = p -> next);
  if (p == NULL) {
    return false;
  }

  if (p == head) {
    head = p -> next;
  }
  else {
    pre -> next = p -> next;
  }
  free_wp(p);
  return true;
}

void list_wp() {
  if (head == NULL) {
    printf("No watchpoints\n");
    return;
  }
  else {
    printf("%4s\t%16s\t%16s\n", "No", "Expression", "Current_val");
    WP *p = head;
    for (; p != NULL; p = p -> next) {
      printf("%4d\t%16s\t%16x\n", p ->  NO, p -> expr, p -> cur_val);
    }
  }
}

void scan_wp(vaddr_t pc) {
  WP *p = head;
  for (; p != NULL; p = p -> next) {
    bool success = true;
    word_t new_val = expr(p -> expr, &success);
    if (p -> cur_val != new_val) {
      printf("Watchpoint: No = %d\nExpression = %s\nChange: %x -> %x\n",
            p -> NO, p -> expr, p -> cur_val, new_val);
      p -> cur_val = new_val;
      nemu_state.state = NEMU_STOP;
      return;
    }
  }
}

