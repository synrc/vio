#ifndef VIO_BLK_H
#define VIO_BLK_H

#include "vio_types.h"
#include "vio_transport.h"

/**
 * Block Device API.
 * Focused on modern PCIe/NVMe backend semantics.
 */

struct vio_blk_device;

vio_status_t vio_blk_init(struct vio_blk_device **out_dev, struct vio_device *base_dev);
vio_status_t vio_blk_read(struct vio_blk_device *dev, uint64_t sector, void *buf, uint32_t count);
vio_status_t vio_blk_write(struct vio_blk_device *dev, uint64_t sector, const void *buf, uint32_t count);

#endif /* VIO_BLK_H */
