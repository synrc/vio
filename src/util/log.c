#include "log.h"
#include <stdarg.h>
#include <stdio.h>

/* Currently using stdio for log output. 
 * In a real unikernel/freestanding build, this would wire to a serial port 
 * or log buffer via a platform abstraction.
 */

void vio_log(vio_log_level_t level, const char *fmt, ...) {
    /* Basic stub implementation */
    (void)level;
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
}
