#ifndef VIO_PLATFORM_H
#define VIO_PLATFORM_H

#include "vio_types.h"
#include <stdint.h>

/**
 * Control-plane / Platform abstraction.
 * This interface must be implemented by the host environment (seL4, Hyper-V, libkrun, Linux).
 */

struct vio_platform_ops {
    /* Memory translation (e.g. Guest Physical to Host Virtual) */
    void *(*map_memory)(uint64_t phys_addr, uint32_t len);
    void (*unmap_memory)(void *virt_addr, uint32_t len);

    /* Interrupt routing */
    vio_status_t (*register_irq)(uint32_t irq, void (*handler)(void *ctx), void *ctx);
    void (*mask_irq)(uint32_t irq);
    void (*unmask_irq)(uint32_t irq);
};

void vio_platform_register(const struct vio_platform_ops *ops);

#endif /* VIO_PLATFORM_H */
