#ifndef VIO_BUS_H
#define VIO_BUS_H

#include "vio_types.h"
#include "vio_transport.h"

/**
 * Bus / Discovery API (minimal device tree support).
 */

vio_status_t vio_bus_probe(void);
struct vio_device *vio_bus_find_device(uint32_t device_id);

#endif /* VIO_BUS_H */
