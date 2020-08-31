#ifndef LG_CMP_H
#define LG_CMP_H

enum lg_cmp {LG_LT=-1, LG_EQ=0, LG_GT=1};

typedef enum lg_cmp (*lg_cmp_t)(const void *, const void *);

enum lg_cmp lg_strcmp(const char *x, const char *y);

#endif
