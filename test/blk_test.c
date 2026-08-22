#include "test_runner.h"
#include "../src/include/vio_blk.h"

void test_blk_init(void) {
    struct vio_blk_device *dev;
    struct vio_device base;
    
    vio_status_t st = vio_blk_init(&dev, &base);
    TEST_ASSERT(st == VIO_OK, "Expected VIO_OK for blk init");
    TEST_ASSERT(dev != NULL, "Expected non-NULL device");
}

void test_blk_read_unsupported(void) {
    struct vio_blk_device *dev;
    struct vio_device base;
    char buf[512];
    
    vio_blk_init(&dev, &base);
    vio_status_t st = vio_blk_read(dev, 0, buf, 1);
    TEST_ASSERT(st == VIO_ERR_UNSUPPORTED, "Expected VIO_ERR_UNSUPPORTED for default backend");
}

void run_blk_tests(void) {
    RUN_TEST(test_blk_init);
    RUN_TEST(test_blk_read_unsupported);
}
