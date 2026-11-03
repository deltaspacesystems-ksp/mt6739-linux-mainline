#!/bin/bash
set -eu
cd /home/y5/mesa-native
meson setup build-mfw -Dvulkan-drivers=imagination -Dimagination-srv=true -Dgallium-drivers=[] -Dplatforms=[] -Dglx=disabled -Degl=disabled -Dgbm=disabled -Dopengl=false -Dgles1=disabled -Dgles2=disabled -Dllvm=enabled -Dbuild-tests=false > /mnt/d/y5_linux_work/out/mesa-configure.log 2>&1
ninja -C build-mfw -j6 > /mnt/d/y5_linux_work/out/mesa-native-build.log 2>&1
