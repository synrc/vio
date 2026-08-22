#ifndef VIO_TRANSPORT_H
#define VIO_TRANSPORT_H

#include "vio_types.h"
#include <stdint.h>

/**
 * Transport abstraction (MMIO / PCI).
 */

struct vio_device;

struct vio_transport_ops {
    uint32_t (*read32)(struct vio_device *dev, uint32_t offset);
    void (*write32)(struct vio_device *dev, uint32_t offset, uint32_t val);
    uint8_t (*read8)(struct vio_device *dev, uint32_t offset);
    void (*write8)(struct vio_device *dev, uint32_t offset, uint8_t val);
    
    vio_status_t (*negotiate_features)(struct vio_device *dev, uint64_t driver_features);
    vio_status_t (*setup_queue)(struct vio_device *dev, uint16_t queue_index, void *mem);
    void (*notify_queue)(struct vio_device *dev, uint16_t queue_index);
};

struct vio_device {
    const struct vio_transport_ops *ops;
    void *transport_ctx;
    uint32_t device_id;
    uint32_t vendor_id;
    uint64_t device_features;
    uint64_t driver_features;
};

#endif /* VIO_TRANSPORT_H */
