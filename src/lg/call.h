#ifndef LG_CALL_H
#define LG_CALL_H

#include <stddef.h>

struct lg_call {
  struct lg_op *pc, *ret_pc;
};

struct lg_call *lg_call_init(struct lg_call *call, struct lg_op *pc, struct lg_op *ret_pc);

#endif
