# libvio

`libvio` gives you a zero-dependency, TRON real-time discipline poll-mode VirtIO/NVMe library that allows targets like seL4/Microkit, libkrun, or Erlang unikernels to achieve even lower boot latency and higher density than others while providing direct poll-mode NVMe storage capability without the overhead of standard Linux/QEMU device emulation.

## Intro

`libvio` is a pure C11 freestanding virtualization driver library designed with extreme real-time discipline. By combining the zero-hidden-allocation approach of TRON real-time OS designs with the clean, modular aesthetics of BeOS and Haiku, `libvio` offers high-performance poll-mode fast paths for modern PCIe NVMe block devices, virtio-net, virtio-console, virtio-crypto, and bus discovery.

## Requirements

* **C Compiler:** Any C11 compliant compiler (`clang`, `gcc`) with support for `-std=c11` and `-fPIC`.

* **Build System:** Standard NetBSD/BeOS-style POSIX `make`.

* **Host Platform (Optional/Testing):** macOS (Apple Silicon/Intel with `libkrun`), Linux (VFIO/UIO/vhost-user), FreeBSD/NetBSD, or seL4 microkernel (Microkit / sDDF).

* **Dependencies:** **Zero external runtime dependencies.**

## Key Properties

1. **TRON Real-Time Discipline:** Predictable execution with zero hidden heap allocations or implicit locks on hot I/O poll paths.

2. **SPDK-Inspired Poll-Mode NVMe Fast Path:** Direct circular Submission Queue (SQ) and Completion Queue (CQ) polling with phase-bit toggling for ultra-low latency I/O.

3. **BeOS / Haiku Architectural Clarity:** Clean header separation, modular front-ends (`blk`, `net`, `console`, `crypto`, `bus`), and explicitly decoupled backends.

4. **Multi-Platform Control-Plane Abstraction:** Modular `vio_platform_ops` supporting:
   - `platform/sel4`: seL4 Protection Domains (`libseL4.c` for sDDF & isolated crypto enclave with TSS key persistence).
   - `platform/apple`: macOS `libkrun` microVM RAM mapping (`libkrun.c`).
   - `platform/linux`: VFIO userspace PCIe & `vhost-user` (`liblinux.c`).
   - `platform/hyperv`: Windows Hyper-V integration (`libhyperv.c`).
   - `platform/netbsd`: BSD `/dev/pci` mapping (`libbsd.c`).

5. **Freestanding & Dual Dual-License Option:** Pure C11 code suitable for bare-metal, unikernels, microkernel protection domains, and ring-0 host OS modules under Apache-2.0 / MIT / BSD licenses.

---

## Tree Structure

```text
vio/
├── Makefile
├── README.md
├── src/
│   ├── include/                  # Frozen Public API Headers
│   │   ├── vio.h                 # Core VirtIO API entrypoint
│   │   ├── vio_types.h           # Base types & status codes
│   │   ├── vio_queue.h           # Virtqueue split-ring layout structures
│   │   ├── vio_transport.h       # MMIO / PCI transport abstraction
│   │   ├── vio_platform.h        # Platform memory & IRQ mapping hooks
│   │   ├── vio_blk.h             # VirtIO Block device header
│   │   ├── vio_net.h             # VirtIO Network device header
│   │   ├── vio_console.h         # VirtIO Console header
│   │   ├── vio_crypto.h          # VirtIO Crypto device header
│   │   └── vio_bus.h             # Device bus & discovery header
│   ├── core/                     # VirtIO State Engine
│   │   ├── queue.c               # Virtqueue ring processing
│   │   ├── transport_mmio.c      # MMIO transport implementation
│   │   ├── transport_pci.c       # Modern PCIe transport implementation
│   │   ├── feature.c             # VirtIO feature bit negotiation
│   │   └── config.c              # Device config space access
│   ├── devices/                  # Device Front-ends
│   │   ├── blk.c                 # Block device driver
│   │   ├── net.c                 # Network device driver
│   │   ├── console.c             # Console device driver
│   │   ├── crypto.c              # Crypto device driver
│   │   └── bus.c                 # Device enumeration & bus manager
│   ├── backend/                  # Platform & Hardware Fast Paths
│   │   ├── null.c                # Stub backend driver
│   │   ├── nvme/                 # Direct PCIe NVMe Fast Path
│   │   │   ├── nvme.h            # Hardware registers & qpair structs
│   │   │   ├── nvme_qpair.c      # Submission/Completion poll queues
│   │   │   ├── nvme_cmd.c        # Command formatting (Read/Write SQE)
│   │   │   └── nvme_pci.c        # Doorbell stride calculation
│   │   └── platform/             # Control-plane target drivers
│   │       ├── sel4/             # seL4 microkit / sDDF & Crypto Enclave
│   │       │   ├── libseL4.c
│   │       │   └── tss_stub.c
│   │       ├── apple/            # Apple libkrun
│   │       │   └── libkrun.c
│   │       ├── linux/            # Linux VFIO / vhost-user
│   │       │   └── liblinux.c
│   │       ├── hyperv/           # Windows Hyper-V
│   │       │   └── libhyperv.c
│   │       └── netbsd/           # NetBSD / BSD /dev/pci
│   │           └── libbsd.c
│   └── util/                     # Zero-dependency Utilities
│       ├── list.h / list.c       # Doubly-linked list helpers
│       ├── atomic.h / atomic.c   # Atomic memory barriers
│       ├── mem.h / mem.c         # Freestanding memory routines
│       └── log.h / log.c         # Light log routines
├── test/                         # Custom Test Suite
│   ├── main.c                    # Runner entrypoint
│   ├── test_runner.h             # Zero-dependency test macros
│   ├── queue_test.c              # Virtqueue & data movement tests
│   ├── blk_test.c                # Block device tests
│   ├── nvme_loopback.c           # NVMe hardware simulation
│   └── krun_test.c               # libkrun platform tests
└── third_party/
    └── libkrun/                  # Embedded libkrun headers & source
```

## Test Assurance

`libvio` includes a custom, zero-dependency test suite embedded directly in `test/`.

To build the static library, dynamic library, fast-path NVMe library, Crypto Enclave, and execute tests:

```bash
make test
```

### Test Suite Summary

* **Virtqueue Mechanics:** Validates queue initialization, layout boundaries, avail/used ring wrap-around, and live byte movement (`test_virtio_data_transfer`).

* **Device Front-ends:** Validates initialization and feature negotiation for VirtIO Block and Net devices.

* **NVMe Hardware Loopback:** Simulates hardware 
submission and completion queues, verifying phase-bit toggles and doorbell register updates.

* **Platform Integrations:** Validates `libkrun` guest RAM translation and context configuration.

```text
Starting libvio test suite...
Running test_queue_init_invalid... PASS
Running test_queue_init_valid... PASS
Running test_queue_has_used... PASS
Running test_virtio_data_transfer... PASS
Running test_blk_init... PASS
Running test_blk_read_unsupported... PASS
Running test_nvme_qpair_submit_poll... PASS
Running test_libkrun_mapping... PASS
Running test_libkrun_blk_init... PASS
Running test_libkrun_net_init... PASS
Running test_libkrun_create_context... PASS
=== TEST SUMMARY ===
Run: 11
Passed: 11
Failed: 0
```

## Conclusion

`libvio` provides an ultra-lightweight, predictable foundation for modern virtualization. By bypassing heavy kernel emulation layers and offering direct poll-mode fast paths for NVMe and VirtIO devices, `libvio` empowers high-density, real-time unikernels and microkernel protection domains to achieve state-of-the-art performance.

## Credits

* Namdak Tonpa — BDFL VE OS.1 Architect

