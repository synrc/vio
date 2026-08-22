#include "vio_blk.h"

/* Block device frontend implementation */

struct vio_blk_device {
    struct vio_device *base;
};

/* Static instance for skeleton since we don't allocate on hot path */
static struct vio_blk_device g_blk_dev;

vio_status_t vio_blk_init(struct vio_blk_device **out_dev, struct vio_device *base_dev) {
    if (!out_dev || !base_dev) return VIO_ERR_INVAL;
    g_blk_dev.base = base_dev;
    *out_dev = &g_blk_dev;
    return VIO_OK;
}

vio_status_t vio_blk_read(struct vio_blk_device *dev, uint64_t sector, void *buf, uint32_t count) {
    (void)dev; (void)sector; (void)buf; (void)count;
    return VIO_ERR_UNSUPPORTED;
}

vio_status_t vio_blk_write(struct vio_blk_device *dev, uint64_t sector, const void *buf, uint32_t count) {
    (void)dev; (void)sector; (void)buf; (void)count;
    return VIO_ERR_UNSUPPORTED;
}
