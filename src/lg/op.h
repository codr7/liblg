#ifndef LG_OP_H
#define LG_OP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "lg/val.h"

enum lg_op_code {LG_ADD=1, LG_BRINT, LG_CALL, LG_CLONE, LG_CP, LG_RET, LG_PUSH, LG_STOP, LG_SUB, LG_SWAP};

enum lg_call_mode {LG_CALL_RECURSIVE, LG_CALL_STACK};

struct lg_brint_op {
  int64_t cond;
  size_t true_pc, false_pc;
};
  
struct lg_call_op {
  enum lg_call_mode mode;
};

struct lg_push_op {
  struct lg_val val;
};
  
struct lg_op {
  enum lg_op_code code;

  union {
    struct lg_brint_op as_brint;
    struct lg_call_op as_call;
    struct lg_push_op as_push;
  };
};

struct lg_vm;

struct lg_op *lg_op_init(struct lg_op *op, enum lg_op_code code);
bool lg_add(struct lg_vm *vm, struct lg_val x, struct lg_val y);
bool lg_eq(struct lg_vm *vm, struct lg_val x, struct lg_val y);
void lg_call(struct lg_target *tgt);
struct lg_val *lg_clone(struct lg_vm *vm, struct lg_val src);
struct lg_val *lg_cp(struct lg_vm *vm, struct lg_val src);
bool lg_sub(struct lg_vm *vm, struct lg_val x, struct lg_val y);
void lg_swap(struct lg_vm *vm);

#endif
