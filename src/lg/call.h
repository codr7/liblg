#ifndef LG_CALL_H
#define LG_CALL_H

#include <stddef.h>

struct lg_call {
  struct lg_target *target;
  size_t ret_pc;
};

#endif
