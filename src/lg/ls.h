#ifndef LG_LS_H
#define LG_LS_H

#include "lg/util.h"

#define _lg_ls_do(ls, i, _next)\
  for (struct lg_ls *i = (ls)->next, *_next = i->next;\
  i != (ls);\
       i = _next, _next = i->next)

#define lg_ls_do(ls, i)\
  _lg_ls_do(ls, i, lg_unique(next))

struct lg_ls {
  struct lg_ls *prev, *next;
};

void lg_ls_init(struct lg_ls *ls);
void lg_ls_fix(struct lg_ls *ls);

void lg_ls_insert(struct lg_ls *ls, struct lg_ls *item);
struct lg_ls *lg_ls_remove(struct lg_ls *ls);

void lg_ls_push_front(struct lg_ls *ls, struct lg_ls *item);
struct lg_ls *lg_ls_pop_front(struct lg_ls *ls);

void lg_ls_push_back(struct lg_ls *ls, struct lg_ls *item);
struct lg_ls *lg_ls_pop_back(struct lg_ls *ls);

#endif
