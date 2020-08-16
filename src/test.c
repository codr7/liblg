#include <assert.h>
#include <stdbool.h>

#include "lg/init.h"

int main() {
  lg_init();

  assert(true);

  lg_deinit();
  return 0;
}
