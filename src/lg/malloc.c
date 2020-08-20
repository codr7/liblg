#include <stdlib.h>

#include "lg/malloc.h"
#include "lg/vm.h"

uint8_t *lg_malloc(struct lg_vm *vm, size_t size, size_t n) {
  size_t s = size*(n+1) + sizeof(struct lg_malloc);

  if (vm->memory_use + s > vm->memory_size) {
    if (!vm->memory_size) {
      vm->memory_size = s;
    } else while (vm->memory_use + s > vm->memory_size) {
	vm->memory_size <<= 1;
      }
    
    vm->memory = realloc(vm->memory, vm->memory_size);
  }

  uint8_t *p = LG_ALIGN(vm->memory + vm->memory_use, LG_MAX(size, sizeof(struct lg_malloc)));
  struct lg_malloc *m = (struct lg_malloc *)p;
  m->size = s;
  vm->memory_use += s;
  return p + sizeof(struct lg_malloc);
}

void lg_free(struct lg_vm *vm, ptrdiff_t p) {
  struct lg_malloc *m = (struct lg_malloc *)(vm->memory + p - sizeof(struct lg_malloc));
  lg_ls_push_front(&vm->free, &m->ls);
}
