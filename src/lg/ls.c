#include "lg/ls.h"

void lg_ls_init(struct lg_ls *ls) {
  ls->prev = ls->next = ls;
}

void lg_ls_fix(struct lg_ls *ls) {
  ls->prev->next = ls->next->prev = ls;
}

void lg_ls_insert(struct lg_ls *ls, struct lg_ls *item) {
  item->prev = ls->prev;
  ls->prev->next = item;
  item->next = ls;
  ls->prev = item;
}

struct lg_ls *lg_ls_remove(struct lg_ls *ls) {
  ls->prev->next = ls->next;
  ls->next->prev = ls->prev;
  return ls;
}

void lg_ls_push_front(struct lg_ls *ls, struct lg_ls *item) {
  lg_ls_insert(ls->next, item);
}

struct lg_ls *lg_ls_pop_front(struct lg_ls *ls) {
  struct lg_ls *i = ls->next;
  return (i == ls) ? NULL : lg_ls_remove(i);
}

void lg_ls_push_back(struct lg_ls *ls, struct lg_ls *item) {
  lg_ls_insert(ls, item);
}

struct lg_ls *lg_ls_pop_back(struct lg_ls *ls) {
  struct lg_ls *i = ls->prev;
  return (i == ls) ? NULL : lg_ls_remove(i);
}
