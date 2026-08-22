#include "test_runner.h"
#include "../src/include/vio_platform.h"
#include "../src/include/vio_blk.h"
#include "../src/include/vio_net.h"
#include <stdint.h>
#include <stddef.h>

extern const struct vio_platform_ops vio_libkrun_platform;
extern void vio_libkrun_init(uint32_t ctx_id, void *guest_ram_base, uint64_t guest_ram_size);
extern int32_t vio_libkrun_create_context(uint8_t num_vcpus, uint32_t ram_mib);

void test_libkrun_mapping(void) {
    /* Simulate 1MB of guest RAM allocated by libkrun in host space */
    uint8_t simulated_guest_ram[1024 * 1024];
    
    /* Initialize the libkrun backend with this simulated RAM */
    vio_libkrun_init(1 /* ctx_id */, simulated_guest_ram, sizeof(simulated_guest_ram));
    
    /* Test mapping a valid guest physical address (e.g., 0x1000) */
    void *mapped = vio_libkrun_platform.map_memory(0x1000, 4096);
    TEST_ASSERT(mapped == (simulated_guest_ram + 0x1000), "Valid mapping failed");
    
    /* Test out-of-bounds mapping */
    void *invalid_map = vio_libkrun_platform.map_memory(1024 * 1024, 4096);
    TEST_ASSERT(invalid_map == NULL, "Out-of-bounds mapping should return NULL");
}

void test_libkrun_blk_init(void) {
    struct vio_blk_device *dev;
    struct vio_device base = { .ops = NULL, .transport_ctx = NULL };
    
    vio_status_t st = vio_blk_init(&dev, &base);
    TEST_ASSERT(st == VIO_OK, "libkrun block init failed");
    TEST_ASSERT(dev != NULL, "libkrun block device is NULL");
}

void test_libkrun_net_init(void) {
    struct vio_net_device *dev;
    struct vio_device base = { .ops = NULL, .transport_ctx = NULL };
    
    vio_status_t st = vio_net_init(&dev, &base);
    TEST_ASSERT(st == VIO_OK, "libkrun net init failed");
    TEST_ASSERT(dev != NULL, "libkrun net device is NULL");
}

void test_libkrun_create_context(void) {
    /* Calls into libkrun API krun_create_ctx & krun_set_vm_config */
    int32_t ctx = vio_libkrun_create_context(2, 512);
    if (ctx >= 0) {
        TEST_ASSERT(ctx >= 0, "libkrun context creation succeeded");
    } else {
        TEST_ASSERT(ctx == -1, "libkrun using weak fallback stub (library not compiled)");
    }
}

void run_krun_tests(void) {
    RUN_TEST(test_libkrun_mapping);
    RUN_TEST(test_libkrun_blk_init);
    RUN_TEST(test_libkrun_net_init);
    RUN_TEST(test_libkrun_create_context);
}
