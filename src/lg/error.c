#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "lg/error.h"
#include "lg/pos.h"
#include "lg/vm.h"

void lg_error(struct lg_vm *vm, struct lg_pos pos, const char *spec, ...) {
  va_list args;
  va_start(args, spec);

  if (vm->debug) {
    fprintf(stderr, "Error in row %d, column %d: ", pos.row, pos.col);
    vfprintf(stderr, spec, args);
    abort();
  }

  va_end(args);
}
