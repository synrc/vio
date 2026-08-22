#include "vio_transport.h"
#include "../util/log.h"

/* Basic MMIO transport skeleton */

static uint32_t mmio_read32(struct vio_device *dev, uint32_t offset) {
    (void)dev; (void)offset;
    return 0;
}

static void mmio_write32(struct vio_device *dev, uint32_t offset, uint32_t val) {
    (void)dev; (void)offset; (void)val;
}

static uint8_t mmio_read8(struct vio_device *dev, uint32_t offset) {
    (void)dev; (void)offset;
    return 0;
}

static void mmio_write8(struct vio_device *dev, uint32_t offset, uint8_t val) {
    (void)dev; (void)offset; (void)val;
}

static vio_status_t mmio_negotiate(struct vio_device *dev, uint64_t driver_features) {
    (void)dev; (void)driver_features;
    return VIO_OK;
}

static vio_status_t mmio_setup_queue(struct vio_device *dev, uint16_t queue_index, void *mem) {
    (void)dev; (void)queue_index; (void)mem;
    return VIO_OK;
}

static void mmio_notify_queue(struct vio_device *dev, uint16_t queue_index) {
    (void)dev; (void)queue_index;
}

const struct vio_transport_ops vio_mmio_ops = {
    .read32 = mmio_read32,
    .write32 = mmio_write32,
    .read8 = mmio_read8,
    .write8 = mmio_write8,
    .negotiate_features = mmio_negotiate,
    .setup_queue = mmio_setup_queue,
    .notify_queue = mmio_notify_queue
};
