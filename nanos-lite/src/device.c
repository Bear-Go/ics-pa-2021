#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
  for (int i = 0; i < len; ++ i) {
    putch(((char*)buf)[i]);
  }
  return len;
}

static bool has_kbd;
size_t events_read(void *buf, size_t offset, size_t len) {
  has_kbd = io_read(AM_INPUT_CONFIG).present;
  if (has_kbd) {
    panic("has here");
    AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
    if (ev.keycode == AM_KEY_NONE) return 0;
    len = sprintf(buf, "%s %s\n", ev.keydown ? "kd" : "ku", keyname[ev.keycode]);
  }
  else {
    panic("here");
  }
  return len;
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
