#include "lg/call.h"

struct lg_call *lg_call_init(struct lg_call *call, struct lg_target *tgt, struct lg_op *ret_pc) {
  call->target = tgt;
  call->ret_pc = ret_pc;
  return call;
}

void lg_call_deinit(struct lg_call *call) {
  //lg_target_deref(call->target);
}
