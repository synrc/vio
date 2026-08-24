#ifndef VIO_GPU_H
#define VIO_GPU_H

#include "vio_types.h"
#include "vio_transport.h"

/**
 * VirtIO-GPU 2D Command Definitions (VirtIO Spec v1.2)
 */
#define VIRTIO_GPU_CMD_GET_DISPLAY_INFO        0x0100
#define VIRTIO_GPU_CMD_RESOURCE_CREATE_2D      0x0101
#define VIRTIO_GPU_CMD_RESOURCE_UNREF          0x0102
#define VIRTIO_GPU_CMD_SET_SCANOUT             0x0103
#define VIRTIO_GPU_CMD_RESOURCE_FLUSH          0x0104
#define VIRTIO_GPU_CMD_TRANSFER_TO_HOST_2D     0x0105
#define VIRTIO_GPU_CMD_RESOURCE_ATTACH_BACKING 0x0106
#define VIRTIO_GPU_CMD_RESOURCE_DETACH_BACKING 0x0107

#define VIRTIO_GPU_RESP_OK_NODATA              0x1100
#define VIRTIO_GPU_RESP_OK_DISPLAY_INFO        0x1101
#define VIRTIO_GPU_RESP_ERR_UNSPEC             0x1200

#define VIRTIO_GPU_FORMAT_B8G8R8A8_UNORM       1
#define VIRTIO_GPU_FORMAT_B8G8R8X8_UNORM       2
#define VIRTIO_GPU_FORMAT_A8R8G8B8_UNORM       3
#define VIRTIO_GPU_FORMAT_X8R8G8B8_UNORM       4
#define VIRTIO_GPU_FORMAT_R8G8B8A8_UNORM       5
#define VIRTIO_GPU_FORMAT_R8G8B8X8_UNORM       6

struct virtio_gpu_ctrl_hdr {
    uint32_t type;
    uint32_t flags;
    uint64_t fence_id;
    uint32_t ctx_id;
    uint32_t padding;
};

struct virtio_gpu_rect {
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
};

struct virtio_gpu_resource_create_2d {
    struct virtio_gpu_ctrl_hdr hdr;
    uint32_t resource_id;
    uint32_t format;
    uint32_t width;
    uint32_t height;
};

struct virtio_gpu_transfer_to_host_2d {
    struct virtio_gpu_ctrl_hdr hdr;
    struct virtio_gpu_rect r;
    uint64_t offset;
    uint32_t resource_id;
    uint32_t padding;
};

struct virtio_gpu_resource_flush {
    struct virtio_gpu_ctrl_hdr hdr;
    struct virtio_gpu_rect r;
    uint32_t resource_id;
    uint32_t padding;
};

struct virtio_gpu_set_scanout {
    struct virtio_gpu_ctrl_hdr hdr;
    struct virtio_gpu_rect r;
    uint32_t scanout_id;
    uint32_t resource_id;
};

/**
 * Host SDL2 / Display Blitter state representation for B-TRON VirtIO-GPU
 */
struct vio_gpu_sdl2_blitter {
    uint32_t host_window_width;
    uint32_t host_window_height;
    uint32_t active_resource_id;
    uint32_t format;
    void *framebuffer;
    size_t framebuffer_size;
    bool dirty;
};

struct vio_gpu_device {
    struct vio_device *base;
    struct vio_gpu_sdl2_blitter blitter;
    uint32_t active_scanout;
    uint32_t current_resource_id;
};

/* VirtIO-GPU Device Control APIs */
vio_status_t vio_gpu_init(struct vio_gpu_device **out_dev, struct vio_device *base_dev);
vio_status_t vio_gpu_create_resource_2d(struct vio_gpu_device *dev, uint32_t resource_id, uint32_t format, uint32_t width, uint32_t height);
vio_status_t vio_gpu_transfer_to_host_2d(struct vio_gpu_device *dev, uint32_t resource_id, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint64_t offset);
vio_status_t vio_gpu_resource_flush(struct vio_gpu_device *dev, uint32_t resource_id, uint32_t x, uint32_t y, uint32_t width, uint32_t height);
vio_status_t vio_gpu_set_scanout(struct vio_gpu_device *dev, uint32_t scanout_id, uint32_t resource_id, uint32_t width, uint32_t height);
vio_status_t vio_gpu_sdl2_blit_frame(struct vio_gpu_device *dev, const void *src_buf, size_t buf_size);

#endif /* VIO_GPU_H */
