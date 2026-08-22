#include "nvme.h"

/* NVMe PCI initialization and memory mapping */

/* Standard NVMe Registers */
#define NVME_REG_CAP  0x00
#define NVME_REG_VS   0x08
#define NVME_REG_CC   0x14
#define NVME_REG_CSTS 0x1C

/* Doorbell base offset */
#define NVME_REG_SQ0TDBL 0x1000

vio_status_t nvme_pci_attach(struct nvme_ctrlr *ctrlr, void *bar0) {
    if (!ctrlr || !bar0) return VIO_ERR_INVAL;

    ctrlr->mmio_base = (volatile uint32_t *)bar0;
    
    /* Read CAP to determine doorbell stride */
    uint64_t cap = ((uint64_t)ctrlr->mmio_base[NVME_REG_CAP/4 + 1] << 32) | ctrlr->mmio_base[NVME_REG_CAP/4];
    uint32_t dstrd = (cap >> 32) & 0xF;
    ctrlr->doorbell_stride = 1 << (2 + dstrd);
    
    /* In a real init, we would wait for CSTS.RDY, then write CC, etc. 
     * For this phase, we just wire up the doorbells for Q0 (admin).
     */
    
    uint8_t *db_base = (uint8_t *)bar0 + NVME_REG_SQ0TDBL;
    
    /* Admin SQ Doorbell (QID 0) */
    ctrlr->admin_q.sq_db = (uint32_t *)db_base;
    /* Admin CQ Doorbell */
    ctrlr->admin_q.cq_db = (uint32_t *)(db_base + ctrlr->doorbell_stride);

    return VIO_OK;
}
