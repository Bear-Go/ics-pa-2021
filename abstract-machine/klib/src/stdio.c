#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  char out[1024];
  va_list ap;
  va_start(ap, fmt);
  int len = vsprintf(out, fmt, ap);
  putstr(out);
  va_end(ap);
  return len;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  memset(out, '\0', strlen(out));
  char *buf = out;
  while (*fmt) {
    if (*fmt != '%') {
      // regular char 
      *buf = *fmt;
      ++buf, ++fmt;
      assert(buf < out + 1024);
      continue;
    }
    // format start at %
    ++fmt;
    // flags
    bool flagzero = 0;
    switch (*fmt) {
      case '0': flagzero = 1; ++fmt; break;
      default : break;
    }

    // width
    size_t width = 0;
    if (*fmt >= '0' && *fmt <= '9') {
      while (*fmt >= '0' && *fmt <= '9') {
        width = width * 10 + *fmt - '0';
        ++fmt;
      }
    }

    switch (*fmt) {
      case 'd': {
        int val = va_arg(ap, int);
        char temp[20];
        size_t len = 0;
        while (val >= 10) {
          int t = val % 10;
          temp[len++] = '0' + t;
          val /= 10;
        }
        temp[len] = '0' + val;
        size_t cnt = len;
        for (; cnt < width; ++cnt) {
          if (flagzero) {
            *buf = '0';
          }
          else {
            *buf = ' ';
          }
          ++buf;
          assert(buf < out + 1024);
        }
        while (len) {
          *buf = temp[--len];
          ++ buf;
          assert(buf < out + 1024);
        }
        ++fmt;
        break;
      }
      case 's': {
        char *p = va_arg(ap, char*);
        size_t len = strlen(p);
        size_t cnt = len;
        for (; cnt < width; ++cnt) {
          if (flagzero) {
            *buf = '0';
          }
          else {
            *buf = ' ';
          }
          ++buf;
          assert(buf < out + 1024);
        }
        strcat(buf, p);
        buf += len + 1;
        ++fmt;
        break;
      }
      case 'x':
      default : assert(0);
    }
  }
  size_t len = strlen(out);
  va_end(ap);
  return len;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int len = vsprintf(out, fmt, ap);
  va_end(ap);
  return len;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  // panic("Not implemented");
  assert(0);
  return 1;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  // panic("Not implemented");
  assert(0);
  return 1;
}

#endif
