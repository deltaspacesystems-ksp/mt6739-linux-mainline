#!/bin/bash
set -eu
ninja -C /home/y5/mesa-native/build-mfw -j6 > /mnt/d/y5_linux_work/out/mesa-stage2-final-build.log 2>&1
python3 /mnt/d/y5_linux_work/export_mesa_stage2.py
