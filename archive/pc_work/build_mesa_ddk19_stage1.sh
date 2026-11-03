#!/bin/bash
set -eu
python3 /mnt/d/y5_linux_work/patch_mesa_ddk19_stage1.py
meson configure /home/y5/mesa-native/build-mfw -Dimagination-mfw-ddk19=true
ninja -C /home/y5/mesa-native/build-mfw -j6 > /mnt/d/y5_linux_work/out/mesa-native-build-ddk19.log 2>&1
