#!/bin/sh
set -eu
cd /tmp/mfw-graphics64
cat <<'SUMS' | sha256sum -c -
136181faf0467b5a52f3974e5c02e532b5989d11cc8918865f4f336b27aa7250  general-ppp.bin
d3efb10ae2c71266b4d95246f74b3830d925347bc6f19278e9a54e2ab935e367  general-vdm.bin
d0355c74a6e6ddc04f8f320c5a4a4b204d905b89937468fc3d04ab1c8f7e41a5  pbe-state.bin
af38210ca047eaf39ac49aea5755c29565e811d899b69b272ab705a489e1dd85  pds-context.bin
d4e99af22e12ced668c6ba1d1a4429fd5e611a44c95365b79b4beb1547fb0c98  pds-main.bin
e160fd2e1138652f7229177e7dd7acfd56688366a64eb9236f3402e553e94e01  ta-context.bin
b94504d0f03444cc9ca6c82c8b0bb8089ac6ad7c072d1595a4941d2a9cfb6a92  usc-context.bin
5847575ff0e8303220bd04d37e8a8a1d922b8a6f3c4ab2ed9d99fa7989924366  usc-main.bin
SUMS
cc -std=c11 -O2 -Wall -Wextra -Werror /tmp/graphics19_stage_trial.c -o /tmp/graphics19-stage-trial
/tmp/graphics19-stage-trial
cat /sys/kernel/debug/pvr/status
