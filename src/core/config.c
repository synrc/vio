#include "vio_transport.h"

/* Device config space helpers.
 * Handles parsing config space mapped by the transport.
 */

vio_status_t vio_config_read(struct vio_device *dev, uint32_t offset, void *buf, uint32_t len) {
    if (!dev || !dev->ops || !buf) return VIO_ERR_INVAL;

    uint8_t *b = (uint8_t *)buf;
    for (uint32_t i = 0; i < len; i++) {
        b[i] = dev->ops->read8(dev, offset + i);
    }

    return VIO_OK;
}

vio_status_t vio_config_write(struct vio_device *dev, uint32_t offset, const void *buf, uint32_t len) {
    if (!dev || !dev->ops || !buf) return VIO_ERR_INVAL;

    const uint8_t *b = (const uint8_t *)buf;
    for (uint32_t i = 0; i < len; i++) {
        dev->ops->write8(dev, offset + i, b[i]);
    }

    return VIO_OK;
}
