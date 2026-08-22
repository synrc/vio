#ifndef VIO_NVME_H
#define VIO_NVME_H

#include "../../include/vio_types.h"

/**
 * Internal NVMe structures for poll-mode PCIe NVMe.
 * Modeled strictly on hardware TRON principles: SPDK-style, zero-allocation.
 */

/* NVMe Submission Queue Entry (SQE) */
struct nvme_sqe {
    uint32_t cdw0;  /* opc, fuse, cid, etc */
    uint32_t nsid;
    uint64_t rsvd2;
    uint64_t mptr;
    uint64_t dptr[2]; /* PRP1 and PRP2 */
    uint32_t cdw10;
    uint32_t cdw11;
    uint32_t cdw12;
    uint32_t cdw13;
    uint32_t cdw14;
    uint32_t cdw15;
};

/* NVMe Completion Queue Entry (CQE) */
struct nvme_cqe {
    uint32_t cdw0;
    uint32_t rsvd1;
    uint16_t sq_head;
    uint16_t sq_id;
    uint16_t cid;
    uint16_t status;
};

/* NVMe Queue Pair (QPair) */
struct nvme_qpair {
    struct nvme_sqe *sq_cmds;
    struct nvme_cqe *cq_cmds;
    uint32_t *sq_db; /* Submission Queue Doorbell */
    uint32_t *cq_db; /* Completion Queue Doorbell */
    
    uint16_t sq_tail;
    uint16_t cq_head;
    uint16_t q_depth;
    
    uint8_t phase;   /* Phase tag for completion queue */
    uint16_t qid;    /* Queue ID */
};

/* Controller abstraction */
struct nvme_ctrlr {
    volatile uint32_t *mmio_base;
    uint32_t doorbell_stride;
    struct nvme_qpair admin_q;
    struct nvme_qpair io_q;
};

/* Setup and Doorbell functions */
vio_status_t nvme_qpair_init(struct nvme_qpair *q, uint16_t qid, uint16_t depth, void *sq_mem, void *cq_mem);
void nvme_qpair_submit(struct nvme_qpair *q, const struct nvme_sqe *cmd);
int nvme_qpair_poll(struct nvme_qpair *q, struct nvme_cqe *out_cqe);

/* Command generation */
void nvme_cmd_build_read(struct nvme_sqe *sqe, uint16_t cid, uint32_t nsid, uint64_t lba, uint16_t count, uint64_t prp1, uint64_t prp2);
void nvme_cmd_build_write(struct nvme_sqe *sqe, uint16_t cid, uint32_t nsid, uint64_t lba, uint16_t count, uint64_t prp1, uint64_t prp2);

/* PCI Binding */
vio_status_t nvme_pci_attach(struct nvme_ctrlr *ctrlr, void *bar0);

#endif /* VIO_NVME_H */
