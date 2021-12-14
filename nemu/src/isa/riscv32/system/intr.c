#include <isa.h>

word_t isa_raise_intr(word_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * Then return the address of the interrupt/exception vector.
   */
  cpu.mcause = NO;
  cpu.mstatus = 0x00001800;

  switch (NO) {
    case 11: cpu.mepc = epc + 4; break;
    default: panic("unimplemented mcause %d", NO); break;
  }

  return cpu.mtvec;
}

word_t isa_query_intr() {
  return INTR_EMPTY;
}
