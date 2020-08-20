#include <stdlib.h>

#include "lg/malloc.h"
#include "lg/op.h"
#include "lg/target.h"
#include "lg/util.h"
#include "lg/vm.h"

struct lg_target *lg_target_init(struct lg_target *tgt, struct lg_vm *vm, const char *id, size_t nops) {
  tgt->vm = vm;
  tgt->id = lg_strdup(id, NULL);
  tgt->offset = lg_malloc(vm, sizeof(struct lg_op), nops) - vm->memory;
  tgt->pc = 0;
  return tgt;
}

void lg_target_deinit(struct lg_target *tgt) {
  free(tgt->id);
  lg_free(tgt->vm, tgt->offset); 
}

struct lg_op *lg_emit(struct lg_target *tgt, enum lg_op_code code) {
  return lg_op_init((struct lg_op *)tgt->offset + tgt->pc++, code);
}

#define LG_DISPATCH()							\
  op = (struct lg_op *)(vm->memory + vm->main.offset) + tgt->pc++;	\
  goto *dispatch[op->code]

void lg_exec(struct lg_target *tgt, size_t start_pc) {
  static void* dispatch[] = {NULL, &&add, &&call, &&clone, &&cp, &&push, &&stop, &&sub, &&swap};
  struct lg_vm *vm = tgt->vm;
  struct lg_op *op = NULL;
  tgt->pc = start_pc;
  LG_DISPATCH();
  
 add: {
    struct lg_val y = *lg_pop(vm), x = *lg_pop(vm);
    lg_add(vm, x, y);
    lg_val_deinit(&x);
    lg_val_deinit(&y);
    LG_DISPATCH();
  }
 call: {
    LG_DISPATCH();
  }
 clone: {
    lg_clone(vm, *lg_peek(vm));
    LG_DISPATCH();
  }
 cp: {
    lg_cp(vm, *lg_peek(vm));
    LG_DISPATCH();
  }
 sub: {
    struct lg_val y = *lg_pop(vm), x = *lg_pop(vm);
    lg_sub(vm, x, y);
    lg_val_deinit(&x);
    lg_val_deinit(&y);
    LG_DISPATCH();
  }
 swap: {
    lg_swap(vm);
    LG_DISPATCH();
  }
 push: {
    lg_clone(vm, op->as_push.val);
    LG_DISPATCH();
  }
 stop: {}
}

