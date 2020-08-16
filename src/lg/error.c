#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "lg/error.h"
#include "lg/vm.h"

void lg_error(struct lg_vm *vm, const char *spec, ...) {
  va_list args;
  va_start(args, spec);

  if (vm->debug) {
    vfprintf(stderr, spec, args);
    abort();
  }

  va_end(args);
}
