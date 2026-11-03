#!/bin/sh
set -eu
export PATH=/usr/sbin:/usr/bin:/sbin:/bin
case "$(uname -v)" in '#48 '*) ;; *) exit 1;; esac
[ "$(cat /sys/module/pvrsrvkm/parameters/enable)" = N ]
[ "$(cat /sys/kernel/kexec_loaded)" = 0 ]
[ "$(cat /proc/sys/kernel/kexec_load_disabled)" = 0 ]
printf '%s  %s\n' 479064c2529c1d1b4adb1cc0acde1277843c8c9cdf650f42e8d972d668f2d54e /tmp/kexec-v62-Image 77d123dd73311f263dd971b93f6e6575f55fd86778d1eb2337a10fdcbec3e87c /tmp/kexec-v62-initramfs.gz | sha256sum -c -
set +e
kexec -s -l /tmp/kexec-v62-Image --initrd=/tmp/kexec-v62-initramfs.gz --reuse-cmdline > /tmp/mfw-kexec-load.log 2>&1
rc=$?
set -e
echo "kexec_load_rc=$rc"
printf 'kexec_loaded=';cat /sys/kernel/kexec_loaded
if [ "$rc" != 0 ]; then
 grep -E '^(kexec_file_load failed:|kexec_load failed:|Cannot allocate memory|Function not implemented|Invalid argument)' /tmp/mfw-kexec-load.log || true
 exit "$rc"
fi
echo 'LOAD_ONLY_PASS; no jump performed'
