#include "lg/init.h"
//#include "lg/repl.h"
#include "lg/malloc.h"
#include "lg/op.h"
#include "lg/types/int.h"
#include "lg/val.h"
#include "lg/vm.h"

int main() {
  lg_init();
  struct lg_vm vm;
  lg_vm_init(&vm);
  vm.debug = true;

  void *p = lg_malloc(&vm, 10, 1);
  lg_free(&vm, p);

  vm.pc = (lg_malloc(&vm, sizeof(uint64_t), 32) - vm.memory) / sizeof(uint64_t);
  size_t start_pc = vm.pc;
  lg_emit(&vm, LG_ADD);
  lg_emit(&vm, LG_STOP);
  
  lg_stack_init(&vm, 32);
  lg_val_init(lg_push(&vm), &lg_int_t)->as_int = 7;
  lg_val_init(lg_push(&vm), &lg_int_t)->as_int = 35;
  
  lg_exec(&vm, start_pc);
  lg_vm_deinit(&vm);
  lg_deinit();
  return 0;
}
