#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(li) f(auipc) f(rem)\
    f(lb) f(lh) f(lw) f(lbu) f(lhu) f(sb) f(sh) f(sw)\
    f(bne) f(beq) f(bge) f(blt)\
    f(addi) f(jal) f(ret) f(div)\
    f(add) f(sub) f(sll) f(slt) f(sltu) f(xor) f(srl) f(sra) f(or) f(and)\
    f(xori)\
    f(lui) f(inv) f(nemu_trap)

def_all_EXEC_ID();
