#!/bin/bash
set -eu
cd /mnt/d/y5_linux_work
dtc -I dts -O dtb -o out/v15.dtb out/v15.dts 2>out/v15-dtc.log
fdtget -t x out/v15.dtb /framebuffer@775eb000 reg
