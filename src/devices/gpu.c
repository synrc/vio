#include "vio_gpu.h"
#include <string.h>

/* VirtIO-GPU frontend implementation for B-TRON 3.20 (2026 Edition) */

static struct vio_gpu_device g_gpu_dev;
static uint8_t g_gpu_framebuffer[1920 * 1080 * 4];

vio_status_t vio_gpu_init(struct vio_gpu_device **out_dev, struct vio_device *base_dev) {
    if (!out_dev || !base_dev) return VIO_ERR_INVAL;

    memset(&g_gpu_dev, 0, sizeof(g_gpu_dev));
    g_gpu_dev.base = base_dev;
    g_gpu_dev.blitter.host_window_width = 1024;
    g_gpu_dev.blitter.host_window_height = 768;
    g_gpu_dev.blitter.format = VIRTIO_GPU_FORMAT_B8G8R8A8_UNORM;
    g_gpu_dev.blitter.framebuffer = g_gpu_framebuffer;
    g_gpu_dev.blitter.framebuffer_size = sizeof(g_gpu_framebuffer);
    g_gpu_dev.blitter.dirty = false;

    *out_dev = &g_gpu_dev;
    return VIO_OK;
}

vio_status_t vio_gpu_create_resource_2d(struct vio_gpu_device *dev, uint32_t resource_id, uint32_t format, uint32_t width, uint32_t height) {
    if (!dev || resource_id == 0 || width == 0 || height == 0) return VIO_ERR_INVAL;

    dev->current_resource_id = resource_id;
    dev->blitter.active_resource_id = resource_id;
    dev->blitter.format = format;
    dev->blitter.host_window_width = width;
    dev->blitter.host_window_height = height;

    return VIO_OK;
}

vio_status_t vio_gpu_transfer_to_host_2d(struct vio_gpu_device *dev, uint32_t resource_id, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint64_t offset) {
    if (!dev || resource_id == 0) return VIO_ERR_INVAL;
    (void)x; (void)y; (void)width; (void)height; (void)offset;

    dev->blitter.dirty = true;
    return VIO_OK;
}

vio_status_t vio_gpu_resource_flush(struct vio_gpu_device *dev, uint32_t resource_id, uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    if (!dev || resource_id == 0) return VIO_ERR_INVAL;
    (void)x; (void)y; (void)width; (void)height;

    dev->blitter.dirty = false;
    return VIO_OK;
}

vio_status_t vio_gpu_set_scanout(struct vio_gpu_device *dev, uint32_t scanout_id, uint32_t resource_id, uint32_t width, uint32_t height) {
    if (!dev) return VIO_ERR_INVAL;

    dev->active_scanout = scanout_id;
    dev->current_resource_id = resource_id;
    dev->blitter.host_window_width = width;
    dev->blitter.host_window_height = height;

    return VIO_OK;
}

vio_status_t vio_gpu_sdl2_blit_frame(struct vio_gpu_device *dev, const void *src_buf, size_t buf_size) {
    if (!dev || !src_buf || buf_size == 0) return VIO_ERR_INVAL;
    if (buf_size > dev->blitter.framebuffer_size) return VIO_ERR_NOMEM;

    memcpy(dev->blitter.framebuffer, src_buf, buf_size);
    dev->blitter.dirty = true;
    return VIO_OK;
}
