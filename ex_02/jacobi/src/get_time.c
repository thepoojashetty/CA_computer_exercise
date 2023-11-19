#define _POSIX_C_SOURCE 199309L
#include <time.h>
#include <sys/time.h>
#include "get_time.h"

uint64_t get_time_us(void) {
    struct timespec a;
    clock_gettime(CLOCK_MONOTONIC, &a);
    return (uint64_t) (((double) a.tv_nsec / 1000.0) + ((double) a.tv_sec * (1000.0 * 1000.0)));
}

