#!/bin/bash
set -eu
cd /mnt/d/y5_linux_work
dtc -I dts -O dtb -o out/v18.dtb out/v18.dts 2>out/v18-dtc.log
fdtget -t s out/v18.dtb /mfw-gpu-id-probe@10006000 compatible
fdtget -t x out/v18.dtb /mfw-gpu-id-probe@10006000 vcore-supply
