#!/bin/bash
set -eu
cd /home/y5/linux-7.3
sed -n '1,240p' drivers/gpu/drm/tiny/arcpgu.c
sed -n '1,95p' include/drm/drm_simple_kms_helper.h
sed -n '1989,2014p' drivers/gpu/drm/drm_atomic_helper.c
