#!/bin/bash
set -eu
cd /home/y5/linux-7.3
find drivers/pmdomain/mediatek -iname '*673*'
cat Documentation/devicetree/bindings/gpu/img,powervr-rogue.yaml | head -145
sed -n '90,158p' drivers/gpu/drm/imagination/pvr_fw.c
grep -nE 'META|MIPS|FW_MAX_SUPPORTED' drivers/gpu/drm/imagination/pvr_fw*.h | head -30
