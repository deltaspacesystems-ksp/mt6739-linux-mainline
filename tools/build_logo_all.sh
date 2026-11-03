#!/bin/bash
# Buduje kompletna partycje logo microfw: logo (0), ladowanie/bateria (1-40) w stylu ASCII.
# Uzycie: tools/build_logo_all.sh ORYGINAL_logo.img WYNIK.img [KATALOG_ROBOCZY]
set -e
ORIG="$1"; OUT="$2"; WORK="${3:-/tmp/microfw_logo}"; D="$(dirname "$0")"
mkdir -p "$WORK"
python3 "$D/render_logo.py" "$D/../phone/logo/microfw_ascii.txt" "$WORK/logo0.raw" "$WORK/logo_preview.png"
python3 "$D/render_battery.py" "$WORK/bat" "$WORK/logo0.raw"
args=(--raw "0=$WORK/logo0.raw")
for i in $(seq 1 40); do f=$(printf "%s/bat/%02d.raw" "$WORK" "$i"); [ -f "$f" ] && args+=(--raw "$i=$f"); done
python3 "$D/make_logo.py" "$ORIG" "$OUT" "${args[@]}"
