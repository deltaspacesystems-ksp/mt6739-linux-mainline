#!/bin/sh
# Explicit, bounded captures on the PHONE. No auto-start, reboot or bus writes.
# Binary test data remains private on rootfs; never export it into Git.
set -eu
usage() {
    echo 'Usage: mfw_capture_test.sh --check'
    echo '       mfw_capture_test.sh --photo /dev/videoN'
    echo '       mfw_capture_test.sh --microphone hw:N,M'
}
case "${1:-}" in
--check)
    echo 'Capture nodes (absence means the kernel path is not ready):'
    for node in /dev/video* /dev/media* /dev/snd/pcm*C*c /dev/gnss*; do
        [ ! -e "$node" ] || printf '%s\n' "$node"
    done
    for command in timeout v4l2-ctl arecord; do
        command -v "$command" || true
    done
    exit 0;;
--photo)
    [ "$#" -eq 2 ] || { usage; exit 2; }
    case "$2" in /dev/video[0-9]*) ;; *) usage; exit 2;; esac
    [ -c "$2" ] || { echo 'No V4L2 capture node; no activation attempted' >&2; exit 1; }
    command -v v4l2-ctl >/dev/null
    command -v timeout >/dev/null
    kind=photo;;
--microphone)
    [ "$#" -eq 2 ] || { usage; exit 2; }
    # A hardware PCM must be selected explicitly; never choose a default route.
    case "$2" in hw:*) ;; *) usage; exit 2;; esac
    [ -d /dev/snd ] || { echo 'No ALSA card; no recording attempted' >&2; exit 1; }
    command -v arecord >/dev/null
    command -v timeout >/dev/null
    kind=microphone;;
*) usage; exit 2;;
esac
umask 077
mkdir -p /var/cache/mfw-hardware-private
chmod 700 /var/cache/mfw-hardware-private
trial=$(mktemp -d "/var/cache/mfw-hardware-private/$kind.XXXXXX")
echo "Private test directory: $trial"
if [ "$kind" = photo ]; then
    timeout 5 v4l2-ctl -d "$2" --get-fmt-video > "$trial/format.txt" 2> "$trial/errors.txt"
    timeout 12 v4l2-ctl -d "$2" --stream-mmap=3 --stream-count=1 --stream-to="$trial/frame.raw" >> "$trial/format.txt" 2>> "$trial/errors.txt"
    [ -s "$trial/frame.raw" ]
    wc -c < "$trial/frame.raw"
    echo 'Frame received; format-aware image inspection is still required. Bytes alone do not prove a working camera.'
else
    timeout 7 arecord -D "$2" -t wav -f S16_LE -r 48000 -c 1 -d 2 "$trial/microphone.wav" > "$trial/output.txt" 2> "$trial/errors.txt"
    [ -s "$trial/microphone.wav" ]
    wc -c < "$trial/microphone.wav"
    echo 'PCM capture completed; silence/clipping/signal analysis is still required. A WAV header alone is not success.'
fi
