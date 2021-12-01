#ifndef ARCH_H__
#define ARCH_H__

struct Context {
  // // TODO: fix the order of these members to match trap.S
  // uintptr_t ra, gp, tp, t0, t1, t2, s0, s1;
  // uintptr_t a0, a1, a2, a3, a4, a5, a6, a7;
  // uintptr_t s2, s3, s4, s5, s6, s7, s8, s9;
  // uintptr_t s10,s11,t3, t4, t5, t6;
  uintptr_t gpr[32], mcause, mstatus, mepc;
  void *pdir;
};

#define GPR1 gpr[17] // a7
// #define GPR1 a7
#define GPR2 gpr[0]
#define GPR3 gpr[0]
#define GPR4 gpr[0]
#define GPRx gpr[0]

#endif
