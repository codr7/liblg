#include "lg/pos.h"

struct lg_pos *lg_pos_init(struct lg_pos *pos, int row, int col) {
  pos->row = row;
  pos->col = col;
  return pos;
}
