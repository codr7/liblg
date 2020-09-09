#include "lg/pos.h"

struct lg_pos LG_INVALID_POS;

struct lg_pos *lg_pos_init(struct lg_pos *_, int row, int col) {
  _->row = row;
  _->col = col;
  return _;
}
