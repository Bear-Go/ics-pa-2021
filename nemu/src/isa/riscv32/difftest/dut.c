#include <isa.h>
#include <cpu/difftest.h>
#include "../local-include/reg.h"

const char *stdregs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

void std_reg_display() {
	for (int i = 0; i < 32; ++ i) {
		printf("%3s: 0x%08x ", stdregs[i], gpr(i));
    if (i % 4 == 3) printf("\n");
	}
}

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  for (int i = 0; i < 32; ++ i) {
    if (!difftest_check_reg(reg_name(i, sizeof(word_t)), pc, ref_r->gpr[i]._32, gpr(i))) {
      std_reg_display();
      return false;
    }
  }
  return true;
}

void isa_difftest_attach() {
}
