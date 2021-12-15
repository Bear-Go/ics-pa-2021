#include <common.h>
#include "syscall.h"

extern void do_syscall(Context*);

const char *tregs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

static Context* do_event(Event e, Context* c) {

  printf("%8s: 0x%08x\n", "mcause", c->mcause);
  printf("%8s: 0x%08x\n", "mepc", c->mepc);
  printf("%8s: 0x%08x\n", "mstatus", c->mstatus);
	for (int i = 0; i < 32; ++ i) {
		printf("%3s: 0x%08x ", tregs[i], c->gpr[i]);
    if (i % 4 == 3) printf("\n");
	}

  switch (e.event) {
    case EVENT_YIELD: Log("event: yield"); break;
    case EVENT_SYSCALL: Log("event: syscall"); do_syscall(c); break;
    default: panic("Unhandled event ID = %d", e.event);
  }

  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
