#include <stdio.h>
#include <stdlib.h>

#include "lg/call.h"
#include "lg/op.h"
#include "lg/stack.h"
#include "lg/val.h"
#include "lg/vm.h"

struct lg_vm *lg_vm_init(struct lg_vm *vm) {
  lg_vec_init(&vm->ops, sizeof(struct lg_op));
  lg_vec_init(&vm->calls, sizeof(struct lg_call));
  vm->pc = NULL;
  vm->debug = false;
  return vm;
}

void lg_vm_deinit(struct lg_vm *vm) {
  LG_VEC_DO(&vm->ops, struct lg_op *, op) {
    lg_op_deinit(op);
  }
  
  lg_vec_deinit(&vm->ops);
  lg_vec_deinit(&vm->calls);
}

struct lg_op *lg_emit(struct lg_vm *vm, enum lg_opcode code) {
  return lg_op_init(lg_vec_push(&vm->ops), code);
}

#define LG_DISPATCH()				\
  op = vm->pc++;				\
  goto *dispatch[op->code]

void lg_exec(struct lg_vm *vm, struct lg_stack *stack, size_t start_pc) {
  static void* dispatch[] = {NULL,
			     &&add,
			     &&biq,
			     &&call, &&cp,
			     &&dec,
			     &&jmp,
			     &&push,
			     &&ret,
			     &&stop, &&swap};
  
  struct lg_op *op = NULL;
  vm->pc = lg_vec_get(&vm->ops, start_pc);
  LG_DISPATCH();
  
 add: {
    struct lg_val y = *lg_pop(stack);
    lg_add(vm, lg_peek(stack), y);
    lg_val_deinit(&y);
    LG_DISPATCH();
  }
 biq: {
    if (op->as_biq.cond == lg_peek(stack)->as_int) {
      vm->pc = lg_vec_get(&vm->ops, op->as_biq.pc);
    }
    
    LG_DISPATCH(); 
  }
 call: {
    struct lg_op *pc = NULL;
    
    switch (op->as_call.mode) {
    case LG_CALL_IMMEDIATE: {
      pc = lg_vec_get(&vm->ops, op->as_call.pc);
      break;
    }
    case LG_CALL_RECURSIVE: {
      struct lg_call *c = lg_vec_peek(&vm->calls);
      pc = c->pc;
      break;
    }
    }

    lg_call(vm, pc);
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
 jmp: {
    vm->pc = lg_vec_get(&vm->ops, op->as_jmp.pc);
    LG_DISPATCH();
  }
 push: {
    lg_clone(vm, stack, op->as_push.val);
    LG_DISPATCH();
  }
 ret: {
    struct lg_call *c = lg_vec_pop(&vm->calls);
    vm->pc = c->ret_pc;
    LG_DISPATCH();
  }
 swap: {
    lg_swap(stack);
    LG_DISPATCH();
  }

 stop: {}
}
