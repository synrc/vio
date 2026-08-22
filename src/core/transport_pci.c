#include "vio_transport.h"
#include "../util/log.h"

/* Basic PCI transport skeleton. 
 * Real implementation will map PCI bars and offset calculations.
 */

static uint32_t pci_read32(struct vio_device *dev, uint32_t offset) {
    (void)dev; (void)offset;
    return 0;
}

static void pci_write32(struct vio_device *dev, uint32_t offset, uint32_t val) {
    (void)dev; (void)offset; (void)val;
}

static uint8_t pci_read8(struct vio_device *dev, uint32_t offset) {
    (void)dev; (void)offset;
    return 0;
}

static void pci_write8(struct vio_device *dev, uint32_t offset, uint8_t val) {
    (void)dev; (void)offset; (void)val;
}

static vio_status_t pci_negotiate(struct vio_device *dev, uint64_t driver_features) {
    (void)dev; (void)driver_features;
    return VIO_OK;
}

static vio_status_t pci_setup_queue(struct vio_device *dev, uint16_t queue_index, void *mem) {
    (void)dev; (void)queue_index; (void)mem;
    return VIO_OK;
}

static void pci_notify_queue(struct vio_device *dev, uint16_t queue_index) {
    (void)dev; (void)queue_index;
}

const struct vio_transport_ops vio_pci_ops = {
    .read32 = pci_read32,
    .write32 = pci_write32,
    .read8 = pci_read8,
    .write8 = pci_write8,
    .negotiate_features = pci_negotiate,
    .setup_queue = pci_setup_queue,
    .notify_queue = pci_notify_queue
};
