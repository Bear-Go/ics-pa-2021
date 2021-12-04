#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);
size_t get_ramdisk_size();

static uintptr_t loader(PCB *pcb, const char *filename) {
  // assert(*(uint32_t *)elf->e_ident == 0x7f454c46);
  size_t offset = 0x001000;
  uint8_t *virtaddr = (uint8_t *)0x80000000;
  size_t filesiz = 0x08edc;
  size_t memsiz = 0x11010;
  uint8_t *p = malloc(filesiz);
  ramdisk_read(p, offset, filesiz);
  memcpy(virtaddr, p, filesiz);
  memset(virtaddr+filesiz, 0, memsiz-filesiz);
  panic("here");
  return 0;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

