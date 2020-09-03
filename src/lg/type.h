#ifndef LG_TYPE_H
#define LG_TYPE_H

#include <stdbool.h>

struct lg_val;
struct lg_vm;

struct lg_type {
  char *id;

  void (*add_imp)(struct lg_vm *vm, struct lg_val *x, struct lg_val y);
  void (*clone_imp)(struct lg_vm *vm, struct lg_val src, struct lg_val dst);
  void (*cp_imp)(struct lg_vm *vm, struct lg_val src, struct lg_val dst);
  void (*deinit_imp)(struct lg_val *v);
  void (*sub_imp)(struct lg_vm *vm, struct lg_val *x, struct lg_val y);
};

struct lg_type *lg_type_init(struct lg_type *_, const char *id);
void lg_type_deinit(struct lg_type *_);

#endif
