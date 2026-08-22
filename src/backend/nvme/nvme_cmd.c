#include "nvme.h"
#include "../../util/mem.h"

/* NVMe Command Builders */

#define NVME_OPC_READ  0x02
#define NVME_OPC_WRITE 0x01

void nvme_cmd_build_read(struct nvme_sqe *sqe, uint16_t cid, uint32_t nsid, uint64_t lba, uint16_t count, uint64_t prp1, uint64_t prp2) {
    vio_memset(sqe, 0, sizeof(*sqe));
    
    /* opcode in bits 0..7, cid in bits 16..31 */
    sqe->cdw0 = NVME_OPC_READ | ((uint32_t)cid << 16);
    sqe->nsid = nsid;
    sqe->dptr[0] = prp1;
    sqe->dptr[1] = prp2;
    
    /* SLBA (LBA) in CDW10 and CDW11 */
    sqe->cdw10 = (uint32_t)(lba & 0xFFFFFFFF);
    sqe->cdw11 = (uint32_t)(lba >> 32);
    
    /* Number of logical blocks (0-based) in CDW12 */
    sqe->cdw12 = (count - 1) & 0xFFFF;
}

void nvme_cmd_build_write(struct nvme_sqe *sqe, uint16_t cid, uint32_t nsid, uint64_t lba, uint16_t count, uint64_t prp1, uint64_t prp2) {
    vio_memset(sqe, 0, sizeof(*sqe));
    
    sqe->cdw0 = NVME_OPC_WRITE | ((uint32_t)cid << 16);
    sqe->nsid = nsid;
    sqe->dptr[0] = prp1;
    sqe->dptr[1] = prp2;
    
    sqe->cdw10 = (uint32_t)(lba & 0xFFFFFFFF);
    sqe->cdw11 = (uint32_t)(lba >> 32);
    
    sqe->cdw12 = (count - 1) & 0xFFFF;
}
