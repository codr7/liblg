#include "lg/type.h"
#include "lg/val.h"

struct lg_val *lg_val_init(struct lg_val *_, struct lg_type *type) {
  _->type = type;
  return _;
}

void lg_val_deinit(struct lg_val *_) {
  if (_->type->deinit_imp) {
    _->type->deinit_imp(_);
  }
}
