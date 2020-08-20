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
  return tgt;
}

void lg_target_deinit(struct lg_target *tgt) {
  free(tgt->id);
  lg_free(tgt->vm, tgt->offset); 
}

struct lg_op *lg_emit(struct lg_target *tgt, enum lg_op_code code) {
  return lg_op_init((struct lg_op *)(tgt->vm->memory + tgt->offset) + tgt->vm->pc++, code);
}
