#!/bin/bash
set -eu
pacman -S --needed --noconfirm meson ninja python-mako python-yaml python-packaging python-ply > /mnt/d/y5_linux_work/out/mesa-build-dependencies.log 2>&1
meson --version
ninja --version
