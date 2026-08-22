#include "vio_bus.h"
#include <stddef.h>

/* Minimal Bus / Discovery implementation */

vio_status_t vio_bus_probe(void) {
    /* For a true implementation, we would scan PCI bus or parse minimal DT */
    return VIO_OK;
}

struct vio_device *vio_bus_find_device(uint32_t device_id) {
    (void)device_id;
    return NULL;
}
