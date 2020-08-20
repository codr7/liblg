#ifndef LG_BSET_H
#define LG_BSET_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "lg/cmp.h"

#define LG_BSET_GROWTH 2

typedef const void *(*lg_bset_key_t)(const void *);

struct lg_bset {
  uint8_t *items;
  size_t item_size;
  lg_cmp_t cmp;
  lg_bset_key_t key;
  size_t cap, len;
};

struct lg_bset *lg_bset_new(size_t item_size, lg_cmp_t cmp);
struct lg_bset *lg_bset_init(struct lg_bset *bset, size_t size, lg_cmp_t cmp);
void lg_bset_deinit(struct lg_bset *bset);
size_t lg_bset_find(struct lg_bset *bset, void *key, bool *ok);
void *lg_bset_get(struct lg_bset *bset, void *key);
void *lg_bset_add(struct lg_bset *bset, void *key);

#endif
