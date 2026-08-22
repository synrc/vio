#ifndef UTIL_ATOMIC_H
#define UTIL_ATOMIC_H

#include <stdint.h>
#include <stdatomic.h>

/**
 * Wrappers for C11 stdatomic, providing a consistent API and allowing
 * potential fallbacks on primitive platforms.
 */

typedef _Atomic(uint32_t) vio_atomic32_t;
typedef _Atomic(uint64_t) vio_atomic64_t;

static inline uint32_t vio_atomic_load32(const vio_atomic32_t *var) {
    return atomic_load_explicit(var, memory_order_acquire);
}

static inline void vio_atomic_store32(vio_atomic32_t *var, uint32_t val) {
    atomic_store_explicit(var, val, memory_order_release);
}

static inline uint32_t vio_atomic_add32(vio_atomic32_t *var, uint32_t val) {
    return atomic_fetch_add_explicit(var, val, memory_order_acq_rel);
}

#endif /* UTIL_ATOMIC_H */
