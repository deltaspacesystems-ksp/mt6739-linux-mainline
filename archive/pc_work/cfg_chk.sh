cd /home/y5/linux-7.3
grep -cE "^CONFIG_ARM64=y|^CONFIG_ARCH_MEDIATEK=y" .config
ls -l .config .config.old
grep -cE "^CONFIG_ARM64=y|^CONFIG_ARCH_MEDIATEK=y" .config.old
