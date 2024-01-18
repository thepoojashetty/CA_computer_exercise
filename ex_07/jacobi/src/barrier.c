#define _POSIX_C_SOURCE 199309L
#include <stdint.h>
#include <stdbool.h>
#include "barrier.h"

static volatile atomic_uint_fast32_t __barrier = 0u;
static volatile atomic_bool          __toggle  = false;

void sync_barrier(uint32_t thread_count) {
	uint32_t b;
	bool     t;

	t = atomic_load(&__toggle);
	b = atomic_fetch_add(&__barrier, 1u);
	if(b == thread_count - 1u) {
		atomic_store(&__barrier, 0u);
		atomic_store(&__toggle, !t);
	}
	else {
		while(atomic_load(&__toggle) == t);
	}
	return;
}
