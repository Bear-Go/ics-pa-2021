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

static uint8_t keystate[256];

int SDL_PushEvent(SDL_Event *ev) {
  printf("sdl_pushevent\n");
  assert(0);
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
  char buf[128];
  buf[0] = '\0';
  int keycode = 0;
  if (!NDL_PollEvent(buf, sizeof(buf))) {
    ev->key.keysym.sym = SDLK_NONE;
    ev->type = SDL_USEREVENT;
    return 0;
  }

  char name[16];
  sscanf(buf + 3, "%s", name);
  for (int i = 0; i < key_num; ++ i)
    if (strcmp(keyname[i], name) == 0) {
      keycode = i;
      break;
    }
  if (buf[1] == 'd') {
    ev->type = SDL_KEYDOWN;
    keystate[keycode] = 1;
  }
  else if (buf[1] == 'u') {
    ev->type = SDL_KEYUP;
    keystate[keycode] = 0;
  }
  ev->key.keysym.sym = keycode;
  assert(keycode >= 1 && keycode < key_num);
  return 1;
}

int SDL_WaitEvent(SDL_Event *event) {
  while (!SDL_PollEvent(event)) ;
  return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  printf("PeepEvens not implement\n");
  assert(0);
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  for (int i = 0; i < 256; ++ i) {
    if (keystate[i] == 1) {
      printf("keystate[%d] == 1\n", i);
    }
  }
  return keystate;
}
