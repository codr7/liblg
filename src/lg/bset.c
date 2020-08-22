#include <stdlib.h>
#include <string.h>

#include "lg/bset.h"
#include "lg/util.h"

static void *get_item(struct lg_bset *set, size_t i) {
  return set->items ? LG_ALIGN(set->items, set->item_size) + set->item_size*i : NULL;
}

struct lg_bset *lg_bset_new(size_t size, lg_cmp_t cmp) {
  return lg_bset_init(malloc(sizeof(struct lg_bset)), size, cmp);
}

struct lg_bset *lg_bset_init(struct lg_bset *set, size_t item_size, lg_cmp_t cmp) {
  set->items = NULL;
  set->item_size = item_size;
  set->cmp = cmp;
  set->key = NULL;
  set->cap = set->len = 0;
  return set;
}

void lg_bset_deinit(struct lg_bset *set) {
  if (set->items) {
    free(set->items);
  }
}

size_t lg_bset_find(struct lg_bset *set, void *key, bool *ok) {
  size_t min = 0, max = set->len;

  while (min < max) {
    const size_t i = (min+max)/2;
    const void *v = get_item(set, i);
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
  return ok ? get_item(set, i) : NULL;
}

static void grow(struct lg_bset *set, size_t cap) {
  set->cap = cap ? cap : LG_BSET_GROWTH;
  set->items = realloc(set->items, set->item_size * (set->cap+1));
}

static void *insert(struct lg_bset *set, size_t i) {
  if (set->len == set->cap) {
    grow(set, set->cap*LG_BSET_GROWTH);
  }

  uint8_t *const p = get_item(set, i);
  memmove(p+set->item_size, p, (set->len-i)*set->item_size);
  set->len++;
  return p;
}

void *lg_bset_add(struct lg_bset *set, void *key) {
  bool ok = false;
  const size_t i = lg_bset_find(set, key, &ok);

  if (ok) {
    return NULL;
  }

  return insert(set, i);
}
