# V62: kexec and hardware modules

Status: Image #48, 41 modules, depmod, boot-layout/digest validation, private boot backup and RAM staging PASS. Phone remains v61. No boot write or restart has been performed. Owner allows one future restart; request concrete boot-write approval only after validation.

The kernel source is the complete v60 patch plus the v61-on-v60 source delta. V62 adds configuration changes only; use phone/kernel/config-7.3-mfw-v62. Build reproducibly using archive/pc_work/build_v62_hardware.sh on the existing WSL tree. All make invocations specify ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu-. Image build uses the existing background bld.sh with /home/y5/build73f.log; module build/install logs and staging are under D:/y5_linux_work/out/.

## Enabled functionality

- KEXEC_FILE and KEXEC_CORE, CPU hotplug for secondary CPU shutdown. No crash kernel or KEXEC_HANDOVER. The legacy KEXEC syscall is not selected by current platform dependencies; use file-syscall-capable kexec-tools.
- MODULES/MODULE_UNLOAD: future compatible native drivers can be loaded without rebuilding the boot image. This does not allow loading stock 4.4 binaries.
- Modular cfg80211/mac80211/rfkill, Bluetooth protocols, WWAN and GNSS frameworks. These do not implement MT6739 CONNSYS/BTIF/CCCI transports.
- Modular MediaTek I2C controller reference and IIO/MC3230 reference. Existing i2c-mt65xx.c has no exact mt6739 compatible; no forced DT binding. MC3230 is a reference, not a verified match for every MC3xxx variant. LTR577 has no exact mainline driver; LTR501/F216A are not substitutes.
- Modular V4L2/media, HI556/HI846/OV8856 candidates from stock camera tables, ALSA/ASoC and MT6357 codec. Actual camera identity, CSI/DMA pipeline and MT6739 audio AFE remain unresolved.
- Existing native built-in GPU driver retained, default binding disabled. No new GPU rendering capability is claimed.

## First test

Keep GPU disabled after the future v62 boot. Check USB, charging, buttons/display and kernel version before loading optional modules. Do not autoload all modules or force compatible strings. Verify kexec tools, uncompressed Image, correct DT/initramfs and shutdown behavior before any jump. The GPU platform has a shutdown callback, but its successful handover is untested. Test kexec only with owner present; it restarts Linux and may hang before USB returns. The read-only /usr/local/bin/mfw-kexec-check is already installed on v61; it loads nothing. Missing API/tool there is expected. No kexec command has been executed.

## DTB and CPU audit

Local 7.3 arch/arm64/kernel/machine_kexec_file.c calls of_kexec_alloc_and_setup_fdt. drivers/of/kexec.c clones initial_boot_params, updates chosen/initrd/command line and removes the old DT reservation. File-syscall boot therefore reuses the running boot DT; it is not a general path for testing a different DTB. Our first image keeps DT19 byte-identical. Dynamic CONNSYS reservation placement and continued display scanout need explicit testing; no successful kexec handover has been established. arm64 machine_shutdown calls smp_shutdown_nonboot_cpus; CPU hotplug is enabled for this path. No suspend feature is enabled just to obtain legacy KEXEC.

## Final artifacts

D:/y5_linux_work/mfw_v62.img: 9332736 bytes; Image 14184456 bytes; SHA256 197dc9dfa30ada4ffa94b1ceb34e66c4f6c2656c95ccb4b158439b6eb599df05. Module bundle: D:/y5_linux_work/out/mfw-v62-modules.tar.gz, 41 modules, SHA256 6ce6658a329a00e00db939091f80afc9388f9f07d6a6a7dafcccf0a04cdc620b. Build #48 SMP PREEMPT Mon Oct 26 09:00:02 CET 2026. Full vmlinux contains file-load syscall and arm64 kexec/CPU-shutdown symbols. All artifacts remain on PC except the inert staged boot image in phone RAM. No module loaded; no boot write/restart.

Signed Alpine native kexec-tools 2.0.32-r2 was verified with apk verify, dependencies simulated offline, then installed offline. Only --version/--help executed; -s file-syscall option confirmed. Provenance: https://pkgs.alpinelinux.org/package/v3.24/community/aarch64/kexec-tools . No APK binaries committed.


## 2026-10-26 — v62 startup and native module loading PASS
After owner reported booting, ping2/2 and RNDIS returned. Linux7.3.0 #48 confirmed at uptime77s. Charging/USBonline1, VBAT4250555uV,26.9C,CPU949999755Hz. NativeGPUdisabledN. kexec_loaded=0 and kexec_load_disabled=0; userspace present. No kexec load/jump or second restart performed. Verified module archive SHA256, installed41modules to rootfs, depmodPASS. Explicit modprobe of industrialio/bluetooth/gnss/wwan/snd_soc_core and dependencies PASS with Not tainted. No physical radio/sensor/codec driver forced; framework presence does not prove hardware works. No autoload setup added.


## 2026-10-26 — first actual kexec PASS
Owner explicitly requested test. v62/#48 native GPU N; kexec disabled0, loaded0. Uploaded exact uncompressed Image and initramfs extracted from validated v62 boot; checked both SHA256. kexec -s -l --initrd=... --reuse-cmdline returned0, loaded1. Command line kept only inside phone, not printed/exported. Then sync, remount rootfs ro (abort on failure), kexec -e. USB returned without any requested owner intervention. At first shell uptime36.56s, RAM trial files absent, loaded0, /chosen/linux,booted-from-kexec exists: definitive kexec boot marker. SMP4CPUs, rootfs rw, Charging USBonline1, CPU~950MHz, GPU N. No boot partition write in this test. Same kernel/DT only; active GPU, different kernel and visual panel behavior remain untested. No second jump attempted. Evidence phone/hardware/kexec-v62-trial.json. Archived scripts are fixed v62 trial scripts, not generic unattended updater; do not reuse for arbitrary images without changing validated hashes/version guards. Modules remain installed but not automatically loaded after reboot.
