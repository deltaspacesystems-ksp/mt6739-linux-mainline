#!/bin/bash
cd /home/y5/linux-7.3 || exit 1
sed -n '18,62p' drivers/gpu/drm/mediatek/mtk_disp_rdma.c
grep -nE '#define.*(L0|ADDR|PITCH|SRC_CON|ROI_SIZE|EN|RDMA)|mt8173_ovl|addr =' drivers/gpu/drm/mediatek/mtk_disp_ovl.c | head -65
