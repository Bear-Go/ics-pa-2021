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
  Elf_Ehdr *elf_ehdr = malloc(get_ramdisk_size());
  printf("ramdisk_size = %d bytes\n", get_ramdisk_size());
  ramdisk_read(elf_ehdr, 0, get_ramdisk_size());
  // Elf_Phdr *elf_phdr = NULL;
  panic("here");
  assert(*(uint32_t *)elf_ehdr->e_ident == 0xBadC0de);
  panic("here");
  return 0;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

