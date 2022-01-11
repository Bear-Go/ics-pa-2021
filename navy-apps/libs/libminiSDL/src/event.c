#include <NDL.h>
#include <SDL.h>
#include <string.h>
#include <assert.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

static int  key_num = sizeof(keyname) / sizeof(keyname[0]);
extern uint8_t* keystate;

int SDL_PushEvent(SDL_Event *ev) {
  printf("sdl_pushevent\n");
  assert(0);
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
  printf("sdl_pushevent\n");
  assert(0);
  return 0;
}

int SDL_WaitEvent(SDL_Event *event) {
  char buf[128];
  buf[0] = '\0';
  int keycode = 0;
  while (!NDL_PollEvent(buf, sizeof(buf)));

  char name[16];
  sscanf(buf + 3, "%s", name);
  for (int i = 0; i < key_num; ++ i)
    if (strcmp(keyname[i], name) == 0) {
      keycode = i;
      break;
    }
  event->type = buf[1] == 'u' ? SDL_KEYUP : SDL_KEYDOWN;
  event->key.keysym.sym = keycode;
  keystate[keycode] == buf[1] == 'u' ? 0 : 1;
  assert(keycode >= 1 && keycode < key_num);
  return 0;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  return NULL;
}
