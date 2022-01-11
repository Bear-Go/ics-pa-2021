#include <NDL.h>
#include <SDL.h>
#include <string.h>
#include <assert.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

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
  char buf[32];
  buf[0] = '\0';
  int keycode = 0;
  while (!NDL_PollEvent(buf, sizeof(buf)));

  char* name = buf + 3;
  for (int i = 0, cnt = 0; i < sizeof(keyname); i += strlen(keyname[cnt]), ++ cnt) {
    bool is_find = !strncmp(keyname[cnt], buf, strlen(keyname[cnt]));
    if (is_find) {
      keycode = cnt;
      break;
    }
  }

  if (buf[1] == 'u') {
    event->key.keysym.sym = keycode;
    event->type = SDL_KEYUP;
    keystate[keycode] = 0;
  }
  else if(buf[1] == 'd') {
    event->key.keysym.sym = keycode;
    event->type = SDL_KEYDOWN;
    keystate[keycode] = 1;
  }
  else assert(0);
  return 0;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  return NULL;
}
