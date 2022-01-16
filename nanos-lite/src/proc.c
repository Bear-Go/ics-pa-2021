#include <proc.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;
Context* schedule(Context *prev);

void naive_uload(PCB *pcb, const char *filename);
void context_kload(PCB* new_pcb, void (*entry)(void *), void* arg);
Context *kcontext(Area kstack, void (*entry)(void *), void *arg);

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {\
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite with arg '%p' for the %dth time!", (uintptr_t)arg, j);
    j ++;
    yield();
  }
}

void init_proc() {
  context_kload(&pcb[0], hello_fun, (void*) 0xffffffff);
  context_kload(&pcb[1], hello_fun, (void*) 0xaaaaaaaa);
  switch_boot_pcb();

  Log("Initializing processes...");

  // load program here
  // naive_uload(NULL, "/bin/menu");
}

static int cnt = 1;
Context* schedule(Context *prev) {
  // panic("here");
  current->cp = prev;
  current = &pcb[cnt % 2];
  ++ cnt;
  return current->cp;
}

void context_kload(PCB* new_pcb, void (*entry)(void *), void* arg) {
  Area stack = {new_pcb->stack, new_pcb->stack + STACK_SIZE};
  new_pcb->cp = kcontext(stack, entry, arg);
}