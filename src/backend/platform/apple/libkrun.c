#include "../../../include/vio_platform.h"
#include "../../../util/log.h"
#include "../../../../third_party/libkrun/include/libkrun.h"
#include <stddef.h>

/**
 * libkrun Platform Backend
 * Provides memory translation and IRQ routing for libvio running as a backend
 * for a libkrun microVM on macOS.
 */

/* For testing purposes, we assume a single continuous guest RAM region */
static void *g_guest_ram_base = NULL;
static uint64_t g_guest_ram_size = 0;
static uint32_t g_krun_ctx_id = 0;

/* Weak stubs so libvio compiles and passes tests out-of-the-box 
 * even if third_party/libkrun has not been built yet.
 * Real libkrun symbols will override these at link time when available.
 */
__attribute__((weak)) int32_t krun_create_ctx(void) {
    return -1;
}

__attribute__((weak)) int32_t krun_set_vm_config(uint32_t ctx_id, uint8_t num_vcpus, uint32_t ram_mib) {
    (void)ctx_id; (void)num_vcpus; (void)ram_mib;
    return -1;
}

int32_t vio_libkrun_create_context(uint8_t num_vcpus, uint32_t ram_mib) {
    int32_t ctx_id = krun_create_ctx();
    if (ctx_id < 0) {
        /* Return -1 gracefully if weak fallback stub is invoked without real libkrun */
        return ctx_id;
    }
    
    int32_t ret = krun_set_vm_config((uint32_t)ctx_id, num_vcpus, ram_mib);
    if (ret < 0) {
        vio_log(VIO_LOG_ERROR, "Failed to set krun VM config");
        return ret;
    }
    
    g_krun_ctx_id = (uint32_t)ctx_id;
    return ctx_id;
}

void vio_libkrun_init(uint32_t ctx_id, void *guest_ram_base, uint64_t guest_ram_size) {
    g_krun_ctx_id = ctx_id;
    g_guest_ram_base = guest_ram_base;
    g_guest_ram_size = guest_ram_size;
}

static void *libkrun_map_memory(uint64_t phys_addr, uint32_t len) {
    if (!g_guest_ram_base || phys_addr + len > g_guest_ram_size) {
        return NULL;
    }
    /* Guest Physical to Host Virtual translation */
    return (void *)((uint8_t *)g_guest_ram_base + phys_addr);
}

static void libkrun_unmap_memory(void *virt_addr, uint32_t len) {
    /* Mapped globally, no-op per region */
    (void)virt_addr; (void)len;
}

static vio_status_t libkrun_register_irq(uint32_t irq, void (*handler)(void *ctx), void *ctx) {
    /* For host-to-guest injection, libkrun has APIs, 
     * but this function is usually for receiving interrupts from hardware.
     * Since we are the backend, we *generate* interrupts to the VM.
     */
    (void)irq; (void)handler; (void)ctx;
    return VIO_OK;
}

static void libkrun_mask_irq(uint32_t irq) {
    (void)irq;
}

static void libkrun_unmask_irq(uint32_t irq) {
    (void)irq;
}

const struct vio_platform_ops vio_libkrun_platform = {
    .map_memory = libkrun_map_memory,
    .unmap_memory = libkrun_unmap_memory,
    .register_irq = libkrun_register_irq,
    .mask_irq = libkrun_mask_irq,
    .unmask_irq = libkrun_unmask_irq
};
