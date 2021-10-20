#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(li) f(auipc) f(rem)\
    f(addi) f(jal) f(ret) f(beq) f(add) f(bge)\
    f(bne) f(sltu) f(xor) f(or) f(xori) f(sb)\
    f(lui) f(lw) f(sw) f(inv) f(nemu_trap)

def_all_EXEC_ID();
