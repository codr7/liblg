#ifndef LG_ERROR_H
#define LG_ERROR_H

struct lg_pos;
struct lg_vm;

void lg_error(struct lg_vm *vm, struct lg_pos pos, const char *spec, ...);

#endif
