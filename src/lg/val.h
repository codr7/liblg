#ifndef LG_VAL_H
#define LG_VAL_H

#include <stdbool.h>
#include <stdint.h>

struct lg_target;

struct lg_val {
  struct lg_type *type;
  
  union {
    bool as_bool;
    int64_t as_int;
    struct lg_target *as_target;
  };
};

struct lg_val *lg_val_init(struct lg_val *val, struct lg_type *type);
void lg_val_deinit(struct lg_val *val);
		   
#endif
