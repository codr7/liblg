#include <string.h>

#include "lg/asm.h"
#include "lg/init.h"
#include "lg/stack.h"
#include "lg/vm.h"

int main(int argc, char *argv[]) {
  struct lg_vm vm;
  lg_vm_init(&vm);
  vm.debug = true;
  
  struct lg_stack stack;
  lg_stack_init(&stack);
  
  while (--argc && ++argv) {
    const char *a = *argv;

    if (strcmp(a, "--debug") == 0) {
      vm.debug = true;
    } else {
      if (!lg_asm(&vm, &vm.main, a)) {
	break;
      }	      
    }
  }

  lg_emit(&vm.main, LG_STOP);
  lg_exec(&vm, &stack, 0);

  if (vm.debug) {
    lg_stack_deinit(&stack);
    lg_vm_deinit(&vm);
    lg_deinit();
  }
  
  return 0;
}
