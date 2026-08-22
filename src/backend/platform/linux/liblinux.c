#include "../../../include/vio_platform.h"
#include <stddef.h>

/**
 * Linux Platform Backend (Stub)
 * Intended to use VFIO for userspace PCIe NVMe access, or vhost-user
 * for virtual environments (Alpine / Chimera).
 */

static void *linux_map_memory(uint64_t phys_addr, uint32_t len) {
    (void)phys_addr; (void)len;
    /* Typically mapped via /dev/vfio or mmap(hugepages) */
    return NULL;
}

static void linux_unmap_memory(void *virt_addr, uint32_t len) {
    (void)virt_addr; (void)len;
}

static vio_status_t linux_register_irq(uint32_t irq, void (*handler)(void *ctx), void *ctx) {
    (void)irq; (void)handler; (void)ctx;
    /* Typically wired via eventfd */
    return VIO_ERR_UNSUPPORTED;
}

static void linux_mask_irq(uint32_t irq) {
    (void)irq;
}

static void linux_unmask_irq(uint32_t irq) {
    (void)irq;
}

const struct vio_platform_ops vio_linux_platform = {
    .map_memory = linux_map_memory,
    .unmap_memory = linux_unmap_memory,
    .register_irq = linux_register_irq,
    .mask_irq = linux_mask_irq,
    .unmask_irq = linux_unmask_irq
};
