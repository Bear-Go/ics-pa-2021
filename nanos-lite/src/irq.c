#include <common.h>
#include "syscall.h"

extern void do_syscall(Context*);
Context* schedule(Context *prev);

static Context* do_event(Event e, Context* c) {

  switch (e.event) {
    case EVENT_YIELD: {
      Log("event: yield"); 
      Context* ret = schedule(c);
      // printf("context mepc %p\n", ret->mepc);
      return ret;
      panic("should not reach here");
    }
    case EVENT_SYSCALL: do_syscall(c); break;
    default: panic("Unhandled event ID = %d", e.event);
  }

  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
