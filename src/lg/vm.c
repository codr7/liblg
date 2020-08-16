#include <stdlib.h>

#include "lg/malloc.h"
#include "lg/op.h"
#include "lg/val.h"
#include "lg/vm.h"

#define LG_DISPATCH()				\
  op = *((lg_op_t *)vm->memory + vm->pc++);	\
  goto *dispatch[LG_OP_CODE(op)]

struct lg_vm *lg_vm_init(struct lg_vm *vm) {
  vm->memory = NULL;
  vm->memory_size = vm->memory_use = 0;
  lg_ls_init(&vm->free);
  vm->pc = vm->sp = 0;
  vm->debug = false;
  return vm;
}

void lg_vm_deinit(struct lg_vm *vm) {
  if (vm->memory) {
    free(vm->memory);
  }
}

size_t lg_pc_init(struct lg_vm *vm, size_t n) {
  size_t prev = vm->pc;
  vm->pc = (lg_malloc(vm, sizeof(lg_op_t), n) - vm->memory) / sizeof(lg_op_t);
  return prev;
}

size_t lg_sp_init(struct lg_vm *vm, size_t n) {
  size_t prev = vm->sp;
  vm->sp = (lg_malloc(vm, sizeof(struct lg_val), n) - vm->memory) / sizeof(struct lg_val);
  return prev;
}
		
struct lg_val *lg_push(struct lg_vm *vm) {
  return (struct lg_val *)vm->memory + vm->sp++;
}

struct lg_val *lg_peek(struct lg_vm *vm) {
  return (struct lg_val *)vm->memory + (vm->sp-1);
}

struct lg_val *lg_pop(struct lg_vm *vm) {
  return (struct lg_val *)vm->memory + --vm->sp;
}

lg_op_t *lg_emit(struct lg_vm *vm, enum lg_op type) {
  lg_op_t *p = (lg_op_t *)vm->memory + vm->pc++;
  *p = type;
  return p;
}

void lg_exec(struct lg_vm *vm, size_t start_pc) {
  static void* dispatch[] = {NULL, &&add, &&sub, &&stop};
  lg_op_t op = 0;
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
