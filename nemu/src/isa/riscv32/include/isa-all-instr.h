#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(li) f(auipc)\
    f(lb) f(lh) f(lw) f(lbu) f(lhu) f(sb) f(sh) f(sw)\
    f(beq) f(bne) f(blt) f(bge) f(bltu) f(bgeu) f(jal) f(jalr)\
    f(mul) f(mulh) f(mulhu) f(div) f(divu) f(rem) f(remu)\
    f(addi)\
    f(add) f(sub) f(sll) f(slt) f(sltu) f(xor) f(srl) f(sra) f(or) f(and)\
    f(xori)\
    f(lui) f(inv) f(nemu_trap)

def_all_EXEC_ID();
