# libvio Makefile

# Targets
LIBVIO = libvio.a
LIBVIO_SO = libvio.so
LIBVIO_NVME = libvio-nvme.a
LIBVIO_ENCLAVE = libvio-crypto-enclave.a

# Compilers and flags
CC ?= cc
AR ?= ar
RANLIB ?= ranlib

CFLAGS += -O2 -Wall -Wextra -Wpedantic -std=c11 -fPIC -ffreestanding
CFLAGS += -Isrc/include -Ithird_party/libkrun/include

KRUN_DIR ?= third_party/libkrun
HAS_KRUN_LIB := $(wildcard $(KRUN_DIR)/target/release/libkrun.dylib $(KRUN_DIR)/target/release/libkrun.so $(KRUN_DIR)/target/release/libkrun.a $(KRUN_DIR)/libkrun.dylib $(KRUN_DIR)/libkrun.so $(KRUN_DIR)/libkrun.a)
ifneq ($(HAS_KRUN_LIB),)
KRUN_LDFLAGS ?= -L$(KRUN_DIR)/target/release -L$(KRUN_DIR) -lkrun -Wl,-rpath,$(KRUN_DIR)/target/release -Wl,-rpath,$(KRUN_DIR) -Wl,-rpath,.
$(shell [ -f $(KRUN_DIR)/target/release/libkrun.dylib ] && [ ! -f $(KRUN_DIR)/target/release/libkrun.2.dylib ] && ln -sf libkrun.dylib $(KRUN_DIR)/target/release/libkrun.2.dylib)
$(shell [ -f $(KRUN_DIR)/libkrun.dylib ] && [ ! -f $(KRUN_DIR)/libkrun.2.dylib ] && ln -sf libkrun.dylib $(KRUN_DIR)/libkrun.2.dylib)
else
KRUN_LDFLAGS ?=
endif

# Core and Util Sources
SRCS_CORE = src/core/queue.c \
            src/core/transport_mmio.c \
            src/core/transport_pci.c \
            src/core/feature.c \
            src/core/config.c

SRCS_UTIL = src/util/list.c \
            src/util/atomic.c \
            src/util/mem.c \
            src/util/log.c

SRCS_DEV  = src/devices/blk.c \
            src/devices/net.c \
            src/devices/console.c \
            src/devices/crypto.c \
            src/devices/bus.c \
            src/devices/gpu.c

# We link a null backend and libkrun platform by default
SRCS_BACKEND = src/backend/null.c \
               src/backend/platform/apple/libkrun.c

SRCS_ALL = $(SRCS_CORE) $(SRCS_UTIL) $(SRCS_DEV) $(SRCS_BACKEND)
OBJS_ALL = $(SRCS_ALL:.c=.o)

# NVMe specific
SRCS_NVME = src/backend/nvme/nvme_qpair.c \
            src/backend/nvme/nvme_cmd.c \
            src/backend/nvme/nvme_pci.c
OBJS_NVME = $(SRCS_NVME:.c=.o)

# Crypto Enclave specific
SRCS_ENCLAVE = src/backend/platform/sel4/libseL4.c \
               src/backend/platform/sel4/tss_stub.c
OBJS_ENCLAVE = $(SRCS_ENCLAVE:.c=.o)

all: $(LIBVIO) $(LIBVIO_SO) $(LIBVIO_NVME) $(LIBVIO_ENCLAVE)

$(LIBVIO): $(OBJS_ALL)
	rm -f $@
	$(AR) rcs $@ $(OBJS_ALL)
	$(RANLIB) $@

$(LIBVIO_SO): $(OBJS_ALL)
	$(CC) -shared -o $@ $(OBJS_ALL)

$(LIBVIO_NVME): $(OBJS_NVME)
	rm -f $@
	$(AR) rcs $@ $(OBJS_NVME)
	$(RANLIB) $@

$(LIBVIO_ENCLAVE): $(OBJS_ENCLAVE)
	rm -f $@
	$(AR) rcs $@ $(OBJS_ENCLAVE)
	$(RANLIB) $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

# Testing
TEST_SRCS = test/main.c test/queue_test.c test/blk_test.c test/nvme_loopback.c test/krun_test.c test/gpu_test.c
TEST_OBJS = $(TEST_SRCS:.c=.o)
TEST_BIN  = vio_test

$(TEST_BIN): $(TEST_OBJS) $(LIBVIO) $(LIBVIO_NVME)
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJS) -L. -lvio -lvio-nvme $(KRUN_LDFLAGS)

test: $(TEST_BIN)
	DYLD_LIBRARY_PATH="$(KRUN_DIR)/target/release:$(KRUN_DIR):$$DYLD_LIBRARY_PATH" LD_LIBRARY_PATH="$(KRUN_DIR)/target/release:$(KRUN_DIR):$$LD_LIBRARY_PATH" ./$(TEST_BIN)

clean:
	rm -f $(OBJS_ALL) $(OBJS_NVME) $(OBJS_ENCLAVE) $(LIBVIO) $(LIBVIO_SO) $(LIBVIO_NVME) $(LIBVIO_ENCLAVE) $(TEST_OBJS) $(TEST_BIN)

.PHONY: all clean test
