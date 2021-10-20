#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(li) f(auipc) f(rem)\
    f(lb) f(lh) f(lw) f(lbu) f(lhu) f(sb) f(sh) f(sw)\
    f(bne) f(beq) f(bge) f(blt)\
    f(addi) f(jal) f(ret) f(add) f(div) f(slt)\
    f(sltu) f(xor) f(or) f(xori) f(sb)\
    f(lui) f(inv) f(nemu_trap)

def_all_EXEC_ID();
