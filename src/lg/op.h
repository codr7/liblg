#ifndef LG_OP_H
#define LG_OP_H

#include <stdbool.h>
#include <stdint.h>

#define LG_OP_CODE_BITS 6

#define LG_OP_CODE(op)				\
  (op & ((1 << LG_OP_CODE_BITS)-1))

typedef uint64_t lg_op_t;

enum lg_op {LG_ADD=1, LG_CP, LG_STOP, LG_SUB, LG_SWAP};

struct lg_val;
struct lg_vm;

bool lg_add(struct lg_vm *vm, struct lg_val x, struct lg_val y);
struct lg_val *lg_cp(struct lg_vm *vm, struct lg_val src);
bool lg_sub(struct lg_vm *vm, struct lg_val x, struct lg_val y);

#endif
