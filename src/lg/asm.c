#include <ctype.h>
#include <errno.h>
#include <string.h>
  
#include "lg/asm.h"
#include "lg/bset.h"
#include "lg/buf.h"
#include "lg/error.h"
#include "lg/op.h"
#include "lg/pos.h"
#include "lg/target.h"

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
  return strncmp(id, in, LG_MIN(strlen(id), len)) == 0;
}

static bool parse_add(struct lg_op *op, const char *in) {
  return true;
}

static bool parse(struct lg_vm *vm, struct lg_target *tgt, struct lg_bset *labels,
		  const char *in, struct lg_pos *pos) {
  const char *start = in;
  
  while (!isspace(*in)) {
    in++;
    pos->col++;
  }

  const char *end = in;
  size_t len = end - start;

  if (end == start) {
    return true;
  }

  if (*(end-1) == ':') {
    char id[LG_LABEL_MAX] = {0};
    strncpy(id, start, len-1);
    struct label *l = lg_bset_add(labels, id);

    if (!l) {
      lg_error(vm, "Duplicate label: %s", id);
      return false;
    }
    
    strcpy(l->id, id);
    l->pc = tgt->ops.len;
    return true;
  }

  if (checkid("begin", start, len)) {
    return true;
  } else if (checkid("end", start, len)) {
    return true;
  }
  
  enum lg_opcode code;

  if (checkid("add", start, len) == 0) {
    code = LG_ADD;
  } else if (checkid("brint", start, len) == 0) {
    code = LG_BRINT;
  } else if (checkid("call", start, len) == 0) {
    code = LG_CALL;
  } else if (checkid("cp", start, len) == 0) {
    code = LG_CP;
  } else if (checkid("dec", start, len) == 0) {
    code = LG_DEC;
  } else if (checkid("push", start, len) == 0) {
    code = LG_PUSH;
  } else if (checkid("ret", start, len) == 0) {
    code = LG_RET;
  } else if (checkid("stop", start, len) == 0) {
    code = LG_STOP;
  } else if (checkid("swap", start, len) == 0) {
    code = LG_SWAP;
  } else {
    lg_error(vm, "Invalid opcode");
    return false;
  }

  struct lg_op *op = lg_emit(tgt, code);

  switch (code) {
  case LG_ADD:
    return parse_add(op, skipws(in, pos));
  default:
    break;
  }
  
  return true;
}

static const void *label_key(const void *x) {
  return ((const struct label *)x)->id;
}

static enum lg_cmp label_cmp(const void *x, const void *y) {
  return lg_strcmp(x, y);
}

bool lg_asm(struct lg_vm *vm, struct lg_target *tgt, const char *path) {
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
  const char *line = NULL;
  
  while (*(line = lg_getline(&buf, f))) {
    line = skipws(line, &pos);
    
    if (*line != '\n') {
      if (!parse(vm, tgt, &labels, line, &pos)) {
	break;
      }
    }

    pos.row++;
    lg_buf_clear(&buf);
  }

  lg_bset_deinit(&labels);
  lg_buf_deinit(&buf);
  fclose(f);
  return true;
}
