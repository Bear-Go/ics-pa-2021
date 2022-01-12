#include <common.h>
#include <sys/time.h>
#include "syscall.h"
#include <proc.h>

int fs_open(const char* pathname, int flags, int mode);
size_t fs_read(int fd, void* buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);
size_t sys_gettimeofday(struct timeval* tv, struct timezone* tz);
size_t sys_execve(const char* filename, char* const argv[], char* const envp[]);
void naive_uload(PCB *pcb, const char *filename);

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
    case SYS_execve:
      sys_execve((char*)a[1], (char**)a[2], (char**)a[3]);
      break;
    case SYS_gettimeofday: 
      c->GPRx = sys_gettimeofday((struct timeval *)a[1], (struct timezone *)a[2]);
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}

size_t sys_gettimeofday(struct timeval* tv, struct timezone* tz) {
  if (tv != NULL) {
    tv->tv_usec = io_read(AM_TIMER_UPTIME).us % 1000000;
    tv->tv_sec = io_read(AM_TIMER_UPTIME).us / 1000000;
  }
  if (tz != NULL) {
    panic("timezone unsupport");
  }
  return 0;
}

size_t sys_execve(const char* filename, char* const argv[], char* const envp[]) {
  naive_uload(NULL, filename);
  return -1;
}