#include "../../../include/vio_platform.h"
#include <stddef.h>

/**
 * Windows Hyper-V Platform Backend (Stub)
 * Intended to use Windows VMBus and Hyper-V isolation APIs.
 */

static void *hyperv_map_memory(uint64_t phys_addr, uint32_t len) {
    (void)phys_addr; (void)len;
    /* Map via Hyper-V specific APIs */
    return NULL;
}

static void hyperv_unmap_memory(void *virt_addr, uint32_t len) {
    (void)virt_addr; (void)len;
}

static vio_status_t hyperv_register_irq(uint32_t irq, void (*handler)(void *ctx), void *ctx) {
    (void)irq; (void)handler; (void)ctx;
    return VIO_ERR_UNSUPPORTED;
}

static void hyperv_mask_irq(uint32_t irq) {
    (void)irq;
}

static void hyperv_unmask_irq(uint32_t irq) {
    (void)irq;
}

const struct vio_platform_ops vio_hyperv_platform = {
    .map_memory = hyperv_map_memory,
    .unmap_memory = hyperv_unmap_memory,
    .register_irq = hyperv_register_irq,
    .mask_irq = hyperv_mask_irq,
    .unmask_irq = hyperv_unmask_irq
};
