#include "lg/init.h"
#include "lg/type.h"
#include "lg/types/int.h"

void lg_init() {
  lg_int_type_init();
}

void lg_deinit() {
  lg_type_deinit(&lg_int_type);
}
