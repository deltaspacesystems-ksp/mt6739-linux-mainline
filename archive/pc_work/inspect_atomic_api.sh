#!/bin/bash
set -eu
cd /home/y5/linux-7.3
sed -n '470,650p' drivers/gpu/drm/sysfb/simpledrm.c
sed -n '650,795p' drivers/gpu/drm/sysfb/simpledrm.c
sed -n '1,110p' include/drm/drm_fb_dma_helper.h
grep -nE 'drmm_universal_plane_alloc|drmm_crtc_alloc_with_planes|drmm_encoder_alloc' include/drm/drm_plane.h include/drm/drm_crtc.h include/drm/drm_encoder.h
