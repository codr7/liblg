#include "lg/init.h"
#include "lg/pos.h"
#include "lg/type.h"
#include "lg/types/int.h"

void lg_init() {
  lg_pos_init(&LG_INVALID_POS, -1, -1);
  lg_int_type_init();
}

void lg_deinit() {
  lg_type_deinit(&lg_int_type);
}
