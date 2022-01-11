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

size_t events_read(void *buf, size_t offset, size_t len) {
  AM_INPUT_KEYBRD_T key = io_read(AM_INPUT_KEYBRD);
  int ret = key.keycode;
  if (key.keydown) ret |= 0x8000;
  int kc = ret;
  char tmp[3]="ku";
  if((kc & 0xfff) == AM_KEY_NONE){
    int time = io_read(AM_TIMER_UPTIME).us;
    len = sprintf(buf,"t %d\n", time);
  }
  else {
    if(kc&0x8000)
      tmp[1] = 'd';
    len = sprintf(buf,"%s %s\n",tmp,keyname[kc&0xfff]);
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
