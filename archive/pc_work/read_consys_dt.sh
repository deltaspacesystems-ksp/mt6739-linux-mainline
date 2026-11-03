#!/bin/sh
set -eu
for prop in reg clock-names clocks interrupts vcn18-supply vcn28-supply vcn33_bt-supply vcn33_wifi-supply; do
 p=/sys/firmware/devicetree/base/consys@18070000/$prop
 if [ -f "$p" ]; then echo "$prop bytes:";od -An -v -tx1 "$p";fi
done
