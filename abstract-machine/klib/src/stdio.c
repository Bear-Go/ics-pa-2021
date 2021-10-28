#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

#define STD_STDIO
#ifdef STD_STDIO

#ifndef PRINTF_NTOA_BUFFER_SIZE
#define PRINTF_NTOA_BUFFER_SIZE    32U
#endif

#define FLAGS_ZEROPAD   (1U <<  0U)
#define FLAGS_LEFT      (1U <<  1U)
#define FLAGS_PLUS      (1U <<  2U)
#define FLAGS_SPACE     (1U <<  3U)
#define FLAGS_HASH      (1U <<  4U)
#define FLAGS_UPPERCASE (1U <<  5U)
#define FLAGS_CHAR      (1U <<  6U)
#define FLAGS_SHORT     (1U <<  7U)
#define FLAGS_LONG      (1U <<  8U)
#define FLAGS_PRECISION (1U << 10U)

///////////////////////////////////////////////////////////////////////////////

typedef void (*out_fct_type)(char character, void* buffer, size_t idx, size_t maxlen);

static inline void _out_buffer(char character, void* buffer, size_t idx, size_t maxlen) {
  if (idx < maxlen) {
    ((char*)buffer)[idx] = character;
  }
}

static inline void _out_null(char character, void* buffer, size_t idx, size_t maxlen) {
  (void)character; (void)buffer; (void)idx; (void)maxlen;
}

static inline void _out_char(char character, void* buffer, size_t idx, size_t maxlen) {
  (void)buffer; (void)idx; (void)maxlen;
  if (character) {
    putch(character);
  }
}

static inline unsigned int _strnlen_s(const char* str, size_t maxsize) {
  const char* s;
  for (s = str; *s && maxsize--; ++s);
  return (unsigned int)(s - str);
}

static inline bool _is_digit(char ch) {
  return (ch >= '0') && (ch <= '9');
}


// internal ASCII string to unsigned int conversion
static unsigned int _atoi(const char** str) {
  unsigned int i = 0U;
  while (_is_digit(**str)) {
    i = i * 10U + (unsigned int)(*((*str)++) - '0');
  }
  return i;
}

static size_t _out_rev(out_fct_type out, char* buffer, size_t idx, size_t maxlen, const char* buf, size_t len, unsigned int width, unsigned int flags) {
  const size_t start_idx = idx;
  // pad spaces up to given width
  if (!(flags & FLAGS_LEFT) && !(flags & FLAGS_ZEROPAD)) {
    for (size_t i = len; i < width; i++) {
      out(' ', buffer, idx++, maxlen);
    }
  }
  // reverse string
  while (len) {
    out(buf[--len], buffer, idx++, maxlen);
  }
  // append pad spaces up to given width
  if (flags & FLAGS_LEFT) {
    while (idx - start_idx < width) {
      out(' ', buffer, idx++, maxlen);
    }
  }
  return idx;
}

static size_t _ntoa_format(out_fct_type out, char* buffer, size_t idx, size_t maxlen, char* buf, size_t len, bool negative, unsigned int base, unsigned int prec, unsigned int width, unsigned int flags) {
  // pad leading zeros
  if (!(flags & FLAGS_LEFT)) {
    if (width && (flags & FLAGS_ZEROPAD) && (negative || (flags & (FLAGS_PLUS | FLAGS_SPACE)))) {
      width--;
    }
    while ((len < prec) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
      buf[len++] = '0';
    }
    while ((flags & FLAGS_ZEROPAD) && (len < width) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
      buf[len++] = '0';
    }
  }
  // handle hash
  if (flags & FLAGS_HASH) {
    if (!(flags & FLAGS_PRECISION) && len && ((len == prec) || (len == width))) {
      len--;
      if (len && (base == 16U)) {
        len--;
      }
    }
    if ((base == 16U) && !(flags & FLAGS_UPPERCASE) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
      buf[len++] = 'x';
    }
    else if ((base == 16U) && (flags & FLAGS_UPPERCASE) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
      buf[len++] = 'X';
    }
    else if ((base == 2U) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
      buf[len++] = 'b';
    }
    if (len < PRINTF_NTOA_BUFFER_SIZE) {
      buf[len++] = '0';
    }
  }
  if (len < PRINTF_NTOA_BUFFER_SIZE) {
    if (negative) {
      buf[len++] = '-';
    }
    else if (flags & FLAGS_PLUS) {
      buf[len++] = '+';
    }
    else if (flags & FLAGS_SPACE) {
      buf[len++] = ' ';
    }
  }
  return _out_rev(out, buffer, idx, maxlen, buf, len, width, flags);
}

static size_t _ntoa_long(out_fct_type out, char* buffer, size_t idx, size_t maxlen, unsigned long value, bool negative, unsigned int base, unsigned int prec, unsigned width, unsigned int flags) {
  char buf[PRINTF_NTOA_BUFFER_SIZE];
  size_t len = 0U;
  // no hash for 0 values
  if (!value) {
    flags &= ~FLAGS_HASH;
  }
  // write if precision != 0 and value is != 0
  if (!(flags & FLAGS_PRECISION) || value) {
    do {
      const char digit = (char)(value % base);
      buf[len++] = digit < 10 ? '0' + digit : (flags & FLAGS_UPPERCASE ? 'A' : 'a') + digit - 10;
      value /= base;
    } while (value && (len < PRINTF_NTOA_BUFFER_SIZE));
  }
  return _ntoa_format(out, buffer, idx, maxlen, buf, len, negative, (unsigned int)base, prec, width, flags);
}

///////////////////////////////////////////////////////////////////////////////

static int _vsnprintf(out_fct_type out, char* buffer, const size_t maxlen, const char* fmt, va_list(va)) {
  unsigned int flags, width, precision, n;
  size_t idx = 0U;

  if (!buffer) {
    out = _out_null;
  }

  while (*fmt) {
    // format ?
    if (*fmt != '%') {
      // no
      out(*fmt, buffer, idx++, maxlen);
      fmt++;
      continue;
    }
    else {
      // yes
      fmt++;
    }

    // flags
    flags = 0U;
    do {
      switch (*fmt) {
        case '0': flags |= FLAGS_ZEROPAD; fmt++; n = 1U; break;
        case '-': flags |= FLAGS_LEFT;    fmt++; n = 1U; break;
        case '+': flags |= FLAGS_PLUS;    fmt++; n = 1U; break;
        case ' ': flags |= FLAGS_SPACE;   fmt++; n = 1U; break;
        case '#': flags |= FLAGS_HASH;    fmt++; n = 1U; break;
        default :                                n = 0U; break;
      }
    } while (n);

    // width
    width = 0U;
    if (_is_digit(*fmt)) {
      width = _atoi(&fmt);
    }
    else if (*fmt == '*') {
      const int w = va_arg(va, int);
      if (w < 0) {
        flags |= FLAGS_LEFT;
        width = (unsigned int) -w;
      }
      else {
        width = (unsigned int) w;
      }
      fmt++;
    }

    // precision
    precision = 0U;
    if (*fmt == '.') {
      flags |= FLAGS_PRECISION;
      fmt++;
      if (_is_digit(*fmt)) {
        precision = _atoi(&fmt);
      }
      else if (*fmt == '*') {
        const int prec = (int)va_arg(va, int);
        precision = prec > 0 ? (unsigned int)prec : 0U;
        fmt++;
      }
    }

    // length
    switch (*fmt) {
      case 'l': 
        flags |= FLAGS_LONG;
        fmt++;
        break;
      case 'h':
        flags |= FLAGS_SHORT;
        fmt++;
        if (*fmt == 'h') {
          flags |= FLAGS_CHAR;
          fmt++;
        }
        break;
    }

    // specifier
    switch (*fmt) {
      case 'd':
      case 'i':
      case 'u':
      case 'x':
      case 'X': {
        // set the base
        unsigned int base;
        if (*fmt == 'x' || *fmt == 'X') {
          base = 16U;
        }
        else {
          base = 10U;
          flags &= ~FLAGS_HASH;
        }
        // uppercase
        if (*fmt == 'X') {
          flags |= FLAGS_UPPERCASE;
        }
        // no plus or space flag for u, x, X
        if ((*fmt != 'i') && (*fmt != 'd')) {
          flags &= ~(FLAGS_PLUS | FLAGS_SPACE);
        }
        // ignore '0' flag when precision is given
        if (flags & FLAGS_PRECISION) {
          flags &= ~FLAGS_ZEROPAD;
        }
        // convert the integer
        if ((*fmt == 'i') || (*fmt == 'd')) {
          // signed
          if (flags & FLAGS_LONG) {
            const long value = va_arg(va, long);
            idx = _ntoa_long(out, buffer, idx, maxlen, (unsigned long)(value > 0 ? value : 0 - value), value < 0, base, precision, width, flags);
          }
          else {
            const int value = (flags & FLAGS_CHAR) ? (char)va_arg(va, int) : (flags & FLAGS_SHORT) ? (short int)va_arg(va, int) : va_arg(va, int);
            idx = _ntoa_long(out, buffer, idx, maxlen, (unsigned int)(value > 0 ? value : 0 - value), value < 0, base, precision, width, flags);
          }
        }
        else {
          // unsigned
          if (flags & FLAGS_LONG) {
            idx = _ntoa_long(out, buffer, idx, maxlen, va_arg(va, unsigned long), false, base, precision, width, flags);
          }
          else {
            const unsigned int value = (flags & FLAGS_CHAR) ? (unsigned char)va_arg(va, unsigned int) : (flags & FLAGS_SHORT) ? (unsigned short int)va_arg(va, unsigned int) : va_arg(va, unsigned int);
            idx = _ntoa_long(out, buffer, idx, maxlen, value, false, base, precision, width, flags);
          }
        }
        fmt++;
        break;
      }

      case 'c': {
        unsigned int l = 1U;
        // pre padding
        if (!(flags & FLAGS_LEFT)) {
          while (l++ < width) {
            out(' ', buffer, idx++, maxlen);
          }
        }
        // char output
        out((char)va_arg(va, int), buffer, idx++, maxlen);
        // post padding
        if (flags & FLAGS_LEFT) {
          while (l++ < width) {
            out(' ', buffer, idx++, maxlen);
          }
        }
        fmt++;
        break;
      }

      case 's': {
        const char* p = va_arg(va, char*);
        unsigned int l = _strnlen_s(p, precision ? precision : (size_t)-1);
        // pre padding
        if (flags & FLAGS_PRECISION) {
          l = (l < precision ? l : precision);
        }
        if (!(flags & FLAGS_LEFT)) {
          while (l++ < width) {
            out(' ', buffer, idx++, maxlen);
          }
        }
        // string output
        while ((*p != 0) && (!(flags & FLAGS_PRECISION) || precision--)) {
          out(*(p++), buffer, idx++, maxlen);
        }
        // post padding
        if (flags & FLAGS_LEFT) {
          while (l++ < width) {
            out(' ', buffer, idx++, maxlen);
          }
        }
        fmt++;
        break;
      }

      case '%':
        out('%', buffer, idx++, maxlen);
        fmt++;
        break;
      default :
        out(*fmt, buffer, idx++, maxlen);
        fmt++;
        break; 
    }
  }

  // termination
  out((char)0, buffer, idx < maxlen ? idx : maxlen - 1U, maxlen);
  return (int) idx;
}

///////////////////////////////////////////////////////////////////////////////

int printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  char buffer[1];
  const int ret = _vsnprintf(_out_char, buffer, (size_t)-1, fmt, ap);
  va_end(ap);
  return ret;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return _vsnprintf(_out_buffer, out, (size_t)-1, fmt, ap);
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  const int ret = _vsnprintf(_out_buffer, out, (size_t)-1, fmt, ap);
  va_end(ap);
  return ret;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  const int ret = _vsnprintf(_out_buffer, out, n, fmt, ap);
  va_end(ap);
  return ret;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  return _vsnprintf(_out_buffer, out, n, fmt, ap);
}

#ifndef STD_STDIO

int printf(const char *fmt, ...) {
  char out[1024];
  memset(out, '\0', strlen(out));
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
          if (flagzero) *buf = '0';
          else *buf = '1';
          ++buf;
          assert(buf < out + 1024);
        }
        len += 1;
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
          if (flagzero) *buf = '0';
          else *buf = '1';
          ++buf;
          assert(buf < out + 1024);
        }
        while (*p != '\0') {
          *buf = *p;
          ++buf; ++p;
        }
        ++fmt;
        break;
      }
      case 'x': putch('\n');putch('x');
      case 'u': putch('\n');putch('u');
      case 'f': putch('\n');putch('f');
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

#endif

#endif
