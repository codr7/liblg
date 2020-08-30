#ifndef LG_ASM_H
#define LG_ASM_H

#include <stdbool.h>

struct lg_target;

bool lg_asm(struct lg_target *tgt, const char *path);
	      
#endif
