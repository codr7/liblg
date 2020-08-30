#include "lg/call.h"
#include "lg/target.h"
#include "lg/util.h"

struct lg_call *lg_call_init(struct lg_call *call, struct lg_target *tgt, struct lg_op *ret_pc) {
  call->target = LG_REF(tgt);
  call->ret_pc = ret_pc;
  return call;
}

void lg_call_deinit(struct lg_call *call) {
  LG_DEREF(target, call->target);
}
