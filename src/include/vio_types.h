#ifndef VIO_TYPES_H
#define VIO_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * Standard status codes for libvio.
 * Explicit error codes are used instead of errno.
 */
typedef enum {
    VIO_OK = 0,
    VIO_ERR_INVAL = -1,
    VIO_ERR_NOMEM = -2,
    VIO_ERR_BUSY = -3,
    VIO_ERR_TIMEOUT = -4,
    VIO_ERR_UNSUPPORTED = -5,
    VIO_ERR_IO = -6
} vio_status_t;

/**
 * Priority levels for real-time queueing.
 */
typedef enum {
    VIO_PRIO_LOW = 0,
    VIO_PRIO_NORMAL = 1,
    VIO_PRIO_HIGH = 2,
    VIO_PRIO_REALTIME = 3
} vio_prio_t;

#endif /* VIO_TYPES_H */
