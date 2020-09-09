#ifndef LG_POS_H
#define LG_POS_H

struct lg_pos {
  int row, col;
};

extern struct lg_pos LG_INVALID_POS;

struct lg_pos *lg_pos_init(struct lg_pos *_, int row, int col);

#endif
