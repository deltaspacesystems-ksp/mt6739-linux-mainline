#!/bin/bash
set -eu
cd /home/y5/linux-7.3
for unit in kernel/drivers/staging/imgtec/pvr_drm services/server/env/linux/physmem_dmabuf kernel/drivers/staging/imgtec/pvr_sync_dma_fence kernel/drivers/staging/imgtec/pvr_counting_timeline kernel/drivers/staging/imgtec/pvr_sw_fence kernel/drivers/staging/imgtec/pvr_fence services/server/env/linux/dma_fence_sync_native_server services/server/env/linux/osfunc; do
  log=/mnt/d/y5_linux_work/out/ddk19-${unit##*/}.log
  if make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- M=/mnt/d/y5_linux_work/rogue-ddk19-port MTK_PLATFORM=mt6739 W=1 "$unit.o" > "$log" 2>&1; then
    printf 'PASS %s\n' "$unit"
  else
    printf 'BLOCKED %s\n' "$unit"
    if command -v rg >/dev/null; then
      rg -m5 'fatal error:|error:' "$log" || true
    else
      grep -m5 -E 'fatal error:|error:' "$log" || true
    fi
  fi
done
