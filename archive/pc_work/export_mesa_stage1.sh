#!/bin/bash
set -eu
cd /home/y5/mesa-native
ninja -C build-mfw -j6 > /mnt/d/y5_linux_work/out/mesa-ddk19-final-build.log 2>&1
readelf -h build-mfw/src/imagination/vulkan/libvulkan_powervr_mesa.so | grep -E 'Class:|Machine:'
git add -N src/imagination/vulkan/winsys/pvrsrvkm/pvr_srv_ddk19.h
git diff -- src/compiler/clc/clc_helpers.cpp src/compiler/nir/nir_fixup_is_exported.c > /mnt/d/y5_linux_work/out/mesa-llvm23-build.patch
git diff -- meson.options src/imagination/vulkan/meson.build src/imagination/vulkan/winsys/pvrsrvkm/pvr_srv_bridge.c src/imagination/vulkan/winsys/pvrsrvkm/pvr_srv_bridge.h src/imagination/vulkan/winsys/pvrsrvkm/pvr_srv_ddk19.h > /mnt/d/y5_linux_work/out/mesa-ddk19-stage1.patch
sha256sum build-mfw/src/imagination/vulkan/libvulkan_powervr_mesa.so
bash /mnt/d/y5_linux_work/test_mesa_ddk19_adapter.sh
