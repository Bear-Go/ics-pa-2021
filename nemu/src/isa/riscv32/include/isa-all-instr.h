#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(lui) f(auipc)\
    f(lb) f(lh) f(lw) f(lbu) f(lhu) f(sb) f(sh) f(sw)\
    f(beq) f(bne) f(blt) f(bge) f(bltu) f(bgeu) f(jal) f(jalr)\
    f(mul) f(mulh) f(mulhu) f(div) f(divu) f(rem) f(remu)\
    f(addi) f(slti) f(sltiu) f(xori) f(ori) f(andi) f(slli) f(srli) f(srai)\
    f(add) f(sub) f(slt) f(sltu) f(xor) f(or) f(and) f(sll) f(srl) f(sra)\
    f(inv) f(nemu_trap)\
    f(csrrw) f(csrrs)

def_all_EXEC_ID();
