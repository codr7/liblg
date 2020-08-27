#include <stdlib.h>

#include "lg/op.h"
#include "lg/target.h"
#include "lg/util.h"
#include "lg/vm.h"

struct lg_target *lg_target_init(struct lg_target *tgt, const char *id) {
  tgt->id = lg_strdup(id, NULL);
  lg_vec_init(&tgt->ops, sizeof(struct lg_op));
  return tgt;
}

void lg_target_deinit(struct lg_target *tgt) {
  free(tgt->id);

  LG_VEC_DO(&tgt->ops, struct lg_op *, op) {
    lg_op_deinit(op);
  }
  
  lg_vec_deinit(&tgt->ops);
}

struct lg_op *lg_emit(struct lg_target *tgt, enum lg_op_code code) {
  return lg_op_init(lg_vec_push(&tgt->ops), code);
}
