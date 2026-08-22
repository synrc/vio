#include "vio_transport.h"

/* Feature negotiation utilities.
 * Handles checking driver vs device features.
 */

#define VIRTIO_F_VERSION_1 32
#define VIRTIO_F_RING_EVENT_IDX 29

vio_status_t vio_feature_negotiate(struct vio_device *dev) {
    if (!dev || !dev->ops) return VIO_ERR_INVAL;

    /* Mask requested driver features with what the device actually supports */
    uint64_t negotiated = dev->driver_features & dev->device_features;
    
    /* VIRTIO 1.0 is generally strictly required for modern devices */
    if (!(negotiated & (1ULL << VIRTIO_F_VERSION_1))) {
        /* If device doesn't support V1, we may want to fallback or fail */
        /* For this skeleton, we assume V1 is mandatory */
        return VIO_ERR_UNSUPPORTED;
    }

    return dev->ops->negotiate_features(dev, negotiated);
}
