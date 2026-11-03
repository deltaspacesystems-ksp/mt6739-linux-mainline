#!/bin/bash
set -eu
cd /home/y5/linux-7.3
sed -n '1989,2014p' drivers/gpu/drm/drm_atomic_helper.c
rg -n 'SHADOW|shadow|REG_FLD|ADDR|LAYER_OFFSET|DRM_FORMAT_XRGB8888' drivers/gpu/drm/mediatek/mtk_disp_ovl.c | head -65
sed -n '1,130p' drivers/gpu/drm/mediatek/mtk_disp_rdma.c
sed -n '1,180p' drivers/gpu/drm/tiny/ofdrm.c
