#include <stdlib.h>

#include "lg/call.h"
#include "lg/malloc.h"
#include "lg/op.h"
#include "lg/val.h"
#include "lg/vm.h"

static enum lg_cmp cmp_malloc(const void *x, const void *y) {
  int xv = *(const lg_malloc_t *)x, yv = *(const lg_malloc_t *)y;

  if (xv < yv) {
    return LG_LT;
  }

  return (xv > yv) ? LG_GT : LG_EQ;
}

struct lg_vm *lg_vm_init(struct lg_vm *vm, size_t nops, size_t ncallstack, size_t nstack) {
  vm->memory = NULL;
  vm->memory_size = vm->memory_use = 0;
  lg_bset_init(&vm->free, sizeof(lg_malloc_t), cmp_malloc);

  lg_target_init(&vm->main, vm, "main", nops);
  vm->target = &vm->main;
  vm->pc = 0;

  vm->callstack = lg_malloc(vm, sizeof(struct lg_call), ncallstack) - vm->memory;
  vm->csp = 0;

  lg_stack_init(vm, nstack);
  vm->debug = false;
  return vm;
}

void lg_vm_deinit(struct lg_vm *vm) {
  lg_target_deinit(&vm->main);

  if (vm->stack != -1) {
    lg_free(vm, vm->stack);
  }

  if (vm->memory) {
    free(vm->memory);
  }

  lg_bset_deinit(&vm->free);
}

void lg_stack_init(struct lg_vm *vm, size_t n) {
  vm->stack = lg_malloc(vm, sizeof(struct lg_val), n) - vm->memory;
  vm->sp = 0;
}

struct lg_call *lg_push_call(struct lg_vm *vm) {
  struct lg_call *c = (struct lg_call *)(vm->memory + vm->callstack) + vm->csp++;
  c->target = vm->target;
  c->pc = vm->pc;
  return c;
}

struct lg_call *lg_peek_call(struct lg_vm *vm) {
  return (struct lg_call *)(vm->memory + vm->callstack) + (vm->csp-1);
}

struct lg_call *lg_pop_call(struct lg_vm *vm) {
  return (struct lg_call *)(vm->memory + vm->callstack) + --vm->csp;
}

struct lg_val *lg_push(struct lg_vm *vm) {
  return (struct lg_val *)(vm->memory + vm->stack) + vm->sp++;
}

struct lg_val *lg_peek(struct lg_vm *vm) {
  return (struct lg_val *)(vm->memory + vm->stack) + (vm->sp-1);
}

struct lg_val *lg_pop(struct lg_vm *vm) {
  return (struct lg_val *)(vm->memory + vm->stack) + --vm->sp;
}

#define LG_DISPATCH()						\
  op = (struct lg_op *)(vm->memory + vm->target->offset) + vm->pc++;	\
  goto *dispatch[op->code]

void lg_exec(struct lg_vm *vm, size_t start_pc) {
  static void* dispatch[] = {NULL, &&add, &&call, &&clone, &&cp, &&push, &&ret, &&stop, &&sub, &&swap};
  struct lg_op *op = NULL;
  vm->pc = start_pc;
  LG_DISPATCH();
  
 add: {
    struct lg_val y = *lg_pop(vm), x = *lg_pop(vm);
    lg_add(vm, x, y);
    lg_val_deinit(&x);
    lg_val_deinit(&y);
    LG_DISPATCH();
  }
 call: {
    struct lg_target *ct = NULL;
    
    switch (op->as_call.mode) {
    case LG_CALL_STACK:
      ct = lg_pop(vm)->as_target;
      break;
    default:
      ct = vm->target;
      break;
    }
    
    lg_call(ct);
    LG_DISPATCH();
  }
 clone: {
    lg_clone(vm, *lg_peek(vm));
    LG_DISPATCH();
  }
 cp: {
    lg_cp(vm, *lg_peek(vm));
    LG_DISPATCH();
  }
 sub: {
    struct lg_val y = *lg_pop(vm), x = *lg_pop(vm);
    lg_sub(vm, x, y);
    lg_val_deinit(&x);
    lg_val_deinit(&y);
    LG_DISPATCH();
  }
 swap: {
    lg_swap(vm);
    LG_DISPATCH();
  }
 push: {
    lg_clone(vm, op->as_push.val);
    LG_DISPATCH();
  }
 ret: {
    struct lg_call *c = lg_pop_call(vm);
    vm->target = c->target;
    vm->pc = c->pc;
  }
 stop: {}
}
