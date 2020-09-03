#include <stdlib.h>
#include <string.h>

#include "lg/vec.h"
#include "lg/util.h"

struct lg_vec *lg_vec_init(struct lg_vec *_, size_t item_size) {
  _->item_size = item_size;
  _->cap = _->len = 0;
  _->items = _->start = _->end = NULL;
  return _;
}

void lg_vec_deinit(struct lg_vec *_) {
  if (_->items) {
    free(_->items);
  }
}

void lg_vec_grow(struct lg_vec *_, size_t cap) {
  _->cap = cap ? cap : LG_VEC_GROWTH;
  _->items = realloc(_->items, _->item_size*(_->cap+1));
  _->start = LG_ALIGN(_->items, _->item_size);
  _->end = _->start + _->item_size*_->len;
}

void lg_vec_clear(struct lg_vec *_) {
  _->len = 0;
  _->end = LG_ALIGN(_->items, _->item_size);
}

void *lg_vec_get(struct lg_vec *_, size_t i) {
  return _->items ? _->start + _->item_size*i : NULL;
}

void *lg_vec_push(struct lg_vec *_) {
  if (_->len == _->cap) {
    lg_vec_grow(_, _->cap*LG_VEC_GROWTH);
  }

  void *p = _->end;
  _->end += _->item_size;
  _->len++;
  return p;
}

void *lg_vec_peek(struct lg_vec *_) {
  return _->len ? _->end - _->item_size : NULL;
}

void *lg_vec_pop(struct lg_vec *_) {
  if (!_->len) {
    return NULL;
  }

  _->end -= _->item_size;
  _->len--;
  return _->end;
}

void *lg_vec_insert(struct lg_vec *_, size_t i) {
  if (_->len == _->cap) {
    lg_vec_grow(_, _->cap*LG_VEC_GROWTH);
  } 

  uint8_t *const p = lg_vec_get(_, i);
  memmove(p+_->item_size, p, (_->len-i)*_->item_size);
  _->len++;
  return p;
}
