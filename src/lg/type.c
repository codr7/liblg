#include <stdlib.h>

#include "lg/type.h"
#include "lg/util.h"

struct lg_type *lg_type_init(struct lg_type *_, const char *id) {
  _->id = lg_strdup(id, NULL);  
  _->add_imp = NULL;
  _->clone_imp = NULL;
  _->cp_imp = NULL;
  _->deinit_imp = NULL;
  _->sub_imp = NULL;
  return _;
}

void lg_type_deinit(struct lg_type *_) {
  free(_->id);
}
