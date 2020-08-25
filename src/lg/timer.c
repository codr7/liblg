#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "lg/error.h"
#include "lg/timer.h"

void lg_timer_init(struct lg_timer *timer) {
  if (!timespec_get(&timer->start, TIME_UTC)) {
    fprintf(stderr, "Failed getting time: %d", errno);
    abort();
  }
}

uint64_t lg_timer_nsecs(const struct lg_timer *timer) {
  struct timespec end;

  if (!timespec_get(&end, TIME_UTC)) {
    fprintf(stderr, "Failed getting time: %d", errno);
    abort();
  }

  return
    (end.tv_sec - timer->start.tv_sec) * 1000000000 +
    (end.tv_nsec - timer->start.tv_nsec);
}

uint64_t lg_timer_usecs(const struct lg_timer *timer) {
  return lg_timer_nsecs(timer) / 1000;
}
