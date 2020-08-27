#ifndef LG_VEC_H
#define LG_VEC_H

#include <stddef.h>
#include <stdint.h>

#include "lg/util.h"

#define LG_VEC_GROWTH 3

#define _LG_VEC_DO(vec, type, var, _i)		\
  size_t _i = 0;				\
  for (type var = (type)(vec)->start;		\
  _i < (vec)->len;				\
  _i++, var++)

#define LG_VEC_DO(vec, type, var)		\
  _LG_VEC_DO(vec, type, var, LG_UNIQUE(i))

struct lg_vec {
  size_t item_size, cap, len;
  uint8_t *items, *start, *end;
};

struct lg_vec *lg_vec_init(struct lg_vec *vec, size_t size);
void lg_vec_deinit(struct lg_vec *vec);
void lg_vec_grow(struct lg_vec *vec, size_t cap);
void lg_vec_clear(struct lg_vec *vec);
void *lg_vec_get(struct lg_vec *vec, size_t i);
void *lg_vec_push(struct lg_vec *vec);
void *lg_vec_peek(struct lg_vec *vec);
void *lg_vec_pop(struct lg_vec *vec);

#endif
