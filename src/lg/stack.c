#include "lg/stack.h"
#include "lg/val.h"

struct lg_stack *lg_stack_init(struct lg_stack *_) {
  lg_vec_init(&_->items, sizeof(struct lg_val));
  return _;
}

void lg_stack_deinit(struct lg_stack *_) {
  LG_VEC_DO(&_->items, struct lg_val *, v) {
    lg_val_deinit(v);
  }
  
  lg_vec_deinit(&_->items); 
}

struct lg_val *lg_push(struct lg_stack *_) {
  return lg_vec_push(&_->items);
}

struct lg_val *lg_peek(struct lg_stack *_) {
  return lg_vec_peek(&_->items); 
}

struct lg_val *lg_pop(struct lg_stack *_) {
  return lg_vec_pop(&_->items); 
}

void lg_swap(struct lg_stack *_) {
  struct lg_val *y = lg_peek(_), *x = y-1, tmp = *x;
  *x = *y;
  *y = tmp;
}

bool lg_drop(struct lg_stack *_, size_t i, size_t n) {
  return lg_vec_delete(&_->items, i, n);
}
