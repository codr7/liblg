#include "lg/error.h"
#include "lg/op.h"
#include "lg/type.h"
#include "lg/val.h"
#include "lg/vm.h"

bool lg_add(struct lg_vm *vm, struct lg_val x, struct lg_val y) {
  struct lg_type *t = x.type;

  if (y.type != t) {
    lg_error(vm, "Expected type %s, actual %s", t->id, y.type->id);
    return false;
  }
  
  if (!t->add_imp) {
    lg_error(vm, "Add not implemented for type %s", t->id);
    return false;
  }

  t->add_imp(vm, x, y);
  return true;
}

struct lg_val *lg_cp(struct lg_vm *vm, struct lg_val src) {
  struct lg_val *dst = lg_push(vm);
  *dst = src;

  if (src.type->cp_imp) {
    src.type->cp_imp(vm, *dst, src);
  }

  return dst;
}
	     
bool lg_sub(struct lg_vm *vm, struct lg_val x, struct lg_val y) {
  struct lg_type *t = x.type;

  if (y.type != t) {
    lg_error(vm, "Expected type %s, actual %s", t->id, y.type->id);
    return false;
  }
  
  if (!t->sub_imp) {
    lg_error(vm, "Add not implemented for type %s", t->id);
    return false;
  }

  t->sub_imp(vm, x, y);
  return true;
}
