#!/bin/bash
set -eu
cd /home/y5/linux-7.3
echo RDMA_REGISTERS
grep -nE '#define.*(MEM_START|MEM_SRC_PITCH|INT_STATUS|FRAME_END|SIZE_CON|GLOBAL_CON|MEM_CON)|irq_handler|dma_addr|addr =|write.*addr' drivers/gpu/drm/mediatek/mtk_disp_rdma.c | head -65 || true
echo DSI_STATUS
grep -nE '#define.*(INTSTA|BUSY|CMD_DONE|TE_RDY|VM_DONE|START)|irq_handler' drivers/gpu/drm/mediatek/mtk_dsi.c | head -45 || true
echo SYSFB_UPDATE
grep -nE 'mode_valid|atomic_update|fb_blit|y5disp|format_conv|damage|mode->' drivers/gpu/drm/sysfb/drm_sysfb_modeset.c | head -60 || true
echo PVR_GPU_ID
grep -RnE 'CORE_ID|CORE_REVISION|read.*bvnc|BVNC|22.49' drivers/gpu/drm/imagination/pvr_device.c drivers/gpu/drm/imagination/pvr_device_info.c drivers/gpu/drm/imagination/pvr_rogue_cr_defs.h | head -65 || true
