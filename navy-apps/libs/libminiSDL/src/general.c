#include <NDL.h>
#include <stdlib.h>
#include <string.h>

uint8_t* keystate;

int SDL_Init(uint32_t flags) {
  keystate = (uint8_t*)malloc(256 * sizeof(uint8_t));
  memset(keystate, 0, sizeof(keystate));
  return NDL_Init(flags);
}

void SDL_Quit() {
  NDL_Quit();
}

char *SDL_GetError() {
  return "Navy does not support SDL_GetError()";
}

int SDL_SetError(const char* fmt, ...) {
  return -1;
}

int SDL_ShowCursor(int toggle) {
  return 0;
}

void SDL_WM_SetCaption(const char *title, const char *icon) {
}
