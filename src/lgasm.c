#include <string.h>

#include "lg/asm.h"
#include "lg/init.h"
#include "lg/vm.h"

int main(int argc, char *argv[]) {
  struct lg_vm vm;
  lg_vm_init(&vm);
  
  while (--argc && ++argv) {
    const char *a = *argv;

    if (strcmp(a, "--debug") == 0) {
      vm.debug = true;
    } else {
      if (!lg_asm(&vm.main, a)) {
	break;
      }
	      
      lg_exec(&vm, 0);
    }
  }

  if (vm.debug) {
    lg_deinit(&vm);
    lg_deinit();
  }
  
  return 0;
}
