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

  Elf_Ehdr *ehdr = (Elf_Ehdr *)malloc(sizeof(Elf_Ehdr));
  ramdisk_read(ehdr, 0, sizeof(Elf_Ehdr));
  assert(*(uint32_t *)ehdr->e_ident == 0x464c457f);

  Elf_Phdr *phdr = (Elf_Phdr *)malloc(sizeof(ehdr->e_phentsize * ehdr->e_phnum));
  printf("%d\n", ehdr->e_phnum);
  for (int i = 0; i < ehdr->e_phnum; ++ i) {
    // printf("%d\n", i);
    if (phdr[i].p_type == PT_LOAD) {
      printf("%d\n", i);
    }
  }
  panic("##!! here !!##");
  return ehdr->e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

