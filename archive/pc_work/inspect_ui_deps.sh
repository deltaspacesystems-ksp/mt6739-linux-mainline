#!/bin/bash
set -eu
for pkg in wayland-client cairo pangocairo librsvg-2.0 wlroots-0.19 wlroots-0.20; do
 if pkg-config --exists "$pkg" 2>/dev/null; then printf '%s ' "$pkg"; pkg-config --modversion "$pkg"; else printf '%s missing\n' "$pkg"; fi
done
command -v wayland-scanner || true
command -v weston || true
ls /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml /usr/share/wlr-protocols/unstable/wlr-layer-shell-unstable-v1.xml 2>/dev/null || true
