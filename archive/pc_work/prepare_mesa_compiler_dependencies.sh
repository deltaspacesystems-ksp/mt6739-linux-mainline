#!/bin/bash
set -eu
pacman -S --needed --noconfirm llvm clang libclc spirv-llvm-translator spirv-tools glslang > /mnt/d/y5_linux_work/out/mesa-compiler-dependencies.log 2>&1
