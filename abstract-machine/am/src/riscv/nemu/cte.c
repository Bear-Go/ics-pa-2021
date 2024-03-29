#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

const char *tregs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

Context* __am_irq_handle(Context *c) {

  // printf("%8s: 0x%08x\n", "mcause", c->mcause);
  // printf("%8s: 0x%08x\n", "mepc", c->mepc);
  // printf("%8s: 0x%08x\n", "mstatus", c->mstatus);
  // for (int i = 0; i < 32; ++ i) {
	// 	printf("%3s: 0x%08x ", tregs[i], c->gpr[i]);
  //   if (i % 4 == 3) printf("\n");
	// }
  assert(c != NULL);
  if (user_handler) {
    Event ev = {0};
    switch (c->mcause) {
      case 11:  
        switch (c->GPR1) {
          case -1:
            ev.event = EVENT_YIELD; 
            break;
          default:
            ev.event = EVENT_SYSCALL; 
            break;
        }
        c->mepc += 4;
        break;
      default: ev.event = EVENT_ERROR; break;
    }

    c = user_handler(ev, c);
    assert(c != NULL);
  }
  // printf("context* %p\n", c);
  // printf("%8s: 0x%08x\n", "mcause", c->mcause);
  // printf("%8s: 0x%08x\n", "mepc", c->mepc);
  // printf("%8s: 0x%08x\n", "mstatus", c->mstatus);
  // for (int i = 0; i < 32; ++ i) {
	// 	printf("%3s: 0x%08x ", tregs[i], c->gpr[i]);
  //   if (i % 4 == 3) printf("\n");
	// }
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
  Context* c = (Context*)((uint8_t*)kstack.end - sizeof(Context) - sizeof(uintptr_t));
  c->mepc = (uintptr_t)entry;
  c->GPR1 = 100;
  uintptr_t* argp = (uintptr_t*)((uint8_t*)kstack.start);
  *argp = (uintptr_t)arg;
  printf("arg = %p\n", arg);
  return c;
}

void yield() {
  asm volatile("li a7, -1; ecall");
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
