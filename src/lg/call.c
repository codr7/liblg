#include "lg/call.h"
#include "lg/util.h"

struct lg_call *lg_call_init(struct lg_call *_, struct lg_op *pc, struct lg_op *ret_pc) {
  _->pc = pc;
  _->ret_pc = ret_pc;
  return _;
}
