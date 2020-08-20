#include <stdlib.h>

#include "lg/malloc.h"
#include "lg/util.h"
#include "lg/vm.h"

uint8_t *lg_malloc(struct lg_vm *vm, size_t size, size_t n) {
  size_t s = size*(n+1) + LG_MALLOC_HEADER_SIZE;

  if (vm->memory_use + s > vm->memory_size) {
    if (!vm->memory_size) {
      vm->memory_size = s;
    } else while (vm->memory_use + s > vm->memory_size) {
	vm->memory_size <<= 1;
      }
    
    vm->memory = realloc(vm->memory, vm->memory_size);
  }

  uint8_t *p = LG_ALIGN(vm->memory + vm->memory_use, LG_MAX(size, LG_MALLOC_HEADER_SIZE));
  *(lg_malloc_t *)p = s;
  vm->memory_use += s;
  return p + LG_MALLOC_HEADER_SIZE;
}

void lg_free(struct lg_vm *vm, ptrdiff_t p) {
  *(lg_malloc_t *)lg_bset_add(&vm->free, &p) = p;
}
