#include "test_runner.h"
#include "../src/backend/nvme/nvme.h"
#include "../src/util/mem.h"

void test_nvme_qpair_submit_poll(void) {
    struct nvme_qpair q;
    struct nvme_sqe sq_mem[4];
    struct nvme_cqe cq_mem[4];
    uint32_t sq_db = 0;
    uint32_t cq_db = 0;
    
    vio_status_t st = nvme_qpair_init(&q, 1, 4, sq_mem, cq_mem);
    TEST_ASSERT(st == VIO_OK, "QPair init failed");
    
    q.sq_db = &sq_db;
    q.cq_db = &cq_db;
    
    struct nvme_sqe cmd;
    nvme_cmd_build_read(&cmd, 123, 1, 0x1000, 1, 0x2000, 0);
    
    nvme_qpair_submit(&q, &cmd);
    TEST_ASSERT(q.sq_tail == 1, "SQ tail not advanced");
    TEST_ASSERT(sq_db == 1, "SQ doorbell not rung");
    TEST_ASSERT(sq_mem[0].cdw10 == 0x1000, "LBA not set correctly");
    
    /* Simulate hardware completion */
    cq_mem[0].cid = 123;
    cq_mem[0].status = q.phase; /* Set phase bit to current phase (1) */
    
    struct nvme_cqe cqe_out;
    int has_completion = nvme_qpair_poll(&q, &cqe_out);
    
    TEST_ASSERT(has_completion == 1, "Poll should find completion");
    TEST_ASSERT(cqe_out.cid == 123, "CID mismatch");
    TEST_ASSERT(q.cq_head == 1, "CQ head not advanced");
    TEST_ASSERT(cq_db == 1, "CQ doorbell not rung");
    
    /* Should not have more completions */
    has_completion = nvme_qpair_poll(&q, &cqe_out);
    TEST_ASSERT(has_completion == 0, "Poll should not find completion");
}

void run_nvme_tests(void) {
    RUN_TEST(test_nvme_qpair_submit_poll);
}
