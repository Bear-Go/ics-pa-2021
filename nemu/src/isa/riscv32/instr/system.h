
def_EHelper(csrrw) {
    rtl_hostcall(s, HOSTCALL_CSRRW, ddest, dsrc1, NULL, id_src2->imm);
}

// t = csr[id_src2->imm]
// csr[id_src2->imm] = x[dsrc1]
// x[ddest] = t

def_EHelper(csrrs) {
    rtl_hostcall(s, HOSTCALL_CSRRS, ddest, dsrc1, NULL, id_src2->imm);
}

def_EHelper(ecall) {
    rtl_trap(s, s->pc, 66); // 66
    // rtl_jr(s, t0);
}