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

  size_t size = get_ramdisk_size();
  void *buf = malloc(size);
  ramdisk_read(buf, 0, size);
  Elf_Ehdr *ehdr = (Elf_Ehdr *)buf;
  assert(*(uint32_t *)ehdr->e_ident == 0x464c457f);
  printf("%p\n", ehdr->e_entry);

  panic("##!! here !!##");
  return 0;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

