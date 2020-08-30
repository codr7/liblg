#include <stdio.h>
#include <stdlib.h>

#include "lg/call.h"
#include "lg/op.h"
#include "lg/stack.h"
#include "lg/val.h"
#include "lg/vm.h"

struct lg_vm *lg_vm_init(struct lg_vm *vm) {
  lg_target_init(&vm->main, "main");
  vm->target = &vm->main;
  lg_vec_init(&vm->calls, sizeof(struct lg_call));
  vm->pc = NULL;
  vm->debug = false;
  return vm;
}

void lg_vm_deinit(struct lg_vm *vm) {
  lg_target_deinit(&vm->main);

  LG_VEC_DO(&vm->calls, struct lg_call *, c) {
    lg_call_deinit(c);
  }

  lg_vec_deinit(&vm->calls);
}

#define LG_DISPATCH()				\
  op = vm->pc++;				\
  goto *dispatch[op->code]

void lg_exec(struct lg_vm *vm, struct lg_stack *stack, size_t start_pc) {
  static void* dispatch[] = {NULL,
			     &&add,
			     &&brint,
			     &&call, &&cp,
			     &&dec,
			     &&push,
			     &&ret,
			     &&stop, &&swap};
  
  struct lg_op *op = NULL;
  vm->pc = lg_vec_get(&vm->target->ops, start_pc);
  LG_DISPATCH();
  
 add: {
    struct lg_val y = *lg_pop(stack);
    lg_add(vm, lg_peek(stack), y);
    lg_val_deinit(&y);
    LG_DISPATCH();
  }
 brint: {
    vm->pc = lg_vec_get(&vm->target->ops,
			(op->as_brint.cond == lg_peek(stack)->as_int)
			? op->as_brint.true_pc
			: op->as_brint.false_pc);
    LG_DISPATCH(); 
  }
 call: {
    struct lg_target *tgt = NULL;
    
    switch (op->as_call.mode) {
    case LG_CALL_IMMEDIATE: {
      tgt = op->as_call.target;
      break;
    }
    default: {
      tgt = vm->target;
      break;
    }
    }

    lg_call(vm, tgt);
    LG_DISPATCH();
  }
 cp: {
    lg_cp(vm, stack, *lg_peek(stack));
    LG_DISPATCH();
  }
 dec: {
    lg_peek(stack)->as_int--;
    LG_DISPATCH();
  }
 swap: {
    lg_swap(stack);
    LG_DISPATCH();
  }
 push: {
    lg_clone(vm, stack, op->as_push.val);
    LG_DISPATCH();
  }
 ret: {
    struct lg_call *c = lg_vec_pop(&vm->calls);
    vm->pc = c->ret_pc;
    lg_call_deinit(c);
    
    if (vm->calls.len) {
      struct lg_call *c = lg_vec_peek(&vm->calls);
      vm->target = c->target;
    } else {
      vm->target = &vm->main;
    }
    
    LG_DISPATCH();
  }
 stop: {}
}
