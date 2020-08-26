#ifndef LG_VM_H
#define LG_VM_H

#include <inttypes.h>
#include <stdbool.h>

#include "lg/op.h"
#include "lg/target.h"
#include "lg/vec.h"

struct lg_val;

struct lg_vm {
  struct lg_target main, *target;
  struct lg_vec calls, stack;
  struct lg_op *pc;
  bool debug;
};

struct lg_vm *lg_vm_init(struct lg_vm *vm);
void lg_vm_deinit(struct lg_vm *vm);

struct lg_val *lg_push(struct lg_vm *vm);
struct lg_val *lg_peek(struct lg_vm *vm);
struct lg_val *lg_pop(struct lg_vm *vm);

void lg_exec(struct lg_vm *vm, size_t start_pc);

#endif
