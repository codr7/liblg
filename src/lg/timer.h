#ifndef LG_TIMER_H
#define LG_TIMER_H

#include <stdint.h>
#include <time.h>

struct lg_timer {
  struct timespec start;
};

void lg_timer_init(struct lg_timer *_);
uint64_t lg_timer_nsecs(const struct lg_timer *_);
uint64_t lg_timer_usecs(const struct lg_timer *_);

#endif
