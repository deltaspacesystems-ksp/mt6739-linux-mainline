#!/bin/bash
cd /home/y5/linux-7.3
yes "" | make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -j6 Image > /home/y5/build73f.log 2>&1
echo done >> /home/y5/build73f.log
