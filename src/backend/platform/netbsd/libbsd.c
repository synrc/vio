#include "../../../include/vio_platform.h"
#include <stddef.h>

/**
 * NetBSD Platform Backend (Stub)
 * Intended to use /dev/pci and standard BSD mmap character devices.
 */

static void *netbsd_map_memory(uint64_t phys_addr, uint32_t len) {
    (void)phys_addr; (void)len;
    /* Mapped via /dev/pci0 using PCIIOCREAD */
    return NULL;
}

static void netbsd_unmap_memory(void *virt_addr, uint32_t len) {
    (void)virt_addr; (void)len;
}

static vio_status_t netbsd_register_irq(uint32_t irq, void (*handler)(void *ctx), void *ctx) {
    (void)irq; (void)handler; (void)ctx;
    /* Wired via kqueue or PCI ioctl */
    return VIO_ERR_UNSUPPORTED;
}

static void netbsd_mask_irq(uint32_t irq) {
    (void)irq;
}

static void netbsd_unmask_irq(uint32_t irq) {
    (void)irq;
}

const struct vio_platform_ops vio_netbsd_platform = {
    .map_memory = netbsd_map_memory,
    .unmap_memory = netbsd_unmap_memory,
    .register_irq = netbsd_register_irq,
    .mask_irq = netbsd_mask_irq,
    .unmask_irq = netbsd_unmask_irq
};
