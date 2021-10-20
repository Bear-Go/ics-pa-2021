def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
}

//My own code below

def_EHelper(auipc) {
  rtl_li(s, ddest, s->pc + id_src1->imm);
}
