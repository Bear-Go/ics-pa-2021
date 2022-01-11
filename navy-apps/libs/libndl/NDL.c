#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <assert.h>

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;
static int canvas_w = 0, canvas_h = 0;
static uint32_t* canvas;
static FILE* events;
static FILE* dispinfo;
static FILE* fb;

uint32_t NDL_GetTicks() {
  struct timeval tv;
  if (gettimeofday(&tv, NULL) == 0) 
    return tv.tv_usec;
  return 0;
}

int NDL_PollEvent(char *buf, int len) {
  assert(events != NULL);
  fseek(events, 0, SEEK_SET);
  memset(buf, 0, len);
  int ret = fread(buf, 1, len, events);
  if (ret == 0) 
    return 0;
  else {
    for (int i = 0; i < len; ++ i) {
      if (buf[i] == '\n') {
        buf[i] = '\0';
        return 1;
      }
    }
  }
}

void get_displayinfo() {
  assert(dispinfo != NULL);
  int w = 0, h = 0;
  char width[20]={0},height[20]={0},after[20]={0};

  fscanf(dispinfo,"%s",width);
  if(width[strlen(width)-1] == ':') {
    fscanf(dispinfo,"%d", &w);//WIDTH:  W
  }
  else if(width[strlen(width)-1] == 'H') {
    fscanf(dispinfo,"%s",after);
    strcat(width,after);
    fscanf(dispinfo,"%d",&w);
  }//WIDTH   :   W
  else {
    w = 0;
    for(int i = 0;i < strlen(width);i++) {
      if(width[i] > '9'||width[i] < '0') continue;
      w = w * 10 + width[i] - '0';
    }
  }//WIDTH:W

  fscanf(dispinfo,"%s",height);
  if(height[strlen(height)-1] == ':')
    fscanf(dispinfo,"%d",&h);//HEIGHT:   H
  else if(height[strlen(height)-1] == 'T') {
    fscanf(dispinfo,"%s",after);
    strcat(height,after);
    fscanf(dispinfo,"%d",&h);
  }//HEIGHT   :   H
  else {
    h = 0;
    for(int i = 0;i < strlen(height);i++) {
      if(height[i] > '9'||height[i] < '0') continue;
      h = h * 10 + height[i] - '0';
    }
  }//WIDTH:W
  screen_h = h;
  screen_w = w;
}

void NDL_OpenCanvas(int *w, int *h) {
  get_displayinfo();
  if (*w == 0 && *h == 0) {
    *w = screen_w;
    *h = screen_h;
  }
  else {
    *w = (*w > screen_w) ? screen_w : *w;
    *h = (*h > screen_h) ? screen_h : *h;
  }
  canvas_w = *w;
  canvas_h = *h;
  canvas = (uint32_t*)malloc(sizeof(uint32_t) * (*w) * (*h));
  memset(canvas, 0 ,sizeof(canvas));
  printf("screen : h = %d w = %d\n", *h, *w);

  if (getenv("NWM_APP")) {
    printf("getenv(NWM_APP)");
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  }
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
  assert(fb != NULL);
  for(int i = 0;i < h;i ++)
    for(int j = 0;j < w;j ++)
    {
      canvas[(y+i)*canvas_w+x+j] = pixels[i*w+j];
    }
  for(int i = 0;i < canvas_h;i ++)
  {
    fseek(fb,4*((i)*screen_w),SEEK_SET);
    fwrite((void*)(canvas+i*canvas_w),1,4*canvas_w,fb);
  }
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }

  fb = fopen("/dev/fb", "w");
  events = fopen("/dev/events", "r");
  dispinfo = fopen("/proc/dispinfo", "r");
  return 0;
}

void NDL_Quit() {
  fclose(fb);
  fclose(events);
  fclose(dispinfo);
}
