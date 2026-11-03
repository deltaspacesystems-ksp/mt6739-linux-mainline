#!/bin/sh
/tmp/mfw-pvr-native-probe --abi
abi_rc=$?
/tmp/mfw-pvr-native-probe --connect /dev/null
refusal_rc=$?
printf 'NATIVE_ABI_RC=%s REFUSAL_RC=%s\n' "$abi_rc" "$refusal_rc"
test "$abi_rc" -eq 0 && test "$refusal_rc" -eq 1
