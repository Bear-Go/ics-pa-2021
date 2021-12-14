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

  Elf_Phdr *phdr = (Elf_Phdr *)malloc(ehdr->e_phentsize * ehdr->e_phnum);
  ramdisk_read(phdr, ehdr->e_phoff, ehdr->e_phentsize * ehdr->e_phnum);
  printf("%d\n", ehdr->e_phnum);
  for (int i = 0; i < ehdr->e_phnum; ++ i) {
    if (phdr[i].p_type == PT_LOAD) {
      printf("%d\n", i);
      void *buf = malloc(phdr[i].p_filesz);
      ramdisk_read(buf, phdr[i].p_offset, phdr[i].p_filesz);
      memcpy((void *)phdr[i].p_vaddr, buf, phdr[i].p_filesz);
      memset((void *)phdr[i].p_vaddr, 0, phdr[i].p_memsz - phdr[i].p_filesz);
    } 
  }

  printf("0x%08x\n", *(uintptr_t *)(0x83004d10));
  printf("0x%08x\n", *(uintptr_t *)(0x83004d12));

  panic("##!! here !!##");
  return ehdr->e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

