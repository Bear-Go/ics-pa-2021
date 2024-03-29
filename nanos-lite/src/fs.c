#include <fs.h>

size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);
size_t serial_write(const void *buf, size_t offset, size_t len);
size_t events_read(void *buf, size_t offset, size_t len);
size_t dispinfo_read(void *buf, size_t offset, size_t len);
size_t fb_write(const void *buf, size_t offset, size_t len);

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

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB, FD_EVENTS, FD_DISPINFO};

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
  [FD_STDOUT] = {"stdout", 0, 0, 0, invalid_read, serial_write},
  [FD_STDERR] = {"stderr", 0, 0, 0, invalid_read, serial_write},
  [FD_FB]     = {"/dev/fb", 0, 0, 0, invalid_read, fb_write},
  [FD_EVENTS] = {"/dev/events", 0, 0, 0, events_read, invalid_write},
  [FD_DISPINFO] = {"/proc/dispinfo", 0, 0, 0, dispinfo_read, invalid_write},
#include "ramdisk.h"
};

static int file_num;
void init_fs() {
  Log("Initializing file system...");
  file_num = sizeof(file_table) / sizeof(file_table[0]);
  file_table[FD_FB].size = io_read(AM_GPU_CONFIG).width * io_read(AM_GPU_CONFIG).height * 4;
}

int fs_open(const char* pathname, int flags, int mode) {
  for (int i = 3; i < file_num; ++ i) {
    if (strcmp(pathname, file_table[i].name) == 0) {
      return i;
    }
  }
  panic("Invalid pathname in fs_open: %s", pathname);
}

size_t fs_read(int fd, void* buf, size_t len) {
  if (file_table[fd].read) {
    len = file_table[fd].read(buf, 0, len);
  }
  else {
    if (file_table[fd].open_offset > file_table[fd].size)
      len = 0;
    else if (file_table[fd].open_offset + len >= file_table[fd].size)
      len = file_table[fd].size - file_table[fd].open_offset;
    
    size_t offset = file_table[fd].disk_offset + file_table[fd].open_offset;
    ramdisk_read(buf, offset, len);
  }
  file_table[fd].open_offset += len;
  return len;
}

size_t fs_write(int fd, const void* buf, size_t len) {
  if (file_table[fd].write) {
    len = file_table[fd].write(buf, file_table[fd].open_offset, len);
  }
  else {
    if (file_table[fd].open_offset > file_table[fd].size)
      len = 0;
    else if (file_table[fd].open_offset + len >= file_table[fd].size)
      len = file_table[fd].size - file_table[fd].open_offset;
    
    size_t offset = file_table[fd].disk_offset + file_table[fd].open_offset;
    ramdisk_write(buf, offset, len);
  }
  file_table[fd].open_offset += len;
  return len;
}

size_t fs_lseek(int fd, size_t offset, int whence) {
  switch (whence) {
    case SEEK_SET:
      file_table[fd].open_offset = offset;
      break;
    case SEEK_CUR:
      file_table[fd].open_offset += offset;
      break;
    case SEEK_END:
      file_table[fd].open_offset = file_table[fd].size + offset;
      break;
    default: 
      panic("Wrong whence in fs_lseek: %d", whence);
      break;
  }
  return file_table[fd].open_offset;
}

int fs_close(int fd) {
  file_table[fd].open_offset = 0;
  return 0;
}
