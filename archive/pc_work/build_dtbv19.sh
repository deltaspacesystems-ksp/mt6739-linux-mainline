#!/bin/bash
set -eu
cd /mnt/d/y5_linux_work
dtc -I dts -O dtb -o out/v19.dtb out/v19.dts 2>out/v19-dtc.log
fdtget -t s out/v19.dtb /gpu@13000000 compatible
fdtget -t s out/v19.dtb /mfw-gpu-id-probe@10006000 status
