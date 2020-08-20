#include <stdlib.h>

#include "lg/malloc.h"
#include "lg/op.h"
#include "lg/val.h"
#include "lg/vm.h"

struct lg_vm *lg_vm_init(struct lg_vm *vm, size_t nops, size_t nstack) {
  vm->memory = NULL;
  vm->memory_size = vm->memory_use = 0;
  lg_ls_init(&vm->free);
  lg_target_init(&vm->main, vm, "main", nops);
  lg_stack_init(vm, nstack);
  vm->debug = false;
  return vm;
}

void lg_vm_deinit(struct lg_vm *vm) {
  lg_target_deinit(&vm->main);

  if (vm->stack != -1) {
    lg_free(vm, vm->stack);
  }

  if (vm->memory) {
    free(vm->memory);
  }
}

void lg_stack_init(struct lg_vm *vm, size_t n) {
  vm->stack = lg_malloc(vm, sizeof(struct lg_val), n) - vm->memory;
  vm->sp = 0;
}
		
struct lg_val *lg_push(struct lg_vm *vm) {
  return (struct lg_val *)(vm->memory + vm->stack) + vm->sp++;
}

struct lg_val *lg_peek(struct lg_vm *vm) {
  return (struct lg_val *)(vm->memory + vm->stack) + (vm->sp-1);
}

struct lg_val *lg_pop(struct lg_vm *vm) {
  return (struct lg_val *)(vm->memory + vm->stack) + --vm->sp;
}
