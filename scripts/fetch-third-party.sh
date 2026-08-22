tonpa@Sky-M4 scripts % cat fetch-third-party.sh
#!/usr/bin/env bash
# Synrc Hypervision (OS.1) - Third Party Dependency Fetch & Recreate Script
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
THIRD_PARTY_DIR="${ROOT_DIR}/third_party"

echo "=== Synrc Hypervision (OS.1) Recreating Third Party Directory ==="
echo "Target directory: ${THIRD_PARTY_DIR}"

rm -rf "${THIRD_PARTY_DIR}"
mkdir -p "${THIRD_PARTY_DIR}"

echo "[1/12] Fetching au-ts/libvmm..."
git clone --depth 1 https://github.com/au-ts/libvmm.git "${THIRD_PARTY_DIR}/libvmm"

echo "[2/12] Fetching au-ts/sddf..."
git clone --depth 1 https://github.com/au-ts/sddf.git "${THIRD_PARTY_DIR}/sddf"

echo "[3/12] Fetching OpenAMP (VirtIO + virtqueue)..."
git clone --depth 1 https://github.com/OpenAMP/open-amp.git "${THIRD_PARTY_DIR}/open-amp"

echo "[4/12] Fetching libmetal..."
git clone --depth 1 https://github.com/OpenAMP/libmetal.git "${THIRD_PARTY_DIR}/libmetal"

echo "[5/12] Fetching SPDK (for NVMe queue reference)..."
git clone --depth 1 --filter=blob:none --sparse https://github.com/spdk/spdk.git "${THIRD_PARTY_DIR}/spdk"
(cd "${THIRD_PARTY_DIR}/spdk" && git sparse-checkout set lib/nvme include/spdk doc)

echo "[6/12] Fetching rcore-os/virtio-drivers (MIT, excellent reference)..."
git clone --depth 1 https://github.com/rcore-os/virtio-drivers.git "${THIRD_PARTY_DIR}/virtio-drivers"

echo "[7/12] Fetching seL4 util_libs (PCI, FDT, platform)..."
git clone --depth 1 https://github.com/seL4/util_libs.git "${THIRD_PARTY_DIR}/util_libs"

echo "[8/12] Fetching BDUG/virtio-drivers-sel4 (MIT, seL4-adapted)..."
git clone --depth 1 https://github.com/BDUG/virtio-drivers-sel4.git \
  "${THIRD_PARTY_DIR}/virtio-drivers-sel4"

echo "[9/12] Fetching FreeRTOS-Labs-libvirtio (BSD-2-Clause, minimal C VirtIO)..."
git clone --depth 1 https://github.com/CTSRD-CHERI/FreeRTOS-Labs-libvirtio.git \
  "${THIRD_PARTY_DIR}/libvirtio-freertos"

echo "[10/12] Fetching EDK2 OvmfPkg VirtioLib (BSD-2-Clause-Patent)..."
git clone --depth 1 --filter=blob:none --sparse \
  https://github.com/tianocore/edk2.git \
  "${THIRD_PARTY_DIR}/edk2"

(cd "${THIRD_PARTY_DIR}/edk2" && \
  git sparse-checkout set \
    OvmfPkg/Library/VirtioLib \
    OvmfPkg/Include/Library \
    OvmfPkg/Include/IndustryStandard \
    OvmfPkg/Include/Protocol)

# Optional convenience symlink
ln -sfn edk2/OvmfPkg/Library/VirtioLib "${THIRD_PARTY_DIR}/virtio-lib-edk2"

echo "[11/12] Fetching libkrun (Apache-2.0, modern VirtIO devices reference)..."
git clone --depth 1 https://github.com/containers/libkrun.git \
  "${THIRD_PARTY_DIR}/libkrun"

echo "[12/12] Fetching Mellanox/virtio-emulation (DPDK-based, BSD-3-Clause)..."
git clone --depth 1 https://github.com/Mellanox/virtio-emulation.git \
  "${THIRD_PARTY_DIR}/virtio-emulation"

echo "=== Third Party Dependencies Refetched Successfully ==="
ls -la "${THIRD_PARTY_DIR}"
