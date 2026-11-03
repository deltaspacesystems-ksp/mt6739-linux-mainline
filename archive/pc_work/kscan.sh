#!/bin/bash
cd /home/y5
[ -d linux-7.3 ] || tar -xf linux-7.3.tar.xz
cd linux-7.3
echo "wersja: $(make kernelversion 2>/dev/null)"
echo "--- dts mediatek (mt67*/mt65*/mt81* dtsi z 'mt6739|mt6765|mt6761|mt6763|mt6580|mt6797'):"
ls arch/arm64/boot/dts/mediatek | grep -E 'mt6(7|5|3|8)|mt65' | tr '\n' ' '; echo
echo "--- 'mt6739' w drzewie:"
grep -rIl -i 'mt6739' --include='*' . 2>/dev/null | head -n 20
echo "--- clk / pinctrl / power dla mt6765 i mt6739:"
ls drivers/clk/mediatek | grep -E '6739|6765|6761|6763|6580' | tr '\n' ' '; echo
ls drivers/pinctrl/mediatek | grep -E '6739|6765|6761|6763|6580' | tr '\n' ' '; echo
ls drivers/pmdomain/mediatek 2>/dev/null | tr '\n' ' '; echo
echo "--- DRM/simpledrm/musb/mmc:"
grep -rIl 'mediatek,mt6739\|mt6739' drivers/mmc drivers/usb drivers/gpu 2>/dev/null | head