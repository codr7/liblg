#ifndef LG_MALLOC_H
#define LG_MALLOC_H

#include <stddef.h>
#include <stdint.h>

#include "lg/ls.h"

struct lg_vm;

struct lg_malloc {
  struct lg_ls ls;
  size_t size;
};

uint8_t *lg_malloc(struct lg_vm *vm, size_t size, size_t n);
void lg_free(struct lg_vm *vm, void *p);

#endif
