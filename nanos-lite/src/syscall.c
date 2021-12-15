#include <common.h>
#include "syscall.h"

const char *tregs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

void do_syscall(Context *c) {

  printf("%8s: 0x%08x\n", "mcause", c->mcause);
  printf("%8s: 0x%08x\n", "mepc", c->mepc);
  printf("%8s: 0x%08x\n", "mstatus", c->mstatus);
	for (int i = 0; i < 32; ++ i) {
		printf("%3s: 0x%08x ", tregs[i], c->gpr[i]);
    if (i % 4 == 3) printf("\n");
	}

  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  switch (a[0]) {
    // case SYS_exit:
    //   printf("exit\n");
    //   halt(a[1]);
    //   break;
    case SYS_yield: 
      printf("yield\n");
      asm volatile("ecall");
      c->GPRx = 0;
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
