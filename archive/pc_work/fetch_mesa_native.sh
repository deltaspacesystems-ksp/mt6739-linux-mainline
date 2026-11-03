#!/bin/bash
set -eu
repo=/home/y5/mesa-native
commit=2736dd1220d58192f5591ca6f9fcc5e60ee368ad
if [ ! -e "$repo" ]; then mkdir "$repo";git -C "$repo" init -q;fi
if ! git -C "$repo" cat-file -e "$commit^{commit}" 2>/dev/null; then
 GIT_TERMINAL_PROMPT=0 git -C "$repo" fetch --depth=1 https://chromium.googlesource.com/external/gitlab.freedesktop.org/mesa/mesa "$commit"
fi
test -z "$(git -C "$repo" status --porcelain)"
git -C "$repo" checkout --detach "$commit"
test "$(git -C "$repo" rev-parse HEAD)" = "$commit"
printf 'Mesa pinned source checkout ready\n'
