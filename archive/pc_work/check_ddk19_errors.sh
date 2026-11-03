#!/bin/bash
set -eu
source <(sed '/^cc /,$d' /mnt/d/y5_linux_work/build_native_pvr_probe.sh)
printf '#include <stdio.h>\n#include "common_srvcore_bridge.h"\nint main(void){printf("retry=%%d bridge=%%d\\n",PVRSRV_ERROR_RETRY,PVRSRV_ERROR_BRIDGE_CALL_FAILED);}\n' > /tmp/check-pvr-errors.c
cc "${flags[@]}" /tmp/check-pvr-errors.c -o /tmp/check-pvr-errors
/tmp/check-pvr-errors
