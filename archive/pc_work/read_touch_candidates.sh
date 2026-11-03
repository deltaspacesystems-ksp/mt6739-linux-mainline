#!/bin/sh
base=/sys/firmware/devicetree/base/touch/ts_kit
for node in goodix_ts@5d focaltech@38 mstar_ts@26; do
 echo "TOUCH_CANDIDATE $node"
 for prop in compatible slave_address chip_id ic_type hard_reset_delay x_max y_max goodix,product_id mstar,product_id; do
  [ -f "$base/$node/$prop" ] || continue
  printf '%s=' "$prop"
  case "$prop" in
   compatible|goodix,product_id|mstar,product_id) tr '\000' ' ' < "$base/$node/$prop"; echo;;
   *) od -An -v -tx1 "$base/$node/$prop";;
  esac
 done
done
