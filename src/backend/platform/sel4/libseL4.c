#include "../../../include/vio_crypto.h"
#include "../../../util/log.h"
#include "../../../util/mem.h"

/**
 * seL4 Crypto Enclave Backend
 * 
 * This represents a distinct seL4 Protection Domain (PD) that receives crypto
 * virtio buffers over shared memory (IPC/Notifications).
 * Keys are loaded from a separate Trusted Storage System (TSS).
 */

extern vio_status_t tss_read_key(uint32_t key_id, void *key_buf, uint32_t len);

void enclave_crypto_poll_loop(struct vio_crypto_device *dev) {
    /* In a real seL4 PD, this blocks on seL4_Wait() waiting for notifications
     * from the virtqueue transport. */
    (void)dev;
    
    vio_log(VIO_LOG_INFO, "Enclave Crypto loop started");
    
    /* Example: read key 0 from TSS on boot */
    uint8_t master_key[32];
    if (tss_read_key(0, master_key, sizeof(master_key)) == VIO_OK) {
        vio_log(VIO_LOG_INFO, "Enclave: Master key loaded from TSS");
    }
    
    /* Processing loop */
    while (1) {
        /* seL4_Wait(...) */
        /* Check shared memory virtqueue for VIRTIO_CRYPTO_OP_ENCRYPT */
        /* ... */
        break; /* Break for testing stub */
    }
}
