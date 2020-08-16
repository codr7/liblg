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
  size_t stack_size, stack_offs;
  size_t pc, sp;
  bool debug;
};

struct lg_vm *lg_vm_init(struct lg_vm *vm);
void lg_vm_deinit(struct lg_vm *vm);

void lg_stack_init(struct lg_vm *vm, size_t n);
struct lg_val *lg_push(struct lg_vm *vm);
struct lg_val *lg_pop(struct lg_vm *vm);

enum lg_op;

uint64_t *lg_emit(struct lg_vm *vm, enum lg_op type);
void lg_exec(struct lg_vm *vm, size_t start_pc);

#endif
