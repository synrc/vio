#ifndef VIO_CONSOLE_H
#define VIO_CONSOLE_H

#include "vio_types.h"
#include "vio_transport.h"

/**
 * Console Device API.
 */

struct vio_console_device;

vio_status_t vio_console_init(struct vio_console_device **out_dev, struct vio_device *base_dev);
vio_status_t vio_console_write(struct vio_console_device *dev, const char *buf, uint32_t len);
vio_status_t vio_console_read(struct vio_console_device *dev, char *buf, uint32_t max_len, uint32_t *out_len);

#endif /* VIO_CONSOLE_H */
