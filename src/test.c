#include <assert.h>
#include <stdio.h>

#include "lg/init.h"
#include "lg/op.h"
#include "lg/target.h"
#include "lg/types/int.h"
#include "lg/types/target.h"
#include "lg/val.h"
#include "lg/vm.h"

void fib_tests(struct lg_vm *vm) {
  size_t start_pc = vm->pc;

  struct lg_op *op = lg_emit(&vm->main, LG_PUSH);
  lg_val_init(&op->as_push.val, &lg_int_type)->as_int = 10;
  
  struct lg_target f;
  lg_target_init(&f, vm, "fib");
  op = lg_emit(&vm->main, LG_PUSH);
  lg_val_init(&op->as_push.val, &lg_target_type)->as_target = &f;
  lg_emit(&vm->main, LG_CALL)->as_call.mode = LG_CALL_STACK;
  
  lg_emit(&f, LG_CP);
  struct lg_brint_op *zero_brint = &lg_emit(&f, LG_BRINT)->as_brint;
  zero_brint->cond = 0;
  zero_brint->false_pc = vm->pc;
  
  lg_emit(&f, LG_CP);
  struct lg_brint_op *one_brint = &lg_emit(&f, LG_BRINT)->as_brint;
  one_brint->cond = 0;
  one_brint->false_pc = vm->pc;

  op = lg_emit(&f, LG_PUSH);
  lg_val_init(&op->as_push.val, &lg_int_type)->as_int = 1;
  lg_emit(&f, LG_SUB);
  
  lg_emit(&f, LG_CP);
  lg_emit(&f, LG_CALL)->as_call.mode = LG_CALL_RECURSIVE;
  lg_emit(&f, LG_SWAP);
  
  op = lg_emit(&vm->main, LG_PUSH);
  lg_val_init(&op->as_push.val, &lg_int_type)->as_int = 1;
  lg_emit(&f, LG_SUB);

  lg_emit(&f, LG_CALL)->as_call.mode = LG_CALL_RECURSIVE;
  lg_emit(&f, LG_ADD);

  zero_brint->true_pc = vm->pc;
  one_brint->true_pc = vm->pc;
  lg_emit(&f, LG_RET);
  
  lg_emit(&vm->main, LG_STOP);
  lg_exec(vm, start_pc);

  lg_target_deinit(&f);
  printf("fib: %ld\n", lg_pop(vm)->as_int);
}

int main() {
  lg_init();
  struct lg_vm vm;
  lg_vm_init(&vm);
  vm.debug = true;

  size_t start_pc = vm.pc;
  lg_emit(&vm.main, LG_ADD);
  lg_emit(&vm.main, LG_STOP);

  lg_val_init(lg_push(&vm), &lg_int_type)->as_int = 7;
  lg_val_init(lg_push(&vm), &lg_int_type)->as_int = 35;
  
  lg_exec(&vm, start_pc);
  assert(lg_pop(&vm)->as_int == 42);
  fib_tests(&vm);
  lg_vm_deinit(&vm);
  lg_deinit();
  return 0;
}
