
def_EHelper(csrrw) {
    rtl_hostcall(s, HOSTCALL_CSR, ddest, dsrc1, NULL, id_src2->imm);
}

// t = csr[id_src2->imm]
// csr[id_src2->imm] = x[dsrc1]
// x[ddest] = t

def_EHelper(csrrs) {
    rtl_hostcall(s, HOSTCALL_CSR, s0, NULL, NULL, id_src2->imm);
    rtl_or(s, s1, s0, dsrc1);
    rtl_mv(s, ddest, s0);
    rtl_hostcall(s, HOSTCALL_CSR, NULL, s1, NULL, id_src2->imm);
}

def_EHelper(ecall) {
    rtl_trap(s, s->pc, 66); // 66
    // rtl_jr(s, t0);
}