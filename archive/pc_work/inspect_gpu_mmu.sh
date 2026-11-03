#!/bin/bash
set -eu
cd /home/y5/linux-7.3
grep -nE '22,|22_|49|BVNC|[.]b =|[.]v =|[.]n =|[.]c =' drivers/gpu/drm/imagination/pvr_device_info.c | head -45
grep -nE 'MMU_EN|NONSEC|mt8173|mt6779|SMI_LARB' drivers/memory/mtk-smi.c | head -55
grep -nE 'SHADOW|shadow|LAYER_ADDR|LAYER_CON|mt8173|mt6779' drivers/gpu/drm/mediatek/mtk_disp_ovl.c | head -40
