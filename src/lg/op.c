#include <assert.h>

#include "lg/call.h"
#include "lg/error.h"
#include "lg/op.h"
#include "lg/target.h"
#include "lg/type.h"
#include "lg/val.h"
#include "lg/vm.h"

struct lg_op *lg_op_init(struct lg_op *op, enum lg_op_code code) {
  op->code = code;
  return op;
}

void lg_op_deinit(struct lg_op *op) {
  switch (op->code) {
  case LG_PUSH:
    lg_val_deinit(&op->as_push.val);
    break;
  default:
    break;
  }
}

bool lg_add(struct lg_vm *vm, struct lg_val *x, struct lg_val y) {
  struct lg_type *t = x->type;

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

void lg_call(struct lg_vm *vm, struct lg_target *tgt) {
  struct lg_call *c = lg_vec_push(&vm->calls);
  c->target = vm->target = tgt;
  c->ret_pc = vm->pc;
  vm->pc = lg_vec_get(&tgt->ops, 0);
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

void lg_swap(struct lg_vm *vm) {
  struct lg_val *y = lg_peek(vm), *x = y-1, tmp = *x;
  *x = *y;
  *y = tmp;
}
