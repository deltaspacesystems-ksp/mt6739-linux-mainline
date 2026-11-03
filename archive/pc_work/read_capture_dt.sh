#!/bin/sh
# Only named hardware properties; excludes chosen, serials and calibration.
base=/sys/firmware/devicetree/base
for node in i2c@11008000/alsps@53 i2c@11008000/cust_alsps@0 i2c@11008000/cust_alsps@1 i2c@11009000/camera_main@43 i2c@11009000/camera_sub@41 i2c@11009000/camera_main_w@42 i2c@11009000/camera_sub_w@40 audio@11220000 audio_sram@11221000 seninf@15008000 kd_camera_hw1@15008000 gps; do
    [ -d "$base/$node" ] || continue
    echo "NODE $node"
    for prop in compatible reg status clocks clock-names pinctrl-0 pinctrl-1 pinctrl-names interrupts interrupt-parent vdd-supply vddio-supply avdd-supply dvdd-supply dovdd-supply clock-frequency i2c_num polling_mode_als polling_mode_ps als_level als_value ps_threshold_high ps_threshold_low; do
        [ -f "$base/$node/$prop" ] || continue
        printf '%s=' "$prop"
        case "$prop" in
            compatible|status|clock-names|pinctrl-names) tr '\000' ' ' < "$base/$node/$prop"; echo;;
            *) od -An -v -tx1 "$base/$node/$prop" | tr -d '\n'; echo;;
        esac
    done
done
