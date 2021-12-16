#include <common.h>
#include "syscall.h"

void do_syscall(Context *c) {

  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  switch (a[0]) {
    case SYS_exit:
      halt(a[1]);
      break;
    case SYS_yield: 
      // yield(); // if yield here then mstatus will get diff right = 0x000218000 wrong = 0x00001800
      c->GPRx = 0;
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
