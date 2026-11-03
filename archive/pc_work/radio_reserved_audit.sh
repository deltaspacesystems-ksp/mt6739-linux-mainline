#!/bin/sh
set -eu
for node in /proc/device-tree/reserved-memory/consys-reserve-memory /proc/device-tree/reserved-memory/*ccci; do
 echo "NODE ${node##*/}"
 for prop in reg size alignment alloc-ranges; do
  if [ -f "$node/$prop" ]; then echo "$prop bytes:"; od -An -v -tx1 "$node/$prop"; fi
 done
 for prop in no-map reusable; do [ ! -f "$node/$prop" ] || echo "$prop present"; done
 done
