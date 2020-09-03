#ifndef LG_BSET_H
#define LG_BSET_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "lg/cmp.h"
#include "lg/vec.h"

typedef const void *(*lg_bset_key_t)(const void *);

struct lg_bset {
  lg_cmp_t cmp;
  lg_bset_key_t key;
  struct lg_vec items;
};

struct lg_bset *lg_bset_init(struct lg_bset *_, size_t size, lg_cmp_t cmp, lg_bset_key_t key);
void lg_bset_deinit(struct lg_bset *_);
size_t lg_bset_find(struct lg_bset *_, void *key, bool *ok);
void *lg_bset_get(struct lg_bset *_, void *key);
void *lg_bset_add(struct lg_bset *_, void *key);

#endif
