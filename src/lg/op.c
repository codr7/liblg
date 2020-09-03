#include <assert.h>

#include "lg/call.h"
#include "lg/error.h"
#include "lg/op.h"
#include "lg/stack.h"
#include "lg/type.h"
#include "lg/val.h"
#include "lg/vm.h"

struct lg_op *lg_op_init(struct lg_op *_, enum lg_opcode code) {
  _->code = code;

  switch (_->code) {
  case LG_BIQ:
    _->as_biq.i = 0;
    _->as_biq.pc = -1;
    break;
  case LG_CALL:
    _->as_call.pc = -1;
    break;
  case LG_DEC:
    _->as_dec.i = 0;
    break;
  case LG_JMP:
    _->as_jmp.pc = -1;
    break;
  default:
    break;
  }

  return _;
}

void lg_op_deinit(struct lg_op *_) {
  switch (_->code) {
  case LG_PUSH:
    lg_val_deinit(&_->as_push.val);
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

void lg_call(struct lg_vm *vm, struct lg_op *pc) {
  lg_call_init(lg_vec_push(&vm->calls), pc, vm->pc);
  vm->pc = pc;
}

struct lg_val *lg_clone(struct lg_vm *vm, struct lg_stack *stack, struct lg_val src) {  
  struct lg_val *dst = lg_push(stack);

  if (src.type->clone_imp) {
    dst->type = src.type;
    src.type->clone_imp(vm, *dst, src);
  } else {
    *dst = src;
  }

  return dst;
}

struct lg_val *lg_cp(struct lg_vm *vm, struct lg_stack *stack, struct lg_val src) {
  struct lg_val *dst = lg_push(stack);

  if (src.type->cp_imp) {
    dst->type = src.type;
    src.type->cp_imp(vm, *dst, src);
  } else {
    *dst = src;
  }

  return dst;
}
