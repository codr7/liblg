#include <assert.h>

#include "lg/error.h"
#include "lg/op.h"
#include "lg/type.h"
#include "lg/val.h"
#include "lg/vm.h"

struct lg_op *lg_op_init(struct lg_op *op, enum lg_op_code code) {
  op->code = code;
  return op;
}

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

bool lg_eq(struct lg_vm *vm, struct lg_val x, struct lg_val y) {
  struct lg_type *t = x.type;

  if (y.type != t) {
    return false;
  }

  assert(t->eq_imp);
  return t->eq_imp(x, y);
}

void lg_call(struct lg_vm *vm, struct lg_target *tgt) {
  lg_push_call(vm, tgt);
  vm->pc = 0;
}

struct lg_val *lg_clone(struct lg_vm *vm, struct lg_val src) {  
  struct lg_val *dst = lg_push(vm);

  if (src.type->clone_imp) {
    dst->type = src.type;
    src.type->clone_imp(vm, *dst, src);
  } else {
    *dst = src;
  }

  return dst;
}

struct lg_val *lg_cp(struct lg_vm *vm, struct lg_val src) {
  struct lg_val *dst = lg_push(vm);

  if (src.type->cp_imp) {
    dst->type = src.type;
    src.type->cp_imp(vm, *dst, src);
  } else {
    *dst = src;
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

void lg_swap(struct lg_vm *vm) {
  struct lg_val *y = lg_peek(vm), *x = lg_peek(vm)-1; 

  struct lg_val tmp = *x;
  *x = *y;
  *y = tmp;
}
