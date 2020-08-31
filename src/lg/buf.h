#ifndef LG_BUF_H
#define LG_BUF_H

#include <stdio.h>

struct lg_buf {
  size_t cap, len;
  char *data;
};

struct lg_buf *lg_buf_init(struct lg_buf *buf);
void lg_buf_deinit(struct lg_buf *buf);

void lg_buf_clear(struct lg_buf *buf);
void lg_buf_grow(struct lg_buf *buf, size_t length);
void lg_printf(struct lg_buf *buf, const char *spec, ...);
void lg_putc(struct lg_buf *buf, char value);
void lg_puts(struct lg_buf *buf, const char *value);
char *lg_getline(struct lg_buf *buf, FILE *in);

#endif
