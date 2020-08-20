#include "lg/types/target.h"
#include "lg/type.h"
#include "lg/val.h"
#include "lg/vm.h"

struct lg_type lg_target_type;

void lg_target_type_init() {
  lg_type_init(&lg_target_type, "Target");
}
