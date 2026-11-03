(
for n in rstoutput0 rstoutput1 eint0default eintoutput0 eintoutput1; do
 p=/sys/firmware/devicetree/base/pinctrl@1000b000/$n/pins_cmd_dat/pins
 [ ! -f "$p" ] || { printf 'PINS_%s=' "$n"; od -An -v -tx1 "$p"; }
done
for n in pinctrl-0 pinctrl-1 pinctrl-2 pinctrl-3 pinctrl-4 pinctrl-5 vtouch-supply; do
 printf 'TOUCH_%s=' "$n"; od -An -v -tx1 /sys/firmware/devicetree/base/touch/$n
done
printf 'PINCTRL_NAMES='; tr '\000' ' ' < /sys/firmware/devicetree/base/touch/pinctrl-names; echo
find /sys/firmware/devicetree/base/mt_pmic_regulator -maxdepth 2 -type d
)
