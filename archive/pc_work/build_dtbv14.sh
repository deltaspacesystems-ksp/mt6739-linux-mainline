#!/bin/bash
set -eu
cd /mnt/d/y5_linux_work
dtc -I dts -O dtb -o out/v14.dtb out/v14.dts 2>out/v14-dtc.log
fdtget -t s out/v14.dtb / compatible
