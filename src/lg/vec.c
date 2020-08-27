#include <stdlib.h>
#include <string.h>

#include "lg/vec.h"
#include "lg/util.h"

struct lg_vec *lg_vec_init(struct lg_vec *vec, size_t item_size) {
  vec->item_size = item_size;
  vec->cap = vec->len = 0;
  vec->items = vec->start = vec->end = NULL;
  return vec;
}

void lg_vec_deinit(struct lg_vec *vec) {
  if (vec->items) {
    free(vec->items);
  }
}

void lg_vec_grow(struct lg_vec *vec, size_t cap) {
  vec->cap = cap ? cap : LG_VEC_GROWTH;
  vec->items = realloc(vec->items, vec->item_size*(vec->cap+1));
  vec->start = LG_ALIGN(vec->items, vec->item_size);
  vec->end = vec->start + vec->item_size*vec->len;
}

void lg_vec_clear(struct lg_vec *vec) {
  vec->len = 0;
  vec->end = LG_ALIGN(vec->items, vec->item_size);
}

void *lg_vec_get(struct lg_vec *vec, size_t i) {
  return vec->items ? vec->start + vec->item_size*i : NULL;
}

void *lg_vec_push(struct lg_vec *vec) {
  if (vec->len == vec->cap) {
    lg_vec_grow(vec, vec->cap*LG_VEC_GROWTH);
  }

  void *p = vec->end;
  vec->end += vec->item_size;
  vec->len++;
  return p;
}

void *lg_vec_peek(struct lg_vec *vec) {
  return vec->len ? vec->end - vec->item_size : NULL;
}

void *lg_vec_pop(struct lg_vec *vec) {
  if (!vec->len) {
    return NULL;
  }

  vec->end -= vec->item_size;
  vec->len--;
  return vec->end;
}
