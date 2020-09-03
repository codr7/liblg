#include <assert.h>
#include <stdio.h>

#include "lg/init.h"
#include "lg/op.h"
#include "lg/stack.h"
#include "lg/timer.h"
#include "lg/types/int.h"
#include "lg/val.h"
#include "lg/vm.h"

static void fib(struct lg_vm *vm, struct lg_stack *stack) {  
  size_t fib_pc = vm->ops.len;

  size_t zero_pc = vm->ops.len;
  lg_emit(vm, LG_BIQ)->as_biq.cond = 0;
  
  size_t one_pc = vm->ops.len;
  lg_emit(vm, LG_BIQ)->as_biq.cond = 1;

  lg_emit(vm, LG_DEC);
  lg_emit(vm, LG_CP);
  lg_emit(vm, LG_RCALL);
  lg_emit(vm, LG_SWAP);
  lg_emit(vm, LG_DEC);
  lg_emit(vm, LG_RCALL);
  lg_emit(vm, LG_ADD);

  struct lg_op *op = lg_vec_get(&vm->ops, zero_pc);
  op->as_biq.pc = vm->ops.len;  
  op = lg_vec_get(&vm->ops, one_pc);
  op->as_biq.pc = vm->ops.len;
  lg_emit(vm, LG_RET);
  
  size_t start_pc = vm->ops.len;
  lg_val_init(&lg_emit(vm, LG_PUSH)->as_push.val, &lg_int_type)->as_int = 20;
  lg_emit(vm, LG_CALL)->as_call.pc = fib_pc;
  lg_emit(vm, LG_STOP);
  
  struct lg_timer t;
  lg_timer_init(&t);
    
  for (int i = 0; i < 100; i++) {
    lg_exec(vm, stack, start_pc);
    assert(lg_pop(stack)->as_int == 6765);
  }
  
  printf("%luus\n", lg_timer_usecs(&t));
}

int main() {
  lg_init();
  struct lg_vm vm;
  lg_vm_init(&vm);
  vm.debug = true;
  struct lg_stack stack;
  lg_stack_init(&stack);
  fib(&vm, &stack);
  lg_stack_deinit(&stack);
  lg_vm_deinit(&vm);
  lg_deinit();
  return 0;
}
