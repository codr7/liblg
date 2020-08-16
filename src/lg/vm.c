#include <stdlib.h>

#include "lg/malloc.h"
#include "lg/op.h"
#include "lg/val.h"
#include "lg/vm.h"

#define LG_DISPATCH()				\
  op = *((uint64_t *)vm->memory + vm->pc++);	\
  goto *dispatch[LG_OP_CODE(op)]

struct lg_vm *lg_vm_init(struct lg_vm *vm) {
  vm->memory = NULL;
  vm->memory_size = vm->memory_use = 0;
  lg_ls_init(&vm->free);
  vm->stack_size = vm->stack_offs = 0;
  vm->pc = vm->sp = 0;
  vm->debug = false;
  return vm;
}

void lg_vm_deinit(struct lg_vm *vm) {
  if (vm->memory) {
    free(vm->memory);
  }
}

void lg_stack_init(struct lg_vm *vm, size_t n) {
  vm->sp = (lg_malloc(vm, sizeof(struct lg_val), n) - vm->memory) / sizeof(struct lg_val);
  vm->stack_size = n;
  vm->stack_offs = 0;
}

struct lg_val *lg_push(struct lg_vm *vm) {
  struct lg_val *p = (struct lg_val *)vm->memory + vm->sp;
  vm->sp++;
  vm->stack_offs++;
  return p;
}

struct lg_val *lg_pop(struct lg_vm *vm) {
  vm->sp--;
  vm->stack_offs--;
  return (struct lg_val *)vm->memory + vm->sp;
}

uint64_t *lg_emit(struct lg_vm *vm, enum lg_op type) {
  uint64_t *p = (uint64_t *)vm->memory + vm->pc++;
  *p = type;
  return p;
}

void lg_exec(struct lg_vm *vm, size_t start_pc) {
  static void* dispatch[] = {NULL, &&add, &&sub, &&stop};
  uint64_t op = 0;
  vm->pc = start_pc;
  LG_DISPATCH();
  
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
 stop: {}
}
