#ifndef LG_VEC_H
#define LG_VEC_H

#include <stddef.h>
#include <stdint.h>

#define LG_VEC_GROWTH 3

struct lg_vec {
  size_t item_size, cap, len;
  uint8_t *items;
};

struct lg_vec *lg_vec_init(struct lg_vec *vec, size_t size);
void lg_vec_deinit(struct lg_vec *vec);
void lg_vec_grow(struct lg_vec *vec, size_t cap);
void lg_vec_clear(struct lg_vec *vec);
void *lg_vec_get(struct lg_vec *vec, size_t i);
void *lg_vec_push(struct lg_vec *vec);
void *lg_vec_pop(struct lg_vec *vec);

#endif
