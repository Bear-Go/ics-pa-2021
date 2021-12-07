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
  size_t ramdisk_size = get_ramdisk_size(); 
  void *base = malloc(sizeof(ramdisk_size));

  Elf_Ehdr *elf_ehdr = (Elf_Ehdr *)malloc(sizeof(Elf_Ehdr));
  ramdisk_read(elf_ehdr, 0, sizeof(Elf_Ehdr));
  assert(*(uint32_t *)elf_ehdr->e_ident == 0x464c457f);

  int cnt = 0;
  for (int i = 0; i < elf_ehdr->e_phnum; ++i) {
    Elf_Phdr *elf_phdr = (Elf_Phdr *)malloc(sizeof(Elf_Phdr));
    ramdisk_read(elf_phdr, elf_ehdr->e_phoff, sizeof(Elf_Phdr));
    if (elf_phdr->p_type == PT_LOAD) {
      memcpy((void *)elf_phdr->p_vaddr, (void *)base + elf_phdr->p_offset, elf_phdr->p_filesz);
      memset((void *)elf_phdr->p_vaddr + elf_phdr->p_filesz, 0, elf_phdr->p_memsz - elf_phdr->p_filesz);
      ++cnt;
    }
  }
  printf("%d\n", cnt);
  panic("##!! here !!##");
  return elf_ehdr->e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  printf("here 0x%08x\n", entry);
  ((void(*)())entry) ();
}

