#include "vio_platform.h"
#include "../util/log.h"
#include <stddef.h>

/* Null test backend implementation */

static void *null_map_memory(uint64_t phys_addr, uint32_t len) {
    (void)phys_addr; (void)len;
    return NULL;
}

static void null_unmap_memory(void *virt_addr, uint32_t len) {
    (void)virt_addr; (void)len;
}

static vio_status_t null_register_irq(uint32_t irq, void (*handler)(void *), void *ctx) {
    (void)irq; (void)handler; (void)ctx;
    return VIO_OK;
}

static void null_mask_irq(uint32_t irq) {
    (void)irq;
}

static void null_unmask_irq(uint32_t irq) {
    (void)irq;
}

const struct vio_platform_ops vio_null_platform = {
    .map_memory = null_map_memory,
    .unmap_memory = null_unmap_memory,
    .register_irq = null_register_irq,
    .mask_irq = null_mask_irq,
    .unmask_irq = null_unmask_irq
};

void vio_platform_register(const struct vio_platform_ops *ops) {
    (void)ops;
    vio_log(VIO_LOG_INFO, "Registered platform operations");
}
