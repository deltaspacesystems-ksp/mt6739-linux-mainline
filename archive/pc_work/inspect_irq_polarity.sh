#!/bin/bash
set -eu
cd /home/y5/linux-7.3
grep -n -A 58 'static int gic_set_type' drivers/irqchip/irq-gic-v3.c | head -65
grep -nE 'SHADOW|shadow|LAYER_ADDR|layer_addr|DISP_REG_OVL_ADDR' drivers/gpu/drm/mediatek/mtk_disp_ovl.c | head -35
grep -n -A 25 'static void mtk_ovl_layer_config' drivers/gpu/drm/mediatek/mtk_disp_ovl.c | head -30
