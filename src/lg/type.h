#ifndef LG_TYPE_H
#define LG_TYPE_H

struct lg_val;
struct lg_vm;

struct lg_type {
  char *id;

  void (*add_imp)(struct lg_vm *vm, struct lg_val *x, struct lg_val *y);
  void (*deinit_imp)(struct lg_val *val);
};

struct lg_type *lg_type_init(struct lg_type *type, const char *id);
void lg_type_deinit(struct lg_type *type);

#endif
