#include "test_runner.h"
#include "../src/include/vio_queue.h"
#include "../src/util/mem.h"

void test_queue_init_invalid(void) {
    struct vio_virtqueue vq;
    char mem[4096];
    
    vio_status_t st = vio_queue_init(NULL, mem, 16);
    TEST_ASSERT(st == VIO_ERR_INVAL, "Expected VIO_ERR_INVAL for NULL vq");
    
    st = vio_queue_init(&vq, mem, 15);
    TEST_ASSERT(st == VIO_ERR_INVAL, "Expected VIO_ERR_INVAL for non-power-of-2 size");
}

void test_queue_init_valid(void) {
    struct vio_virtqueue vq;
    char mem[4096];
    vio_memset(mem, 0, sizeof(mem));
    
    vio_status_t st = vio_queue_init(&vq, mem, 16);
    TEST_ASSERT(st == VIO_OK, "Expected VIO_OK for valid init");
    TEST_ASSERT(vq.queue_size == 16, "Queue size mismatch");
    TEST_ASSERT(vq.desc != NULL, "Desc ring not initialized");
    TEST_ASSERT(vq.avail != NULL, "Avail ring not initialized");
    TEST_ASSERT(vq.used != NULL, "Used ring not initialized");
}

void test_queue_has_used(void) {
    struct vio_virtqueue vq;
    char mem[8192]; /* Need enough space for page alignment */
    vio_memset(mem, 0, sizeof(mem));
    
    vio_queue_init(&vq, mem, 16);
    
    TEST_ASSERT(vio_queue_has_used(&vq) == 0, "Queue should be empty initially");
    
    /* Simulate hardware placing an element in the used ring */
    vq.used->idx = 1;
    
    TEST_ASSERT(vio_queue_has_used(&vq) == 1, "Queue should have used element");
}

void test_virtio_data_transfer(void) {
    struct vio_virtqueue vq;
    char mem[8192]; 
    vio_memset(mem, 0, sizeof(mem));
    
    vio_queue_init(&vq, mem, 16);
    
    /* 1. Guest sets up a buffer with some data */
    char guest_buf[32] = "hello_virtio";
    
    /* 2. Guest places buffer in descriptor table */
    vq.desc[0].addr = (uint64_t)(uintptr_t)guest_buf;
    vq.desc[0].len = 32;
    vq.desc[0].flags = 0;
    
    /* 3. Guest adds descriptor index to avail ring */
    vq.avail->ring[0] = 0;
    
    /* Ensure the memory barrier would happen here in real code */
    vq.avail->idx = 1;
    
    /* --- Host (Backend) side processing --- */
    
    /* Backend checks if there is work (simplistic check for skeleton) */
    TEST_ASSERT(vq.last_avail_idx != vq.avail->idx, "Host should see new avail element");
    
    /* Host pops the descriptor index */
    uint16_t desc_idx = vq.avail->ring[vq.last_avail_idx % vq.queue_size];
    vq.last_avail_idx++;
    
    /* Host processes the data */
    char *host_ptr = (char *)(uintptr_t)vq.desc[desc_idx].addr;
    TEST_ASSERT(vio_memcmp(host_ptr, "hello_virtio", 12) == 0, "Host did not read correct data");
    
    /* Host modifies the data (byte moving) */
    vio_memcpy(host_ptr, "virtio_ack", 11);
    
    /* Host puts descriptor in used ring */
    uint16_t used_slot = vq.last_used_idx % vq.queue_size;
    vq.used->ring[used_slot].id = desc_idx;
    vq.used->ring[used_slot].len = 11;
    
    /* Memory barrier would go here */
    vq.used->idx++;
    
    /* --- Guest side completion --- */
    
    /* Guest checks used ring */
    TEST_ASSERT(vio_queue_has_used(&vq) == 1, "Guest should see completed work");
    
    /* Guest verifies data */
    TEST_ASSERT(vio_memcmp(guest_buf, "virtio_ack", 11) == 0, "Guest did not receive modified data");
}

void run_queue_tests(void) {
    RUN_TEST(test_queue_init_invalid);
    RUN_TEST(test_queue_init_valid);
    RUN_TEST(test_queue_has_used);
    RUN_TEST(test_virtio_data_transfer);
}
