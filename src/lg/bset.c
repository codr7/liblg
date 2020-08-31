#include <stdlib.h>
#include <string.h>

#include "lg/bset.h"
#include "lg/util.h"

struct lg_bset *lg_bset_init(struct lg_bset *set, size_t item_size, lg_cmp_t cmp, lg_bset_key_t key) {
  set->cmp = cmp;
  set->key = key;
  lg_vec_init(&set->items, item_size);
  return set;
}

void lg_bset_deinit(struct lg_bset *set) {
  lg_vec_deinit(&set->items);
}

size_t lg_bset_find(struct lg_bset *set, void *key, bool *ok) {
  size_t min = 0, max = set->items.len;

  while (min < max) {
    const size_t i = (min+max)/2;
    const void *v = lg_vec_get(&set->items, i);
    const void *k = set->key ? set->key(v) : v;

    switch (set->cmp(key, k)) {
    case LG_LT:
      max = i;
      break;
    case LG_GT:
      min = i+1;
      break;
    default:
      if (ok) {
	*ok = true;
      }

      return i;
    }
  }

  return min;
}

void *lg_bset_get(struct lg_bset *set, void *key) {
  bool ok = false;
  const size_t i = lg_bset_find(set, key, &ok);
  return ok ? lg_vec_get(&set->items, i) : NULL;
}

void *lg_bset_add(struct lg_bset *set, void *key) {
  bool ok = false;
  const size_t i = lg_bset_find(set, key, &ok);
  return ok ? NULL : lg_vec_insert(&set->items, i);
}
