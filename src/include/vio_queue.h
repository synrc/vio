#ifndef VIO_QUEUE_H
#define VIO_QUEUE_H

#include "vio_types.h"
#include <stdint.h>

/**
 * VirtIO Split Virtqueue Definitions
 */

#define VIO_VRING_DESC_F_NEXT  1
#define VIO_VRING_DESC_F_WRITE 2
#define VIO_VRING_DESC_F_INDIRECT 4

struct vio_vring_desc {
    uint64_t addr;
    uint32_t len;
    uint16_t flags;
    uint16_t next;
};

struct vio_vring_avail {
    uint16_t flags;
    uint16_t idx;
    uint16_t ring[];
    /* uint16_t used_event; follows ring array */
};

struct vio_vring_used_elem {
    uint32_t id;
    uint32_t len;
};

struct vio_vring_used {
    uint16_t flags;
    uint16_t idx;
    struct vio_vring_used_elem ring[];
    /* uint16_t avail_event; follows ring array */
};

struct vio_virtqueue {
    uint16_t queue_size;
    uint16_t last_avail_idx;
    uint16_t last_used_idx;
    struct vio_vring_desc *desc;
    struct vio_vring_avail *avail;
    struct vio_vring_used *used;
};

/**
 * Initialize a virtqueue structure from a shared memory region.
 */
vio_status_t vio_queue_init(struct vio_virtqueue *vq, void *mem, uint16_t size);

/**
 * Check if there are used elements to process.
 */
int vio_queue_has_used(const struct vio_virtqueue *vq);

#endif /* VIO_QUEUE_H */
