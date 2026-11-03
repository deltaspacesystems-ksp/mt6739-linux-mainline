#!/bin/bash
set -eu
python3 /mnt/d/y5_linux_work/fix_mesa_llvm23.py
ninja -C /home/y5/mesa-native/build-mfw -j6 > /mnt/d/y5_linux_work/out/mesa-native-build-llvm23.log 2>&1
