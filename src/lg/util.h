#ifndef LG_UTILS_H
#define LG_UTILS_H

#include <stddef.h>

#define LG_ALIGN(base, size) ({						\
      __auto_type _base = base;						\
      __auto_type _size = LG_MIN((size), _Alignof(max_align_t));	\
      (_base) + _size - ((ptrdiff_t)(_base)) % _size;			\
    })									\

#define LG_BASEOF(p, t, m) ({			\
      uint8_t *_p = (uint8_t *)p;		\
      _p ? ((t *)(_p - offsetof(t, m))) : NULL;	\
    })

#define _LG_ID(x, y)				\
  x##y

#define LG_ID(x, y)				\
  _LG_ID(x, y)

#define LG_MAX(x, y) ({				\
      __auto_type _x = x;			\
      __auto_type _y = y;			\
      _x > _y ? _x : _y;			\
    })						\

#define LG_MIN(x, y) ({				\
      __auto_type _x = x;			\
      __auto_type _y = y;			\
      _x < _y ? _x : _y;			\
    })						\

#define LG_UNIQUE(x)				\
  LG_ID(x, __COUNTER__)

char *lg_strdup(const char *in, size_t *out_len);

#endif
