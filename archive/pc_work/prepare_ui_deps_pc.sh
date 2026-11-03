#!/bin/bash
set -eu
python /mnt/d/y5_linux_work/fetch_librsvg_pc.py
pacman -S --needed --noconfirm wayland cairo pango wayland-protocols weston ttf-dejavu labwc
