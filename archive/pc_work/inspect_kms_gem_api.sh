#!/bin/bash
set -eu
cd /home/y5/linux-7.3
find drivers/gpu/drm -name '*simpledrm*' -o -name '*ofdrm*'
sed -n '480,580p' drivers/gpu/drm/mediatek/mtk_disp_ovl.c
grep -nE 'PS_SEL|DSI_PSCTRL|HSA_WC|HBP_WC|HFP_WC' drivers/gpu/drm/mediatek/mtk_dsi.c | head -26
grep -nE 'fw_version|FW_VERSION|firmware|BVNC' drivers/gpu/drm/imagination/pvr_fw.c | head -35
