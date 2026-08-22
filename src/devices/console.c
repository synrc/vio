#include "vio_console.h"

struct vio_console_device {
    struct vio_device *base;
};

static struct vio_console_device g_console_dev;

vio_status_t vio_console_init(struct vio_console_device **out_dev, struct vio_device *base_dev) {
    if (!out_dev || !base_dev) return VIO_ERR_INVAL;
    g_console_dev.base = base_dev;
    *out_dev = &g_console_dev;
    return VIO_OK;
}

vio_status_t vio_console_write(struct vio_console_device *dev, const char *buf, uint32_t len) {
    (void)dev; (void)buf; (void)len;
    return VIO_ERR_UNSUPPORTED;
}

vio_status_t vio_console_read(struct vio_console_device *dev, char *buf, uint32_t max_len, uint32_t *out_len) {
    (void)dev; (void)buf; (void)max_len; (void)out_len;
    return VIO_ERR_UNSUPPORTED;
}
