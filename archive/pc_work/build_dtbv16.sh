#!/bin/bash
set -eu
cd /mnt/d/y5_linux_work
dtc -I dts -O dtb -o out/v16.dtb out/v16.dts 2>out/v16-dtc.log
fdtget -t x out/v16.dtb /disp_rdma0@1400b000 interrupts
fdtget -t s out/v16.dtb /disp_rdma0@1400b000 compatible
