#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t len = 0;
  while (*s) {++ len; ++ s;}
  return len;
}

char *strcpy(char *dst, const char *src) {
  char *ret = dst;
  while (*src) {*dst = *src; ++ dst; ++ src;}
  return ret;
}

char *strncpy(char *dst, const char *src, size_t n) {
  size_t i;
  for (i = 0; i < n && src[i] != '\0'; i++)
    dst[i] = src[i];
  for ( ; i < n; i++)
    dst[i] = '\0';
  return dst;
}

char *strcat(char *dst, const char *src) {
  char *ret = dst;
  while (*dst) ++ dst;
  while (*src) {*dst = *src; ++ dst; ++ src;}
  return ret;
}

int strcmp(const char *s1, const char *s2) {
  int ret = 0;
  while (!(ret = *(unsigned char*) s1 - *(unsigned char*) s2) && *s1) {++ s1; ++ s2;}
  return ret;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  int ret = 0;
  while (!(ret = *(unsigned char*) s1 - *(unsigned char*) s2) && *(unsigned char*) s1 && n -- > 0) {++ s1; ++ s2;}
  return ret;
}

void *memset(void *s, int c, size_t n) {
  unsigned char *p = s;
  while (n > 0) {*(unsigned char*) s = (unsigned char) c; (unsigned char*) ++ s; -- n;}
  return p;
}

void *memmove(void *dst, const void *src, size_t n) {
  unsigned char *ret = dst;
  const unsigned char *s = src;
  if (ret < s)
    while (n > 0) {-- n; *ret = *s; ++ ret; ++ s;}
  else {
    unsigned char *backdst = ret + (n - 1);
    const unsigned char *backsrc = s + (n - 1);
    while (n) {-- n; *backdst = *backsrc; -- backdst; -- backsrc;}
  }
  return dst;
}

void *memcpy(void *out, const void *in, size_t n) {
  unsigned char *ret = out;
  const unsigned char *s = in;
  while (n > 0) {-- n; *ret = *s; ++ ret; ++ s;}
  return out; 
}

int memcmp(const void *s1, const void *s2, size_t n) {
  int ret = 0;
  while (!(ret = *(unsigned char*) s1 - *(unsigned char*) s2) && *(unsigned char*) s1 && n -- > 0) {++ s1; ++ s2;}
  return ret;
}

#endif
