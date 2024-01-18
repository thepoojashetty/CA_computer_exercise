#ifndef BARRIER_H
#define BARRIER_H

#include <stdatomic.h>

void sync_barrier(uint32_t thread_count);

#endif
