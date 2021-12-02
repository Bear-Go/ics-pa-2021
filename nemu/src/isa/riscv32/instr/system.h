
def_EHelper(csrrw) {
    rtl_hostcall(s, HOSTCALL_CSRRW, ddest, dsrc1, NULL, id_src2->imm);
}

def_EHelper(csrrs) {
    rtl_hostcall(s, HOSTCALL_CSRRS, ddest, dsrc1, NULL, id_src2->imm);
}

def_EHelper(ecall) {
    rtl_trap(s, s->pc, 66); // 66
}

def_EHelper(mret) {
    rtl_hostcall(s, HOSTCALL_MRET, t0, NULL, NULL, 0);
    printf("0x%08x\n", *t0);
    rtl_jr(s, t0);
    panic("here");
}