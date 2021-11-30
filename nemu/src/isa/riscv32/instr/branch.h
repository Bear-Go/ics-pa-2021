
def_EHelper(beq) {
  rtl_jrelop(s, RELOP_EQ, dsrc1, dsrc2, s->pc + id_dest->imm);
}

def_EHelper(bne) {
  rtl_jrelop(s, RELOP_NE, dsrc1, dsrc2, s->pc + id_dest->imm);
}

def_EHelper(blt) {
  rtl_jrelop(s, RELOP_LT, dsrc1, dsrc2, s->pc + id_dest->imm);
}

def_EHelper(bge) {
  rtl_jrelop(s, RELOP_GE, dsrc1, dsrc2, s->pc + id_dest->imm);
}

def_EHelper(bltu) {
  rtl_jrelop(s, RELOP_LTU, dsrc1, dsrc2, s->pc + id_dest->imm);
}

def_EHelper(bgeu) {
  rtl_jrelop(s, RELOP_GEU, dsrc1, dsrc2, s->pc + id_dest->imm);
}

def_EHelper(jal) {
  rtl_li(s, ddest, s->snpc);
  rtl_j(s, s->pc + id_src1->imm);
}

def_EHelper(jalr) {
    rtl_li(s, s0, s->snpc);
    rtl_addi(s, s1, dsrc1, id_src2->imm);
    rtl_jr(s, s1);
    rtl_mv(s, ddest, s0);
}