#!/bin/bash
set -eu
cd /home/y5/linux-7.3
echo SIMPLE_PIPE_API
grep -nE 'struct drm_simple_display_pipe_funcs|\(\*enable|\(\*disable|\(\*update|\(\*check|prepare_fb|enable_vblank|disable_vblank|struct drm_simple_display_pipe' include/drm/drm_simple_kms_helper.h | head -45 || true
echo DMA_DRIVER_API
grep -nE 'DEFINE_DRM_GEM_DMA_FOPS|DRM_GEM_DMA_DRIVER_OPS|drm_fb_dma_get_gem_addr|struct drm_gem_dma_object' include/drm/drm_gem_dma_helper.h include/drm/drm_fb_dma_helper.h | head -38 || true
echo EXAMPLE_DRIVERS
find drivers/gpu/drm -name '*arcpgu*' -o -name '*pl111*'
echo COMMIT_TAIL
grep -nE 'void drm_atomic_helper_commit_tail|wait_for_flip_done|fake_vblank|commit_hw_done' drivers/gpu/drm/drm_atomic_helper.c | head -30 || true
