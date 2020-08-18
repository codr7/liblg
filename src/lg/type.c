#include <stdlib.h>

#include "lg/type.h"
#include "lg/util.h"

struct lg_type *lg_type_init(struct lg_type *type, const char *id) {
  type->id = lg_strdup(id, NULL);  
  type->add_imp = NULL;
  type->cp_imp = NULL;
  type->deinit_imp = NULL;
  type->sub_imp = NULL;
  return type;
}

void lg_type_deinit(struct lg_type *type) {
  free(type->id);
}
