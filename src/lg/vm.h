#ifndef LG_VM_H
#define LG_VM_H

#include <inttypes.h>
#include <stdbool.h>

#include "lg/ls.h"
#include "lg/op.h"
#include "lg/target.h"

struct lg_val;

struct lg_vm {
  uint8_t *memory;
  size_t memory_size, memory_use;
  struct lg_ls free;
  struct lg_target main;
  ptrdiff_t stack;
  size_t sp;
  bool debug;
};

struct lg_vm *lg_vm_init(struct lg_vm *vm, size_t nops, size_t nstack);
void lg_vm_deinit(struct lg_vm *vm);
void lg_stack_init(struct lg_vm *vm, size_t n);

struct lg_val *lg_push(struct lg_vm *vm);
struct lg_val *lg_peek(struct lg_vm *vm);
struct lg_val *lg_pop(struct lg_vm *vm);

#endif
