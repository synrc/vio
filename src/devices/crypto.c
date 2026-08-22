#include "vio_crypto.h"

struct vio_crypto_device {
    struct vio_device *base;
};

static struct vio_crypto_device g_crypto_dev;

vio_status_t vio_crypto_init(struct vio_crypto_device **out_dev, struct vio_device *base_dev) {
    if (!out_dev || !base_dev) return VIO_ERR_INVAL;
    g_crypto_dev.base = base_dev;
    *out_dev = &g_crypto_dev;
    return VIO_OK;
}

vio_status_t vio_crypto_encrypt(struct vio_crypto_device *dev, const void *in, void *out, uint32_t len) {
    (void)dev; (void)in; (void)out; (void)len;
    return VIO_ERR_UNSUPPORTED;
}

vio_status_t vio_crypto_decrypt(struct vio_crypto_device *dev, const void *in, void *out, uint32_t len) {
    (void)dev; (void)in; (void)out; (void)len;
    return VIO_ERR_UNSUPPORTED;
}
