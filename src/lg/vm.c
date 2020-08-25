#include <stdio.h>
#include <stdlib.h>

#include "lg/call.h"
#include "lg/op.h"
#include "lg/val.h"
#include "lg/vm.h"

struct lg_vm *lg_vm_init(struct lg_vm *vm) {
  lg_target_init(&vm->main, "main");
  lg_vec_init(&vm->calls, sizeof(struct lg_call));
  lg_vec_init(&vm->stack, sizeof(struct lg_val));
  vm->pc = 0;
  vm->debug = false;
  return vm;
}

void lg_vm_deinit(struct lg_vm *vm) {
  lg_target_deinit(&vm->main);
  lg_vec_deinit(&vm->calls);
  lg_vec_deinit(&vm->stack); 
}

struct lg_target *lg_target(struct lg_vm *vm) {
  if (!vm->calls.len) {
    return &vm->main;
  }

  struct lg_call *c = lg_vec_peek(&vm->calls);
  return c->target;
}

struct lg_call *lg_push_call(struct lg_vm *vm, struct lg_target *target) {
  struct lg_call *c = lg_vec_push(&vm->calls);
  c->target = target;
  c->ret_pc = vm->pc;
  return c;
}

struct lg_call *lg_peek_call(struct lg_vm *vm) {
  return lg_vec_peek(&vm->calls);
}

struct lg_call *lg_pop_call(struct lg_vm *vm) {
  return lg_vec_pop(&vm->calls);
}

struct lg_val *lg_push(struct lg_vm *vm) {
  return lg_vec_push(&vm->stack);
}

struct lg_val *lg_peek(struct lg_vm *vm) {
  return lg_vec_peek(&vm->stack); 
}

struct lg_val *lg_pop(struct lg_vm *vm) {
  return lg_vec_pop(&vm->stack); 
}

#define LG_DISPATCH()							\
  op = lg_vec_get(&lg_target(vm)->ops, vm->pc++);			\
  goto *dispatch[op->code]

void lg_exec(struct lg_vm *vm, size_t start_pc) {
  static void* dispatch[] = {NULL, &&add, &&brint, &&call, &&clone, &&cp, &&push, &&ret, &&stop, &&sub, &&swap};
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
 brint: {
    vm->pc = (op->as_brint.cond == lg_pop(vm)->as_int) ? op->as_brint.true_pc : op->as_brint.false_pc;
    LG_DISPATCH(); 
  }
 call: {
    struct lg_target *tgt = NULL;
    
    switch (op->as_call.mode) {
    case LG_CALL_STACK:
      tgt = lg_pop(vm)->as_target;
      break;
    default:
      tgt = lg_target(vm);
      break;
    }

    lg_call(vm, tgt);
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

    if (x.as_int < 0) {
      abort();
    }
    
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
    vm->pc = c->ret_pc;
    LG_DISPATCH();
  }
 stop: {}
}
