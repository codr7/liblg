#include <string.h>

#include "lg/cmp.h"

enum lg_cmp lg_strcmp(const char *x, const char *y) {
  int result = strcmp(x, y);

  if (result < 0) {
    return LG_LT;
  }

  return (result) ? LG_GT : LG_EQ;
}
