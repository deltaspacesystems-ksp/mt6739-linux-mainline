#!/bin/sh
set -eu
export PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
case "$(uname -v)" in '#48 '*) ;; *) echo 'Wrong kernel'; exit 1;; esac
cd /tmp
printf '%s  %s\n' 6ce6658a329a00e00db939091f80afc9388f9f07d6a6a7dafcccf0a04cdc620b mfw-v62-modules.tar.gz | sha256sum -c -
tar -tzf mfw-v62-modules.tar.gz | awk '$0 !~ /^lib\// || $0 ~ /(^|\/)\.\.(\/|$)/ {bad=1} END {exit bad}'
tar -xzf mfw-v62-modules.tar.gz -C /
depmod -a 7.3.0
for name in industrialio bluetooth gnss wwan snd_soc_core; do
 echo "Loading framework: $name"
 modprobe "$name"
done
lsmod
