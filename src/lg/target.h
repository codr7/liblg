#ifndef LG_TARGET_H
#define LG_TARGET_H

#include <stddef.h>

struct lg_vm;

struct lg_target {
  struct lg_vm *vm;
  char *id;
  ptrdiff_t offset;
};

struct lg_target *lg_target_init(struct lg_target *tgt, struct lg_vm *vm, const char *id, size_t nops);
void lg_target_deinit(struct lg_target *tgt);

struct lg_op *lg_emit(struct lg_target *tgt, enum lg_op_code code);

#endif
