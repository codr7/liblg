#include "lg/types/int.h"
#include "lg/type.h"
#include "lg/val.h"
#include "lg/vm.h"

struct lg_type lg_int_type;

void add_imp(struct lg_vm *vm, struct lg_val x, struct lg_val y) {
  lg_val_init(lg_push(vm), &lg_int_type)->as_int = x.as_int + y.as_int;
}

void sub_imp(struct lg_vm *vm, struct lg_val x, struct lg_val y) {
  lg_val_init(lg_push(vm), &lg_int_type)->as_int = x.as_int - y.as_int;
}

void lg_int_type_init() {
  struct lg_type *t = lg_type_init(&lg_int_type, "Int");
  t->add_imp = &add_imp;
  t->sub_imp = &sub_imp;
}
