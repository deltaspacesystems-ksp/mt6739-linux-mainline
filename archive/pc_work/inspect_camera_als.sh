#!/bin/bash
set -eu
cd /home/y5/linux-7.3
grep -RnE 'ltr577|ltrf216|ltr501|ltr559|hi556|hi846|ov8856|ov13855|s5k3l6|gc5025|gc8034|mt6739' drivers/iio/light drivers/media/i2c drivers/media/platform/mediatek Documentation/devicetree/bindings/iio/light | head -110
