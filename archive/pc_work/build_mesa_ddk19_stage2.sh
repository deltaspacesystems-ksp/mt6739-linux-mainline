#!/bin/bash
set -eu
python3 /mnt/d/y5_linux_work/patch_mesa_ddk19_stage2.py
ninja -C /home/y5/mesa-native/build-mfw -j6 > /mnt/d/y5_linux_work/out/mesa-native-build-ddk19-stage2.log 2>&1
