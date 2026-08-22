#include "nvme.h"
#include "../../util/mem.h"

/* SPDK-inspired, TRON-style NVMe Queue Pair management */

vio_status_t nvme_qpair_init(struct nvme_qpair *q, uint16_t qid, uint16_t depth, void *sq_mem, void *cq_mem) {
    if (!q || !sq_mem || !cq_mem || depth == 0) return VIO_ERR_INVAL;

    q->sq_cmds = (struct nvme_sqe *)sq_mem;
    q->cq_cmds = (struct nvme_cqe *)cq_mem;
    
    q->sq_tail = 0;
    q->cq_head = 0;
    q->q_depth = depth;
    q->phase = 1;
    q->qid = qid;
    
    vio_memset(q->sq_cmds, 0, depth * sizeof(struct nvme_sqe));
    vio_memset(q->cq_cmds, 0, depth * sizeof(struct nvme_cqe));
    
    return VIO_OK;
}

void nvme_qpair_submit(struct nvme_qpair *q, const struct nvme_sqe *cmd) {
    /* Copy command into submission queue at tail */
    struct nvme_sqe *sqe = &q->sq_cmds[q->sq_tail];
    vio_memcpy(sqe, cmd, sizeof(struct nvme_sqe));
    
    /* Advance tail */
    q->sq_tail++;
    if (q->sq_tail >= q->q_depth) {
        q->sq_tail = 0;
    }
    
    /* Ring doorbell. In real TRON systems, we use memory barriers here */
    if (q->sq_db) {
        *q->sq_db = q->sq_tail;
    }
}

int nvme_qpair_poll(struct nvme_qpair *q, struct nvme_cqe *out_cqe) {
    struct nvme_cqe *cqe = &q->cq_cmds[q->cq_head];
    
    /* Check phase bit to see if hardware updated this entry */
    uint16_t status = cqe->status;
    uint8_t phase = (status & 0x01);
    
    if (phase != q->phase) {
        return 0; /* No new completion */
    }
    
    /* We have a completion! */
    if (out_cqe) {
        vio_memcpy(out_cqe, cqe, sizeof(struct nvme_cqe));
    }
    
    /* Advance CQ head */
    q->cq_head++;
    if (q->cq_head >= q->q_depth) {
        q->cq_head = 0;
        q->phase ^= 1; /* Invert expected phase */
    }
    
    /* Ring CQ doorbell to notify controller we processed it */
    if (q->cq_db) {
        *q->cq_db = q->cq_head;
    }
    
    return 1;
}
