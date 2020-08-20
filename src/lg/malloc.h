#ifndef LG_MALLOC_H
#define LG_MALLOC_H

#include <stddef.h>
#include <stdint.h>

#include "lg/ls.h"

#define LG_MALLOC_HEADER_SIZE sizeof(ptrdiff_t)

typedef ptrdiff_t lg_malloc_t;
struct lg_vm;

uint8_t *lg_malloc(struct lg_vm *vm, size_t size, size_t n);
void lg_free(struct lg_vm *vm, ptrdiff_t p);

#endif
