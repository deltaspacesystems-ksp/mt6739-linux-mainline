#!/bin/bash
set -eu
cd /home/y5/linux-7.3
echo CONFIG
grep -E '^CONFIG_(MEDIA_SUPPORT|VIDEO_DEV|VIDEO_V4L2|SND|SND_SOC|IIO|I2C|GNSS|MODULES)=|^# CONFIG_(MEDIA_SUPPORT|SND|IIO|MODULES) is not set' .config || true
echo SENSOR_DRIVERS
find drivers/media/i2c -maxdepth 1 -type f | grep -Ei 'hi556|hi846|ov8856|ov13855|s5k3l6|gc5025|gc8034' || true
echo LTR_MATCHES
grep -nE '\.compatible|\.name = "ltr|#define LTR.*(MAIN_CTRL|PART_ID|ALS_DATA|PS_DATA|MANUFAC|STATUS)' drivers/iio/light/ltr501.c drivers/iio/light/ltrf216a.c
echo AUDIO_AND_CAPTURE_MT6739
grep -RnE 'mt6739|mt6357' sound/soc/mediatek sound/soc/codecs drivers/media/platform/mediatek | head -45 || true
echo MAINLINE_MT6357_AUDIO_FILES
find sound -iname '*6357*'
