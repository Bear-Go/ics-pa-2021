#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
  printf("Running ## __am_irq_handle() ##\n...\n");
  for (int i = 0; i < 32; ++i) {
    printf("gpr[%2d] 0x%08x ", i, c->gpr[i]);
    if (i % 4 == 3) printf("\n");
  }
  printf("mcause  0x%08x\n", c->MCAUSE);
  printf("mstatus 0x%08x\n", c->MSTATUS);
  printf("mepc    0x%08x\n", c->MEPC);
  if (user_handler) {
    Event ev = {0};
    switch (c->MCAUSE) {
      case 1: ev.event = EVENT_YIELD; break;
      default: ev.event = EVENT_ERROR; break;
    }

    c = user_handler(ev, c);
    assert(c != NULL);
  }

  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  return NULL;
}

void yield() {
  asm volatile("li a7, -1; ecall");
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
