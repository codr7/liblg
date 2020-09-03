#ifndef LG_VAL_H
#define LG_VAL_H

#include <stdbool.h>
#include <stdint.h>

struct lg_target;

struct lg_val {
  struct lg_type *type;
  
  union {
    int64_t as_int;
  };
};

struct lg_val *lg_val_init(struct lg_val *_, struct lg_type *type);
void lg_val_deinit(struct lg_val *_);
		   
#endif
