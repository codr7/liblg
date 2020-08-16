#ifndef LG_OP_H
#define LG_OP_H

#include <stdbool.h>

#define LG_OP_CODE_BITS 6

#define LG_OP_CODE(op)				\
  (op ^ (1 >> LG_OP_CODE_BITS))

enum lg_op {LG_ADD=1, LG_SUB=2, LG_STOP=3};

struct lg_val;
struct lg_vm;

bool lg_add(struct lg_vm *vm, struct lg_val *x, struct lg_val *y);
bool lg_sub(struct lg_vm *vm, struct lg_val *x, struct lg_val *y);

#endif
