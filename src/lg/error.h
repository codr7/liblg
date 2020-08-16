#ifndef LG_ERROR_H
#define LG_ERROR_H

struct lg_vm;

void lg_error(struct lg_vm *vm, const char *spec, ...);

#endif
