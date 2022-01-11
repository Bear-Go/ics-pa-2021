#include <common.h>
#include <sys/time.h>
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
      halt(a[1]);
      break;
    case SYS_yield:
      c->GPRx = 0;
      break;    
    case SYS_open:
      c->GPRx = fs_open((const char*)a[1], a[2], a[3]);
      break;
    case SYS_read:
      c->GPRx = fs_read(a[1], (void*)a[2], a[3]);
      break;
    case SYS_write:
      c->GPRx = fs_write(a[1], (const void*)a[2], a[3]);
      break;
    case SYS_close:
      c->GPRx = fs_close(a[1]);
      break;
    case SYS_lseek:
      c->GPRx = fs_lseek(a[1], a[2], a[3]);
      break;
    case SYS_brk:
      c->GPRx = 0;
      break;
    case SYS_gettimeofday: {
      if ((void*)a[1] != NULL) {
        struct timeval* tv = (void*)a[1]; 
        (*tv).tv_usec = io_read(AM_TIMER_UPTIME).us;
        (*tv).tv_sec = io_read(AM_TIMER_UPTIME).us / 1000000;
        c->GPRx = 0;
      }
      else c->GPRx = -1;
      break;
    }
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
