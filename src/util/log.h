#ifndef UTIL_LOG_H
#define UTIL_LOG_H

/**
 * Minimal logging facility.
 * Compiled out on fast paths if needed.
 */

typedef enum {
    VIO_LOG_DEBUG = 0,
    VIO_LOG_INFO  = 1,
    VIO_LOG_WARN  = 2,
    VIO_LOG_ERROR = 3
} vio_log_level_t;

void vio_log(vio_log_level_t level, const char *fmt, ...);

#endif /* UTIL_LOG_H */
