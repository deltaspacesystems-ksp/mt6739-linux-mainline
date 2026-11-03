#!/bin/bash
set -eu
cd /mnt/d/y5_linux_work
dtc -I dts -O dtb -o out/v13.dtb out/v13.dts 2>out/v13-dtc.log
ls -l out/v13.dtb
