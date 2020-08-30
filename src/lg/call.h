#ifndef LG_CALL_H
#define LG_CALL_H

#include <stddef.h>

struct lg_call {
  struct lg_target *target;
  struct lg_op *ret_pc;
};

struct lg_call *lg_call_init(struct lg_call *call, struct lg_target *tgt, struct lg_op *ret_pc);
void lg_call_deinit(struct lg_call *call);

#endif
