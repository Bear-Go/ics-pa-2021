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
  void *base = malloc(ramdisk_size);
  Elf_Ehdr *elf_ehdr = (Elf_Ehdr *)base;
  ramdisk_read(elf_ehdr, 0, ramdisk_size);
  assert(*(uint32_t *)elf_ehdr->e_ident == 0x464c457f);
  Elf_Phdr *elf_phdr = (Elf_Phdr *)(base+elf_ehdr->e_phoff);
  // printf("0x%08x\n", elf_phdr->p_vaddr);
  memcpy((void *)elf_phdr->p_vaddr, base + elf_phdr->p_offset, elf_phdr->p_filesz);
  memset((void *)elf_phdr->p_vaddr + elf_phdr->p_filesz, 0, elf_phdr->p_memsz - elf_phdr->p_filesz);
  panic("here");
  return 0;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

