#!/bin/bash
set -eu
cd /home/y5/linux-7.3
grep -R -n 'mt6739' drivers/media/platform/mediatek sound/soc/mediatek || true
grep -R -n 'compatible = ' drivers/media/platform/mediatek/vcodec | head -22
grep -R -n 'HEVC\|H264' drivers/media/platform/mediatek/vcodec/decoder/vdec/vdec_h264_if.c drivers/media/platform/mediatek/vcodec/decoder/vdec/vdec_hevc_req_multi_if.c | head -10
