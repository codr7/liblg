#include <stdlib.h>
#include <string.h>

#include "lg/util.h"

char *lg_strdup(const char *in, size_t *out_len) {
  const size_t len = strlen(in);
  char *out = malloc(len+1);
  strcpy(out, in);

  if (out_len) {
    *out_len = len;
  }

  return out;
}
