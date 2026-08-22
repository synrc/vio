#ifndef UTIL_MEM_H
#define UTIL_MEM_H

#include <stddef.h>

/**
 * Minimal memory operations.
 * By design, no allocation on hot paths.
 */

void *vio_memset(void *s, int c, size_t n);
void *vio_memcpy(void *dest, const void *src, size_t n);
int vio_memcmp(const void *s1, const void *s2, size_t n);

#endif /* UTIL_MEM_H */
