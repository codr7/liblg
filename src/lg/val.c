#include "lg/type.h"
#include "lg/val.h"

struct lg_val *lg_val_init(struct lg_val *val, struct lg_type *type) {
  val->type = type;
  return val;
}

void lg_val_deinit(struct lg_val *val) {
  if (val->type->deinit_imp) {
    val->type->deinit_imp(val);
  }
}
