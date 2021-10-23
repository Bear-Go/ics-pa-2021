#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  assert(0);
  return 1;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  // panic("Not implemented");
  assert(0);
  return 1;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  memset(out, '\0', strlen(out));
  char *buf = out;
  while (*fmt) {
    if (*fmt != '%') {
      // regular char 
      *buf = *fmt;
      ++buf, ++fmt;
      continue;
    }
    // format start at %
    ++fmt;
    switch (*fmt) {
      case 'd': {
        int val = va_arg(ap, int);
        char temp[20];
        int cnt = 0;
        while (val >= 10) {
          int t = val % 10;
          temp[cnt++] = '0' + t;
          val /= 10;
        }
        temp[cnt++] = '0' + val;
        while (cnt) {
          *buf = temp[--cnt];
          ++ buf;
        }
        ++fmt;
        break;
      }
      case 's': {
        char *p = va_arg(ap, char*);
        size_t len = strlen(p);
        strcat(buf, p);
        buf += len + 1;
        ++fmt;
        break;
      }
      default : assert(0);
    }
  }
  size_t len = strlen(out);
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
