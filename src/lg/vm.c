#include <stdlib.h>

#include "lg/op.h"
#include "lg/val.h"
#include "lg/vm.h"

#define LG_DISPATCH()				\
  op = *vm->pc++;				\
  goto *dispatch[LG_OP_CODE(op)]

struct lg_vm *lg_vm_init(struct lg_vm *vm) {
  vm->memory = NULL;
  vm->memory_size = 0;
  vm->memory_use = 0;
  lg_ls_init(&vm->free);
  vm->pc = NULL;
  vm->stack = NULL;
  vm->debug = false;
  return vm;
}

void lg_vm_deinit(struct lg_vm *vm) {
  if (vm->memory) {
    free(vm->memory);
  }
}

struct lg_val *lg_push(struct lg_vm *vm) {
  return vm->stack++;
}

struct lg_val *lg_pop(struct lg_vm *vm) {
  return vm->stack--;
}

void lg_exec(struct lg_vm *vm, uint64_t *start_pc) {
  static void* dispatch[] = {NULL, &&add, &&sub};
  uint64_t op = 0;
  vm->pc = start_pc;
  LG_DISPATCH();
  
  for (;;) {
  add: {
      struct lg_val y = *lg_pop(vm), x = *lg_pop(vm);
      lg_add(vm, &x, &y);
      lg_val_deinit(&x);
      lg_val_deinit(&y);
      LG_DISPATCH();
    }
  sub: {
      struct lg_val y = *lg_pop(vm), x = *lg_pop(vm);
      lg_sub(vm, &x, &y);
      lg_val_deinit(&x);
      lg_val_deinit(&y);
      LG_DISPATCH();
    }
  }
}
