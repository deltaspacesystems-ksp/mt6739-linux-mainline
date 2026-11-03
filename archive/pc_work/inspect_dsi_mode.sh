#!/bin/bash
cd /home/y5/linux-7.3 || exit 1
sed -n '33,65p' drivers/gpu/drm/mediatek/mtk_dsi.c
