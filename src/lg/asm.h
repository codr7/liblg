#ifndef LG_ASM_H
#define LG_ASM_H

#include <stdbool.h>

struct lg_target;
struct lg_vm;

bool lg_asm(struct lg_vm *vm, const char *path);
	      
#endif
