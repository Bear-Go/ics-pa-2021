#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

static uintptr_t loader(PCB *pcb, const char *filename) {
  // assert(*(uint32_t *)elf->e_ident == 0x7f454c46);
  // uint32_t offset = 0x001000;
  // uint32_t virtaddr = 0x80000000;
  // uint32_t filesiz = 0x08edc;
  // uint32_t memsiz = 0x11010;
  // uint32_t *p = malloc(memsiz);
  // memset(p, 0, memsiz);
  // ramdisk_read(p, offset, filesiz);
  // io_write(virtaddr, p);
  panic("here");
  return 0;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

