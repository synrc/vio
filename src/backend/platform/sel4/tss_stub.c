#include "../../../include/vio_types.h"
#include "../../../util/mem.h"

/**
 * Trusted Storage System (TSS) persistence stub.
 * In a real enclave, this communicates with RPMB, a TPM, or a trusted disk partition.
 */

vio_status_t tss_read_key(uint32_t key_id, void *key_buf, uint32_t len) {
    if (!key_buf || len == 0) return VIO_ERR_INVAL;
    
    /* Stub implementation returning a fake key */
    (void)key_id;
    vio_memset(key_buf, 0x42, len);
    
    return VIO_OK;
}

vio_status_t tss_write_key(uint32_t key_id, const void *key_buf, uint32_t len) {
    if (!key_buf || len == 0) return VIO_ERR_INVAL;
    
    /* Stub */
    (void)key_id;
    
    return VIO_OK;
}
