#!/bin/sh
set -eu
root=/sys/firmware/devicetree/base
for node in vdec@17040000 vdec_gcon@17000000 venc@17020000 venc_global_con@17000000 venc_jpg@17030000 seninf@15008000 camsv@15009000 audio@11220000; do
 [ -d "$root/$node" ] || continue
 echo "NODE $node"
 for p in compatible status clock-names; do
  if [ -f "$root/$node/$p" ]; then printf '%s=' "$p";tr '\000' ' ' < "$root/$node/$p";echo;fi
 done
 for p in reg interrupts clocks; do
  if [ -f "$root/$node/$p" ]; then echo "$p bytes:";od -An -v -tx1 "$root/$node/$p";fi
 done
done
