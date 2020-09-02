#include "lg/call.h"
#include "lg/util.h"

struct lg_call *lg_call_init(struct lg_call *call, struct lg_op *pc, struct lg_op *ret_pc) {
  call->pc = pc;
  call->ret_pc = ret_pc;
  return call;
}
