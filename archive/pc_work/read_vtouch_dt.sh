(
for d in /sys/firmware/devicetree/base/mt_pmic_regulator/ldo_regulators/*; do
 [ -f "$d/phandle" ] || continue
 v=$(od -An -v -tx1 "$d/phandle" | tr -d ' \n')
 if [ "$v" = 0000008b ]; then
  echo VTOUCH_NODE=$d
  tr '\000' ' ' < "$d/regulator-name"; echo
  for n in regulator-min-microvolt regulator-max-microvolt; do [ ! -f "$d/$n" ] || { echo "$n"; od -An -v -tx1 "$d/$n"; }; done
 fi
done
ls -l /dev/uinput /dev/input/uinput 2>/dev/null
ls -1 /sys/class/misc/uinput /sys/module/uinput 2>/dev/null
)
