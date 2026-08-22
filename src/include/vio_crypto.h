#ifndef VIO_CRYPTO_H
#define VIO_CRYPTO_H

#include "vio_types.h"
#include "vio_transport.h"

/**
 * Crypto Device API.
 */

struct vio_crypto_device;

vio_status_t vio_crypto_init(struct vio_crypto_device **out_dev, struct vio_device *base_dev);
vio_status_t vio_crypto_encrypt(struct vio_crypto_device *dev, const void *in, void *out, uint32_t len);
vio_status_t vio_crypto_decrypt(struct vio_crypto_device *dev, const void *in, void *out, uint32_t len);

#endif /* VIO_CRYPTO_H */
