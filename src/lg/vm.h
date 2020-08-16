#ifndef LG_VM_H
#define LG_VM_H

#include <inttypes.h>
#include <stdbool.h>

#include "lg/ls.h"

struct lg_val;

struct lg_vm {
  uint8_t *memory;
  size_t memory_size, memory_use;
  struct lg_ls free;
  uint64_t *code;
  uint64_t *pc;
  struct lg_val *stack;
  bool debug;
};

struct lg_vm *lg_vm_init(struct lg_vm *vm);
void lg_vm_deinit(struct lg_vm *vm);

struct lg_val *lg_push(struct lg_vm *vm);
struct lg_val *lg_pop(struct lg_vm *vm);

void lg_exec(struct lg_vm *vm, uint64_t *start_pc);

#endif
