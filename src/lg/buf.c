#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "lg/buf.h"

struct lg_buf *lg_buf_init(struct lg_buf *_) {
  _->data = NULL;
  _->len = _->cap = 0;
  return _;
}

void lg_buf_deinit(struct lg_buf *_) {
  if (_->data) { free(_->data); }
}

void lg_buf_clear(struct lg_buf *_) {
  if (_->data) {
    _->len = 0;
    _->data[0] = 0;
  }
}

void lg_buf_grow(struct lg_buf *_, size_t len) {
  if (_->cap) {
    while (_->cap < len + 1) {
      _->cap *= 2;
    }
  } else {
    _->cap = len + 1;
  }

  _->data = realloc(_->data, _->cap);
}

void lg_printf(struct lg_buf *_, const char *spec, ...) {
  va_list args;
  va_start(args, spec);

  va_list len_args;
  va_copy(len_args, args);
  int len = vsnprintf(NULL, 0, spec, len_args);
  va_end(len_args);

  lg_buf_grow(_, _->len + len);
  vsnprintf(_->data + _->len, len + 1, spec, args);
  va_end(args);
  _->len += len;
}

void lg_putc(struct lg_buf *_, char value) {
  lg_buf_grow(_, _->len + 1);
  _->data[_->len++] = value;
  _->data[_->len] = 0;
}

void lg_puts(struct lg_buf *_, const char *value) {
  size_t len = strlen(value);
  lg_buf_grow(_, _->len + len);
  strncpy(_->data + _->len, value, len+1);
  _->len += len;
}

char *lg_getline(struct lg_buf *_, FILE *in) {
  size_t start = _->len;

  for (;;) {
    char c = fgetc(in);

    if (c == EOF) {
      break;
    }

    lg_putc(_, c);

    if (c == '\n') {
      break;
    }
  }

  return _->data + start;
}
