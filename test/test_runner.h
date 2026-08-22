#ifndef VIO_TEST_RUNNER_H
#define VIO_TEST_RUNNER_H

#include <stdio.h>
#include <stdlib.h>

/**
 * Minimal C test harness to avoid external dependencies.
 */

extern int g_tests_run;
extern int g_tests_passed;
extern int g_tests_failed;

#define TEST_ASSERT(cond, msg) \
    do { \
        if (!(cond)) { \
            printf("[FAIL] %s:%d: %s\n", __FILE__, __LINE__, msg); \
            g_tests_failed++; \
            return; \
        } \
    } while (0)

#define RUN_TEST(test_func) \
    do { \
        int initial_fails = g_tests_failed; \
        printf("Running %s... ", #test_func); \
        g_tests_run++; \
        test_func(); \
        if (g_tests_failed == initial_fails) { \
            printf("PASS\n"); \
            g_tests_passed++; \
        } \
    } while (0)

#define TEST_SUMMARY() \
    do { \
        printf("\n=== TEST SUMMARY ===\n"); \
        printf("Run: %d\nPassed: %d\nFailed: %d\n", g_tests_run, g_tests_passed, g_tests_failed); \
        if (g_tests_failed > 0) exit(1); \
    } while (0)

#endif /* VIO_TEST_RUNNER_H */
