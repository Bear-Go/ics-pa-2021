#include <common.h>
#include "syscall.h"

int fs_open(const char* pathname, int flags, int mode);
size_t fs_read(int fd, void* buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);

void do_syscall(Context *c) {

  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  switch (a[0]) {
    case SYS_exit:
      // printf("exit!\n");
      halt(a[1]);
      break;
    case SYS_yield: 
      // printf("yield!\n");
      // if yield here then mstatus will get diff right = 0x000218000 wrong = 0x00001800
      // yield(); 
      c->GPRx = 0;
      break;
    case SYS_write:
      if (a[1] == 1 || a[1] ==  2) {
        for (int i = 0; i < a[3]; ++ i) {
            putch(((char*)a[2])[i]);
        }
        c->GPRx = a[3];
      }
      break;
    case SYS_open:
      fs_open((char*)a[1],a[2],a[3]);
      break;
    case SYS_brk:
      c->GPRx = 0;
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
