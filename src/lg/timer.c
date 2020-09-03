#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "lg/error.h"
#include "lg/timer.h"

void lg_timer_init(struct lg_timer *_) {
  if (!timespec_get(&_->start, TIME_UTC)) {
    fprintf(stderr, "Failed getting time: %d", errno);
    abort();
  }
}

uint64_t lg_timer_nsecs(const struct lg_timer *_) {
  struct timespec end;

  if (!timespec_get(&end, TIME_UTC)) {
    fprintf(stderr, "Failed getting time: %d", errno);
    abort();
  }

  return
    (end.tv_sec - _->start.tv_sec) * 1000000000 +
    (end.tv_nsec - _->start.tv_nsec);
}

uint64_t lg_timer_usecs(const struct lg_timer *_) {
  return lg_timer_nsecs(_) / 1000;
}
