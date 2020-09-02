#include <ctype.h>
#include <errno.h>
#include <string.h>
  
#include "lg/asm.h"
#include "lg/bset.h"
#include "lg/buf.h"
#include "lg/error.h"
#include "lg/op.h"
#include "lg/pos.h"
#include "lg/vm.h"

#define LG_LABEL_MAX 32

struct label {
  char id[LG_LABEL_MAX];
  size_t pc;
};

static const char *skipws(const char *in, struct lg_pos *pos) {
  for (;;) {
    switch (*in) {
    case ' ':
      in++;
      pos->col++;
      break;
    default:
      return in;
    }
  }
}

static bool checkid(const char *id, const char *in, size_t len) {
  return len && strncmp(id, in, LG_MIN(strlen(id), len)) == 0;
}

static int char_int(char c, int base) {
  if (isdigit(c)) {
    return c - '0';
  }

  if (base == 16 && c >= 'a' && c <= 'f') {
    return 10 + c - 'a';
  }

  return -1;
}

static const char *parse_int(const char *in,
			     int64_t *val,
			     struct lg_pos *pos) {
  int base = 10;
  const char *start = in;
  *val = 0;
  
  if (*in == '0') {
    in++;
    pos->col++;

    switch (*in) {
    case 'b':
      base = 2;
      in++;
      pos->col++;
      break;
    case 'x':
      base = 16;
      in++;
      pos->col++;
      break;
    }
  }

  char c = 0;

  while ((c = *in)) {
    const int v = char_int(c, base);

    if (v == -1) {
      break;
    }

    *val *= base;
    *val += v;
    in++;
    pos->col++;
  }

  return (in == start) ? NULL : in;
}

static const char *parse_size(const char *in,
			      size_t *val,
			      struct lg_pos *pos) {
  printf("parse_size: %c\n", *(in-1));
  const char *start = in; 
  *val = 0;

  while (isdigit(*in)) {
    *val *= 10;
    *val += *in - '0';
    in++;
    pos->col++;
  }

  return (in == start) ? NULL : in;
}

static const char *parse_label(struct lg_vm *vm,
			       const char *in,
			       size_t *val,
			       struct lg_bset *labels,
			       struct lg_pos *pos) {
  printf("parse_label: %c\n", *in);
  const char *start = in;
  
  while (!isspace(*in)) {
    in++;
    pos->col++;
  }

  const char *end = in;
  size_t len = end - start;

  if (end == start) {
    return NULL;
  }

  char id[LG_LABEL_MAX] = {0};
  strncpy(id, start, len-1);
  struct label *l = lg_bset_get(labels, id);

  if (!l) {
    lg_error(vm, "Unknown label: %s", id);
    return NULL;
  }

  *val = l->pc;
  return in;
}

static const char *parse_add(struct lg_op *op, const char *in) {
  return in;
}

static const char* parse_biq(struct lg_vm *vm,
			     struct lg_op *op,
			     const char *in,
			     struct lg_bset *labels,
			     struct lg_pos *pos) {  
  printf("parse_biq: %c\n", *in);

  return
    ((in = parse_size(in, &op->as_biq.offs, pos)) &&
     (in = parse_int(skipws(in, pos), &op->as_biq.cond, pos)) &&
     (in = parse_label(vm, skipws(in, pos), &op->as_biq.pc, labels, pos))) ? in : NULL;
}

static const char * parse_call(struct lg_op *op, const char *in) {
  return in;
}

static const char * parse_cp(struct lg_op *op, const char *in) {
  return in;
}

static const char * parse_dec(struct lg_op *op, const char *in) {
  return in;
}

static const char * parse_push(struct lg_op *op, const char *in) {
  return in;
}

static const char * parse_ret(struct lg_op *op, const char *in) {
  return in;
}

static const char * parse_stop(struct lg_op *op, const char *in) {
  return in;
}

static const char * parse_swap(struct lg_op *op, const char *in) {
  return in;
}

static bool parse_labels(struct lg_vm *vm, struct lg_bset *labels, const char *in) {
  while (*in) {      
    const char *start = in;
  
    while (!isspace(*in)) {
      in++;
    }
    
    const char *end = in;
    size_t len = end - start;
    
    if (end > start && *(end-1) == ':') {
      char id[LG_LABEL_MAX] = {0};
      strncpy(id, start, len-1);
      struct label *l = lg_bset_add(labels, id);
      
      if (!l) {
	lg_error(vm, "Duplicate label: %s", id);
	return false;
      }
      
      strcpy(l->id, id);
      l->pc = vm->ops.len;
    }
    
    while (*in) {
      char c = *in;
      in++;
      
      if (c == '\n') {
	break;
      }
    }
  }

  return true;
}
  
static const char *parse_op(struct lg_vm *vm, 
			    struct lg_bset *labels,
			    const char *in,
			    struct lg_pos *pos) {
  const char *start = in = skipws(in, pos);
  
  while (!isspace(*in)) {
    in++;
    pos->col++;
  }

  const char *end = in;
  size_t len = end - start;

  if (end == start) {
    return in;
  }

  if (*(end-1) == ':') {
    return in;
  }

  if (checkid("begin", start, len)) {
    return in;
  } else if (checkid("end", start, len)) {
    return in;
  }
  
  enum lg_opcode code;

  if (checkid("add", start, len)) {
    code = LG_ADD;
  } else if (checkid("biq", start, len)) {
    code = LG_BIQ;
  } else if (checkid("call", start, len)) {
    code = LG_CALL;
  } else if (checkid("cp", start, len)) {
    code = LG_CP;
  } else if (checkid("dec", start, len)) {
    code = LG_DEC;
  } else if (checkid("push", start, len)) {
    code = LG_PUSH;
  } else if (checkid("ret", start, len)) {
    code = LG_RET;
  } else if (checkid("stop", start, len)) {
    code = LG_STOP;
  } else if (checkid("swap", start, len)) {
    code = LG_SWAP;
  } else {
    lg_error(vm, "Invalid opcode");
    return NULL;
  }

  struct lg_op *op = lg_emit(vm, code);

  switch (code) {
  case LG_ADD:
    return parse_add(op, skipws(in, pos));
  case LG_BIQ:
    return parse_biq(vm, op, skipws(in, pos), labels, pos);
  case LG_CALL:
    return parse_call(op, skipws(in, pos));
  case LG_CP:
    return parse_cp(op, skipws(in, pos));
  case LG_DEC:
    return parse_dec(op, skipws(in, pos));
  case LG_PUSH:
    return parse_push(op, skipws(in, pos));
  case LG_RET:
    return parse_ret(op, skipws(in, pos));
  case LG_STOP:
    return parse_stop(op, skipws(in, pos));
  case LG_SWAP:
    return parse_swap(op, skipws(in, pos));
  }

  return in;
}

static const void *label_key(const void *x) {
  return ((const struct label *)x)->id;
}

static enum lg_cmp label_cmp(const void *x, const void *y) {
  return lg_strcmp(x, y);
}

bool lg_asm(struct lg_vm *vm, const char *path) {
  struct lg_buf buf;
  lg_buf_init(&buf);

  FILE *f = fopen(path, "r");

  if (!f) {
    lg_error(vm, "Failed opening file '%s': %d", path, errno);
    lg_buf_deinit(&buf);
    return false;
  }

  struct lg_bset labels;
  lg_bset_init(&labels, sizeof(struct label), label_cmp, label_key);
	    
  struct lg_pos pos;
  lg_pos_init(&pos, 1, 0);
  while (*lg_getline(&buf, f));

  if (!parse_labels(vm, &labels, buf.data)) {
    return false;
  }
  
  const char *in = buf.data;
  
  while ((in = parse_op(vm, &labels, in, &pos))) {
    pos.row++;
  }

  lg_bset_deinit(&labels);
  lg_buf_deinit(&buf);
  fclose(f);
  return true;
}
