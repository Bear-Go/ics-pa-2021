#include <fs.h>

size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  size_t open_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, 0, invalid_read, invalid_write},
  [FD_STDERR] = {"stderr", 0, 0, 0, invalid_read, invalid_write},
#include "files.h"
};

static int file_num;
void init_fs() {
  Log("Initializing file system...");
  file_num = sizeof(file_table) / sizeof(file_table[0]);
}

int fs_open(const char* pathname, int flags, int mode) {
  for (int i = 3; i < file_num; ++ i) {
    if (strcmp(pathname, file_table[i].name) == 0) return i;
  }
  panic("Invalid pathname in fs_open: %s", pathname);
}

size_t fs_read(int fd, void* buf, size_t len) {
  size_t offset = file_table[fd].disk_offset + file_table[fd].open_offset;
  ramdisk_read(buf, offset, len);
  return len;
}

size_t fs_write(int fd, const void* buf, size_t len) {
  return 0;
}

size_t fs_lseek(int fd, size_t offset, int whence) {
  if (whence == SEEK_SET) {
    file_table[fd].open_offset = offset;
  }
  else if (whence == SEEK_CUR) {
    file_table[fd].open_offset += offset;
  }
  else if (whence == SEEK_END) {
    file_table[fd].open_offset += file_table[fd].disk_offset + offset;
  }
  panic("Wrong whence in fs_lseek: %d", whence);
}

int fs_close(int fd) {
  return 0;
}