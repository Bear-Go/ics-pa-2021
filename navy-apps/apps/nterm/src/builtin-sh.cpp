#include <nterm.h>
#include <stdarg.h>
#include <unistd.h>
#include <SDL.h>

char handle_key(SDL_Event *ev);

static void sh_printf(const char *format, ...) {
  static char buf[256] = {};
  va_list ap;
  va_start(ap, format);
  int len = vsnprintf(buf, 256, format, ap);
  va_end(ap);
  term->write(buf, len);
}

static void sh_banner() {
  sh_printf("Built-in Shell in NTerm (NJU Terminal)\n\n");
}

static void sh_prompt() {
  sh_printf("sh> ");
}

static void sh_handle_cmd(const char *cmd) {
  if (!strncmp(cmd, "ls", 2)) {
    sh_printf("/bin/bird\n");
    sh_printf("/bin/nterm\n");
    sh_printf("/bin/pal\n");
    sh_printf("/bin/nslider\n");
    sh_printf("/bin/menu\n");
    sh_printf("\n");
  }
  else if (!strncmp(cmd, "./", 2)) {
    assert(setenv("PATH", "/bin", 0) == 0);
    char* str = (char*)malloc(strlen(cmd) + 16);
    memset(str, 0, strlen(str));
    strcpy(str, cmd + 1);
    int len = strlen(str);
    str[len - 1] = '\0';
    sh_printf("Loading...\n");
    execvp(str, NULL);
  }
  else{
    sh_printf("Command not find\n");
  }
}

void builtin_sh_run() {
  sh_banner();
  sh_prompt();

  while (1) {
    SDL_Event ev;
    if (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_KEYUP || ev.type == SDL_KEYDOWN) {
        const char *res = term->keypress(handle_key(&ev));
        if (res) {
          sh_handle_cmd(res);
          sh_prompt();
        }
      }
    }
    refresh_terminal();
  }
}
