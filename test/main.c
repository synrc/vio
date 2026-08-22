#include "test_runner.h"

extern void run_queue_tests(void);
extern void run_blk_tests(void);
extern void run_nvme_tests(void);
extern void run_krun_tests(void);

/*
libvio gives you a zero-dependency, TRON real-time discipline poll-mode VirtIO/NVMe library
that allows targets like seL4/Microkit, libkrun, or Erlang unikernels to achieve even lower
boot latency and higher density than NanoVMs, while providing direct poll-mode NVMe storage
capability without the overhead of standard Linux/QEMU device emulation.
*/

int g_tests_run = 0;
int g_tests_passed = 0;
int g_tests_failed = 0;

int main(void) {
    printf("Starting libvio test suite...\n\n");
    
    run_queue_tests();
    run_blk_tests();
    run_nvme_tests();
    run_krun_tests();
    
    TEST_SUMMARY();
    return 0;
}
