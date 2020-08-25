#include <assert.h>
#include <stdio.h>

#include "lg/init.h"
#include "lg/op.h"
#include "lg/target.h"
#include "lg/timer.h"
#include "lg/types/int.h"
#include "lg/types/target.h"
#include "lg/val.h"
#include "lg/vm.h"

static void fib_tests(struct lg_vm *vm) {
  size_t start_pc = vm->main.ops.len;

  struct lg_op *op = lg_emit(&vm->main, LG_PUSH);
  lg_val_init(&op->as_push.val, &lg_int_type)->as_int = 20;

  struct lg_target f;
  lg_target_init(&f, "fib");
  op = lg_emit(&vm->main, LG_CALL);
  op->as_call.mode = LG_CALL_IMMEDIATE;
  op->as_call.target = &f;
  
  lg_emit(&f, LG_CP);
  size_t zero_pc = f.ops.len;
  op = lg_emit(&f, LG_BRINT);
  op->as_brint.cond = 0;
  op->as_brint.false_pc = f.ops.len;
  
  lg_emit(&f, LG_CP);
  size_t one_pc = f.ops.len;
  op = lg_emit(&f, LG_BRINT);
  op->as_brint.cond = 1;
  op->as_brint.false_pc = f.ops.len;

  op = lg_emit(&f, LG_PUSH);
  lg_val_init(&op->as_push.val, &lg_int_type)->as_int = 1;
  lg_emit(&f, LG_SUB);
  
  lg_emit(&f, LG_CP);
  lg_emit(&f, LG_CALL)->as_call.mode = LG_CALL_RECURSIVE;
  lg_emit(&f, LG_SWAP);
  
  op = lg_emit(&f, LG_PUSH);
  lg_val_init(&op->as_push.val, &lg_int_type)->as_int = 1;
  lg_emit(&f, LG_SUB);

  lg_emit(&f, LG_CALL)->as_call.mode = LG_CALL_RECURSIVE;
  lg_emit(&f, LG_ADD);

  op = lg_vec_get(&f.ops, zero_pc);
  op->as_brint.true_pc = f.ops.len;  
  op = lg_vec_get(&f.ops, one_pc);
  op->as_brint.true_pc = f.ops.len;
  lg_emit(&f, LG_RET);
  
  lg_emit(&vm->main, LG_STOP);
  
  struct lg_timer t;
  lg_timer_init(&t);
    
  for (int i = 0; i < 100; i++) {
    lg_exec(vm, start_pc);
    assert(lg_pop(vm)->as_int == 6765);
  }
  
  printf("%luus\n", lg_timer_usecs(&t));
  lg_target_deinit(&f);
}

void add_tests(struct lg_vm *vm) {
  size_t start_pc = vm->main.ops.len;
  lg_emit(&vm->main, LG_ADD);
  lg_emit(&vm->main, LG_STOP);

  lg_val_init(lg_push(vm), &lg_int_type)->as_int = 7;
  lg_val_init(lg_push(vm), &lg_int_type)->as_int = 35;
  
  lg_exec(vm, start_pc);
  assert(lg_pop(vm)->as_int == 42);
}

int main() {
  lg_init();
  struct lg_vm vm;
  lg_vm_init(&vm);
  vm.debug = true;
  
  fib_tests(&vm);
  
  lg_vm_deinit(&vm);
  lg_deinit();
  return 0;
}
