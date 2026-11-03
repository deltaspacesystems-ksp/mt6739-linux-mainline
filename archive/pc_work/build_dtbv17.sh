#!/bin/bash
set -eu
cd /mnt/d/y5_linux_work
dtc -I dts -O dtb -o out/v17.dtb out/v17.dts 2>out/v17-dtc.log
fdtget -t x out/v17.dtb /disp_rdma0@1400b000 interrupts
fdtget -t s out/v17.dtb /disp_rdma0@1400b000 compatible
