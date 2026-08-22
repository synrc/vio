#include "vio_net.h"

struct vio_net_device {
    struct vio_device *base;
};

static struct vio_net_device g_net_dev;

vio_status_t vio_net_init(struct vio_net_device **out_dev, struct vio_device *base_dev) {
    if (!out_dev || !base_dev) return VIO_ERR_INVAL;
    g_net_dev.base = base_dev;
    *out_dev = &g_net_dev;
    return VIO_OK;
}

vio_status_t vio_net_tx(struct vio_net_device *dev, const void *pkt, uint32_t len) {
    (void)dev; (void)pkt; (void)len;
    return VIO_ERR_UNSUPPORTED;
}

vio_status_t vio_net_rx(struct vio_net_device *dev, void *buf, uint32_t *len) {
    (void)dev; (void)buf; (void)len;
    return VIO_ERR_UNSUPPORTED;
}
