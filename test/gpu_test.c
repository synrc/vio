#include "test_runner.h"
#include "vio_gpu.h"
#include <string.h>

void test_gpu_init(void) {
    struct vio_gpu_device *gpu_dev = NULL;
    struct vio_device base_dev;

    vio_status_t status = vio_gpu_init(&gpu_dev, &base_dev);
    TEST_ASSERT(status == VIO_OK, "vio_gpu_init failed");
    TEST_ASSERT(gpu_dev != NULL, "gpu_dev is NULL");
    TEST_ASSERT(gpu_dev->blitter.host_window_width == 1024, "default width mismatch");
    TEST_ASSERT(gpu_dev->blitter.host_window_height == 768, "default height mismatch");
}

void test_gpu_resource_create_and_scanout(void) {
    struct vio_gpu_device *gpu_dev = NULL;
    struct vio_device base_dev;

    vio_gpu_init(&gpu_dev, &base_dev);
    
    vio_status_t status = vio_gpu_create_resource_2d(gpu_dev, 1, VIRTIO_GPU_FORMAT_B8G8R8A8_UNORM, 1920, 1080);
    TEST_ASSERT(status == VIO_OK, "create_resource_2d failed");
    TEST_ASSERT(gpu_dev->current_resource_id == 1, "resource_id mismatch");
    TEST_ASSERT(gpu_dev->blitter.host_window_width == 1920, "width mismatch");

    status = vio_gpu_set_scanout(gpu_dev, 0, 1, 1920, 1080);
    TEST_ASSERT(status == VIO_OK, "set_scanout failed");
    TEST_ASSERT(gpu_dev->active_scanout == 0, "active_scanout mismatch");
}

void test_gpu_sdl2_blit(void) {
    struct vio_gpu_device *gpu_dev = NULL;
    struct vio_device base_dev;
    uint32_t dummy_pixels[100];

    memset(dummy_pixels, 0xFF, sizeof(dummy_pixels));
    vio_gpu_init(&gpu_dev, &base_dev);

    vio_status_t status = vio_gpu_sdl2_blit_frame(gpu_dev, dummy_pixels, sizeof(dummy_pixels));
    TEST_ASSERT(status == VIO_OK, "sdl2_blit_frame failed");
    TEST_ASSERT(gpu_dev->blitter.dirty == true, "framebuffer dirty flag not set");
}

void run_gpu_tests(void) {
    RUN_TEST(test_gpu_init);
    RUN_TEST(test_gpu_resource_create_and_scanout);
    RUN_TEST(test_gpu_sdl2_blit);
}
