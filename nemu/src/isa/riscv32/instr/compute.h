def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
}

//My own code below

def_EHelper(li) {
  //wait
  rtl_li(s, ddest, id_src1->imm);
}

def_EHelper(auipc) {
  //wait
  rtl_li(s, ddest, id_src1->imm);
}

// def_EHelper(addi) {
//   //wait
//   rtl_li(s, ddest, id_src1->imm);
// }

// def_EHelper(jal) {
//   //wait
//   rtl_li(s, ddest, id_src1->imm);
// }

// def_EHelper(j) {
//   //wait
//   rtl_j(s, id_src1->imm);
// }