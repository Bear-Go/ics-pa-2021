#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  // string length
  size_t len = 0;
  while (*s) {++ len; ++ s;}
  return len;
  // panic("Not implemented");
}

char *strcpy(char *dst, const char *src) {
  // string copy
  char *ret = dst;
  while (*src) {*dst = *src; ++ dst; ++ src;}
  return ret; 
  // panic("Not implemented");
}

char *strncpy(char *dst, const char *src, size_t n) {
  // string copy n
  size_t i;
  for (i = 0; i < n && src[i] != '\0'; i++)
    dst[i] = src[i];
  for ( ; i < n; i++)
    dst[i] = '\0';
  return dst;
  // panic("Not implemented");
}

char *strcat(char *dst, const char *src) {
  // string cancat
  char *ret = dst;
  while (*dst) ++ dst;
  while (*src) {*dst = *src; ++ dst; ++ src;}
  return ret;
  // panic("Not implemented");
}

int strcmp(const char *s1, const char *s2) {
  // string compare
  int ret = 0;
  while (!(ret = *(unsigned char*) s1 - *(unsigned char*) s2) && *s1) {++ s1; ++ s2;}
  if (ret > 0) return 1;
  else if (ret < 0) return -1;
  else return 0;
  // panic("Not implemented");
}

int strncmp(const char *s1, const char *s2, size_t n) {
  int ret = 0;
  while (!(ret = *(unsigned char*) s1 - *(unsigned char*) s2) && *(unsigned char*) s1 && n -- > 0) {++ s1; ++ s2;}
  if (ret > 0) return 1;
  else if (ret < 0) return -1;
  else return 0;
  // panic("Not implemented");
}

void *memset(void *s, int c, size_t n) {
  // memory set
  unsigned char *p = s;
  while (n > 0) {*p = c; ++ p; -- n;}
  return s;
  // panic("Not implemented");
}

void *memmove(void *dst, const void *src, size_t n) {
  char *ret = dst;
  const char *s = src;
  if (ret < s)
    while (n > 0) {-- n; *ret = *s; ++ ret; ++ s;}
  else {
    unsigned char *backdst = (unsigned char*) dst + (n - 1);
    unsigned char *backsrc = (unsigned char*) src + (n - 1);
    while (n) {-- n; *backdst = *backsrc; -- backdst; -- backsrc;}
  }
  return dst;
  // panic("Not implemented");
}

void *memcpy(void *out, const void *in, size_t n) {
  char *ret = out;
  const char *s = in;
  while (n > 0) {-- n; *ret = *s; ++ ret; ++ s;}
  return out; 
  // panic("Not implemented");
}

int memcmp(const void *s1, const void *s2, size_t n) {
  // string compare
  int ret = 0;
  while (!(ret = *(unsigned char*) s1 - *(unsigned char*) s2) && *(unsigned char*) s1 && n -- > 0) {++ s1; ++ s2;}
  if (ret > 0) return 1;
  else if (ret < 0) return -1;
  else return 0;
  // panic("Not implemented");
}

#endif
