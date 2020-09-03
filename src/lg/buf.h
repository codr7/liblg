#ifndef LG_BUF_H
#define LG_BUF_H

#include <stdio.h>

struct lg_buf {
  size_t cap, len;
  char *data;
};

struct lg_buf *lg_buf_init(struct lg_buf *_);
void lg_buf_deinit(struct lg_buf *_);

void lg_buf_clear(struct lg_buf *_);
void lg_buf_grow(struct lg_buf *_, size_t length);
void lg_printf(struct lg_buf *_, const char *spec, ...);
void lg_putc(struct lg_buf *_, char value);
void lg_puts(struct lg_buf *_, const char *value);
char *lg_getline(struct lg_buf *_, FILE *in);

#endif
