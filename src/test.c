#include <assert.h>
#include <stdio.h>

#include "lg/init.h"
#include "lg/malloc.h"
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
  lg_target_init(&f, vm, "fib", 32);
  op = lg_emit(&vm->main, LG_PUSH);
  lg_val_init(&op->as_push.val, &lg_target_type)->as_target = &f;
  lg_emit(&vm->main, LG_CALL)->as_call.mode = LG_CALL_STACK;

  op = lg_emit(&vm->main, LG_PUSH);
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
  lg_emit(&f, LG_RET);
  
  lg_emit(&f, LG_STOP);
  lg_exec(vm, start_pc);

  lg_target_deinit(&f);
  printf("fib: %ld\n", lg_pop(vm)->as_int);
}

int main() {
  lg_init();
  struct lg_vm vm;
  lg_vm_init(&vm, 100, 100, 100);
  vm.debug = true;

  void *p = lg_malloc(&vm, 10, 1);
  lg_free(&vm, p - (void *)vm.memory);

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
