#include "lg/error.h"
#include "lg/op.h"
#include "lg/type.h"
#include "lg/val.h"

bool lg_add(struct lg_vm *vm, struct lg_val *x, struct lg_val *y) {
  struct lg_type *t = x->type;

  if (y->type != t) {
    lg_error(vm, "Expected type %s, actual %s", t->id, y->type->id);
    return false;
  }
  
  if (!t->add_imp) {
    lg_error(vm, "Add not implemented for type %s", t->id);
    return false;
  }

  t->add_imp(vm, x, y);
  return true;
}

bool lg_sub(struct lg_vm *vm, struct lg_val *x, struct lg_val *y) {
  return false;
}
