#!/bin/bash
set -eu
cd /home/y5/linux-7.3
sed -n '1,165p' drivers/gpu/drm/imagination/pvr_fw_info.h
grep -n 'FW_MAX_SUPPORTED_MAJOR_VERSION\|PVR_FW_INFO_VERSION\|FW_BLOCK_SIZE' drivers/gpu/drm/imagination/pvr_fw.c drivers/gpu/drm/imagination/*.h
