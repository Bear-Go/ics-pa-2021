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
      printf("exit!\n");
      halt(a[1]);
      break;
    case SYS_yield: 
      printf("yield!\n");
      // if yield here then mstatus will get diff right = 0x000218000 wrong = 0x00001800
      // yield(); 
      c->GPRx = 0;
      break;
    case SYS_write:
      putch('!');
      printf("write!\n");
      printf("a[0]%d\n", a[0]);
      printf("a[1]%d\n", a[1]);
      printf("a[2]%d\n", a[2]);
      printf("a[3]%d\n", a[3]);
      if (a[1] == 1 || a[1] ==  2) {
        for (int i = 0; i < a[3]; ++ i) {
            putch(((char*)a[2])[i]);
        }
        c->GPRx = a[3];
      }
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
