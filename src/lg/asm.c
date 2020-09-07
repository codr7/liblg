#include <ctype.h>
#include <errno.h>
#include <string.h>
  
#include "lg/asm.h"
#include "lg/bset.h"
#include "lg/buf.h"
#include "lg/error.h"
#include "lg/op.h"
#include "lg/pos.h"
#include "lg/types/int.h"
#include "lg/vm.h"

#define LG_LABEL_MAX 32

struct label {
  char id[LG_LABEL_MAX];
  size_t pc;
};

static const char *skipws(const char *in, struct lg_pos *pos) {
  while (*in) {
    switch (*in) {
    case ' ':
      in++;
      pos->col++;
      break;
    case '\n':
      in++;
      pos->row++;
      pos->col = 0;
      break;
    case '\r':
      in++;
      break;
    default:
      return in;
    }
  }

  return in;
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
  const char *start = in; 
  *val = 0;

  while (isdigit(*in)) {
    *val *= 10;
    *val += *in++ - '0';
    pos->col++;
  }

  return (in == start) ? NULL : in;
}

static const char *parse_label(struct lg_vm *vm,
			       const char *in,
			       size_t *val,
			       struct lg_bset *labels,
			       struct lg_pos *pos) {
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
  strncpy(id, start, len);
  struct label *l = lg_bset_get(labels, id);

  if (!l) {
    lg_error(vm, "Unknown label: %s", id);
    return NULL;
  }

  *val = l->pc;
  return in;
}

static const char* parse_beq(struct lg_vm *vm,
			     struct lg_op *op,
			     const char *in,
			     struct lg_bset *labels,
			     struct lg_pos *pos) {  
  return
    ((in = parse_size(in, &op->as_beq.i, pos)) &&
     (in = parse_int(skipws(in, pos), &op->as_beq.cond, pos)) &&
     (in = parse_label(vm, skipws(in, pos), &op->as_beq.pc, labels, pos))) ? in : NULL;
}

static const char* parse_blt(struct lg_vm *vm,
			     struct lg_op *op,
			     const char *in,
			     struct lg_bset *labels,
			     struct lg_pos *pos) {  
  return
    ((in = parse_size(in, &op->as_blt.i, pos)) &&
     (in = parse_int(skipws(in, pos), &op->as_blt.cond, pos)) &&
     (in = parse_label(vm, skipws(in, pos), &op->as_blt.pc, labels, pos))) ? in : NULL;
}

static const char *parse_call(struct lg_vm *vm,
			      struct lg_op *op,
			      const char *in,
			      struct lg_bset *labels,
			      struct lg_pos *pos) {
  return parse_label(vm, skipws(in, pos), &op->as_call.pc, labels, pos);
}

static const char* parse_cp(struct lg_vm *vm,
			    struct lg_op *op,
			    const char *in,
			    struct lg_bset *labels,
			    struct lg_pos *pos) {  
  return parse_size(in, &op->as_cp.i, pos);
}

static const char* parse_dec(struct lg_vm *vm,
			    struct lg_op *op,
			    const char *in,
			    struct lg_bset *labels,
			    struct lg_pos *pos) {  
  return parse_size(in, &op->as_dec.i, pos);
}

static const char* parse_drop(struct lg_vm *vm,
			    struct lg_op *op,
			    const char *in,
			    struct lg_bset *labels,
			    struct lg_pos *pos) {  
  return
    (in = parse_size(in, &op->as_drop.i, pos)) &&
    (in = parse_size(skipws(in, pos), &op->as_drop.n, pos)) ? in : NULL;
}

static const char *parse_jmp(struct lg_vm *vm,
			     struct lg_op *op,
			     const char *in,
			     struct lg_bset *labels,
			     struct lg_pos *pos) {
  return parse_label(vm, skipws(in, pos), &op->as_jmp.pc, labels, pos);
}

static const char* parse_push(struct lg_vm *vm,
			    struct lg_op *op,
			    const char *in,
			    struct lg_bset *labels,
			    struct lg_pos *pos) {  
  lg_val_init(&op->as_push.val, &lg_int_type);
  return parse_int(in, &op->as_push.val.as_int, pos);
}

static bool parse_labels(struct lg_vm *vm, struct lg_bset *labels, const char *in) {
  size_t pc = 0;
  
  while (*in) {
    while (isspace(*in)) {
      in++;
    }
    
    const char *start = in;
  
    while (*in && !isspace(*in)) {
      in++;
    }
    
    const char *end = in;
    size_t len = end - start;
    
    if (end > start) {
      if (*(end-1) == ':') {
	char id[LG_LABEL_MAX] = {0};
	strncpy(id, start, len-1);
	struct label *l = lg_bset_add(labels, id);
	
	if (!l) {
	  lg_error(vm, "Duplicate label: %s", id);
	  return false;
	}
	
	strcpy(l->id, id);
	l->pc = pc;
      } else {
	pc++;
      }
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

typedef const char *(*lg_parser_t)(struct lg_vm *vm,
				   struct lg_op *op,
				   const char *in,
				   struct lg_bset *labels,
				   struct lg_pos *pos);


static const char *parse_op(struct lg_vm *vm, 
			    struct lg_bset *labels,
			    const char *in,
			    struct lg_pos *pos) {
  const char *start = in = skipws(in, pos);
  
  while (*in && !isspace(*in)) {
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

  enum lg_opcode code = LG_NOP;

  if (checkid("add", start, len)) {
    code = LG_ADD;
  } else if (checkid("beq", start, len)) {
    code = LG_BEQ;
  } else if (checkid("blt", start, len)) {
    code = LG_BLT;
  } else if (checkid("call", start, len)) {
    code = LG_CALL;
  } else if (checkid("cp", start, len)) {
    code = LG_CP;
  } else if (checkid("dec", start, len)) {
    code = LG_DEC;
  } else if (checkid("drop", start, len)) {
    code = LG_DROP;
  } else if (checkid("jmp", start, len)) {
    code = LG_JMP;
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
  in = skipws(in, pos);
  
  static const lg_parser_t parsers[LG_OP_MAX] =
    {NULL, NULL,
     parse_beq, parse_blt,
     parse_call, parse_cp,
     parse_dec, parse_drop,
     parse_jmp,
     parse_push,
     NULL, NULL, NULL};

  lg_parser_t p = parsers[code];
  return p ? p(vm, op, in, labels, pos) : in;
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
  while (*in && (in = parse_op(vm, &labels, in, &pos)));
  
  lg_bset_deinit(&labels);
  lg_buf_deinit(&buf);
  fclose(f);
  return true;
}
