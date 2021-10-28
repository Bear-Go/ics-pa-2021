#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
  int i;
  int w = inw(VGACTL_ADDR + 2);  // TODO: get the correct width
  int h = inw(VGACTL_ADDR);  // TODO: get the correct height
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
   for (i = 0; i < w * h; i ++) fb[i] = i;
  outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = inw(VGACTL_ADDR + 2), .height = inw(VGACTL_ADDR),
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  // Put the image stored in pixels into window (FB_ADDR) at (x, y) of size (w * h) 
  int i, j;
  uint32_t *pixels = ctl->pixels;
  uint32_t *fb = (uint32_t *)(uintptr_t) FB_ADDR;
  // int x = ctl->x;
  // int y = ctl->y;
  int w = ctl->w, h = ctl->h;
  fb = fb + 2000;
  for (i = 0; i < w; ++ i) {
    for (j = 0; j < h; ++j) {
      *fb = *pixels;
      fb++; pixels++;
    }
  }
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = false;
}
