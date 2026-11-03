#!/bin/bash
set -eu
cd /home/y5/linux-7.3
printf 'GPU SUPPORT\n'
ls drivers/gpu/drm/imagination
find drivers/gpu/drm/imagination -maxdepth 2 -type f -name '*info*' -o -name '*device*'
grep -R -nE 'GE8100|AXE|B\.V\.N\.C|BVNC|bvnc|4\.40' drivers/gpu/drm/imagination/pvr_device_info.c drivers/gpu/drm/imagination/pvr_device.c drivers/gpu/drm/imagination/pvr_gpu_info.c Documentation/devicetree/bindings/gpu/img,powervr-rogue.yaml 2>/dev/null | head -65 || true
printf 'UI DEPS\n'
for p in wayland-client cairo pangocairo; do pkg-config --modversion "$p" 2>/dev/null || true; done
