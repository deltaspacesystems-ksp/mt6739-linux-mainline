#!/bin/bash
set -eu
cd /mnt/d/y5_linux_work
dtc -I dts -O dtb -o out/v12.dtb out/v12.dts 2>out/v12-dtc.log
ls -l out/v12.dtb