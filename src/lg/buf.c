#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "lg/buf.h"

struct lg_buf *lg_buf_init(struct lg_buf *buf) {
  buf->data = NULL;
  buf->len = buf->cap = 0;
  return buf;
}

void lg_buf_deinit(struct lg_buf *buf) {
  if (buf->data) { free(buf->data); }
}

void lg_buf_clear(struct lg_buf *buf) {
  if (buf->data) {
    buf->len = 0;
    buf->data[0] = 0;
  }
}

void lg_buf_grow(struct lg_buf *buf, size_t len) {
  if (buf->cap) {
    while (buf->cap < len + 1) {
      buf->cap *= 2;
    }
  } else {
    buf->cap = len + 1;
  }

  buf->data = realloc(buf->data, buf->cap);
}

void lg_printf(struct lg_buf *buf, const char *spec, ...) {
  va_list args;
  va_start(args, spec);

  va_list len_args;
  va_copy(len_args, args);
  int len = vsnprintf(NULL, 0, spec, len_args);
  va_end(len_args);

  lg_buf_grow(buf, buf->len + len);
  vsnprintf(buf->data + buf->len, len + 1, spec, args);
  va_end(args);
  buf->len += len;
}

void lg_putc(struct lg_buf *buf, char value) {
  lg_buf_grow(buf, buf->len + 1);
  buf->data[buf->len++] = value;
  buf->data[buf->len] = 0;
}

void lg_puts(struct lg_buf *buf, const char *value) {
  size_t len = strlen(value);
  lg_buf_grow(buf, buf->len + len);
  strncpy(buf->data + buf->len, value, len+1);
  buf->len += len;
}

char *lg_getline(struct lg_buf *buf, FILE *in) {
  size_t start = buf->len;

  for (;;) {
    char c = fgetc(in);

    if (c == EOF) {
      break;
    }

    lg_putc(buf, c);

    if (c == '\n') {
      break;
    }
  }

  return buf->data + start;
}
