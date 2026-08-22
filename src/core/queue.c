#include "vio_queue.h"
#include "../util/mem.h"

vio_status_t vio_queue_init(struct vio_virtqueue *vq, void *mem, uint16_t size) {
    if (!vq || !mem || size == 0 || (size & (size - 1)) != 0) {
        return VIO_ERR_INVAL; /* Size must be power of 2 */
    }

    vq->queue_size = size;
    vq->last_avail_idx = 0;
    vq->last_used_idx = 0;

    /* Basic layout: desc, then avail, then used. 
       In real virtio, alignment rules apply (usually 4096 page boundary for used ring),
       but for this minimal skeleton we just map them sequentially for testing. 
       Actual implementation will need exact standard layout math. */
       
    uint8_t *p = (uint8_t *)mem;
    vq->desc = (struct vio_vring_desc *)p;
    p += size * sizeof(struct vio_vring_desc);
    
    vq->avail = (struct vio_vring_avail *)p;
    p += sizeof(struct vio_vring_avail) + size * sizeof(uint16_t) + sizeof(uint16_t); /* + avail_event */
    
    /* Simulate page alignment for used ring (4096) */
    uintptr_t u = (uintptr_t)p;
    u = (u + 4095) & ~4095;
    vq->used = (struct vio_vring_used *)u;

    return VIO_OK;
}

int vio_queue_has_used(const struct vio_virtqueue *vq) {
    if (!vq || !vq->used) return 0;
    /* In actual implementation we'd read vq->used->idx with an acquire barrier. */
    /* For TRON style, we assume this is called on a polling hot-path. */
    return vq->last_used_idx != vq->used->idx;
}
