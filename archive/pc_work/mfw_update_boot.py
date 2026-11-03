"""Local USB boot updater. Default: read-only preflight; --stage: RAM + PC backup.

--write is an explicit, separate action. Never writes any other partition.
Private boot backups stay outside the repository and are never printed.
"""
import argparse
import datetime as dt
import hashlib
import json
from pathlib import Path
import re
import shlex
import socket
import struct
import subprocess
import sys
import zlib

WORK = Path(r"D:\y5_linux_work")
DEVICE = "/dev/mmcblk0p36"
SECTORS = 49152
REMOTE = "/tmp/mfw-update.img"


def validate_image(data):
    if not 2048 < len(data) <= SECTORS * 512 or data[:8] != b"ANDROID!":
        raise ValueError("Invalid boot magic or image size")
    ks, ka, rs, ra, ss, _, ta, page = struct.unpack_from("<8I", data, 8)
    if (ka, ra, ta, page, ss) != (0x40200000, 0x45000000, 0x44000000, 2048, 0):
        raise ValueError("Unexpected DURA-L21 boot layout")
    roff = page + ((ks + page - 1) // page) * page
    if not ks or not rs or roff + rs > len(data):
        raise ValueError("Truncated boot payload")
    kernel, ramdisk = data[page:page + ks], data[roff:roff + rs]
    digest = hashlib.sha1()
    for blob in (kernel, ramdisk, b""):
        digest.update(blob)
        digest.update(struct.pack("<I", len(blob)))
    if data[576:596] != digest.digest():
        raise ValueError("Boot payload SHA1 mismatch")
    dec = zlib.decompressobj(31)
    image = dec.decompress(kernel, 14 * 1024 * 1024 + 1)
    if not dec.eof or not 0 < len(image) <= 14 * 1024 * 1024:
        raise ValueError("Invalid or oversized kernel gzip")
    fdt = dec.unused_data
    if len(fdt) < 40 or fdt[:4] != b"\xd0\x0d\xfe\xed" or struct.unpack_from(">I", fdt, 4)[0] != len(fdt):
        raise ValueError("Invalid appended DTB")
    rd = zlib.decompressobj(31)
    rd_data = rd.decompress(ramdisk, 64 * 1024 * 1024 + 1)
    if not rd.eof or rd.unused_data or len(rd_data) > 64 * 1024 * 1024:
        raise ValueError("Invalid ramdisk gzip")
    return {"image_bytes": len(data), "sha256": hashlib.sha256(data).hexdigest(),
            "kernel_bytes": len(image), "dtb_bytes": len(fdt)}


def remote(script, timeout=45):
    from phone_shell import run
    out, rc, ok = run("sh -c " + shlex.quote(script), timeout)
    if not ok or str(rc) != "0":
        raise RuntimeError("Phone command failed or timed out; boot update not confirmed")
    return out


def field(output, name, pattern):
    values = re.findall(r"^MFW_" + name + r"=(" + pattern + r")\s*$", output, re.M)
    if len(values) != 1:
        raise RuntimeError("Missing/ambiguous phone field: " + name)
    return values[0]


def inspect_phone():
    out = remote(r'''set -eu
test "$(id -u)" = 0
test -b /dev/mmcblk0p36
printf 'MFW_PART=%s\n' "$(sed -n 's/^PARTNAME=//p' /sys/class/block/mmcblk0p36/uevent)"
printf 'MFW_SECTORS=%s\n' "$(cat /sys/class/block/mmcblk0p36/size)"
printf 'MFW_RO=%s\n' "$(cat /sys/class/block/mmcblk0p36/ro)"
printf 'MFW_ROOT=%s\n' "$(awk '$2=="/" {print $1}' /proc/mounts)"
printf 'MFW_SHA=%s\n' "$(sha256sum /dev/mmcblk0p36 | cut -d' ' -f1)"
''')
    if (field(out, "PART", r"[a-z0-9_]+"), field(out, "SECTORS", r"\d+"),
        field(out, "RO", r"[01]"), field(out, "ROOT", r"/dev/[a-z0-9]+")) != (
            "boot", str(SECTORS), "0", "/dev/mmcblk0p46"):
        raise RuntimeError("Unexpected partition, capacity, read-only state or rootfs")
    return field(out, "SHA", r"[0-9a-f]{64}")


def push(local, target, mode="600"):
    subprocess.run([sys.executable, str(WORK / "phone_push.py"), str(local), target,
                    "--port", "9500", "--mode", mode], check=True)


def stage(image, info, old_hash):
    # Explicit fixed private path; never use a repository or secrets directory.
    backups = WORK / "boot_update_backups"
    backups.mkdir(exist_ok=True)
    stamp = dt.datetime.now(dt.timezone.utc).strftime("%Y%m%dT%H%M%SZ")
    backup = backups / ("boot-before-" + stamp + ".img")
    if backup.exists():
        raise RuntimeError("Backup name already exists")
    remote("(nc -l -p 9502 < /dev/mmcblk0p36 > /dev/null 2>/tmp/mfw-backup.err &)\nsleep 1", 15)
    remaining = SECTORS * 512
    digest = hashlib.sha256()
    with socket.create_connection(("192.168.7.2", 9502), timeout=15) as sock, backup.open("xb") as dest:
        sock.settimeout(45)
        while remaining:
            chunk = sock.recv(min(65536, remaining))
            if not chunk:
                raise RuntimeError("Incomplete boot backup; retained for inspection")
            dest.write(chunk)
            digest.update(chunk)
            remaining -= len(chunk)
    if digest.hexdigest() != old_hash or inspect_phone() != old_hash:
        raise RuntimeError("Boot backup checksum mismatch or boot changed")
    push(image, REMOTE)
    uploaded = field(remote("printf 'MFW_UPLOAD=%s\\n' \"$(sha256sum /tmp/mfw-update.img | cut -d' ' -f1)\""),
                     "UPLOAD", r"[0-9a-f]{64}")
    if uploaded != info["sha256"]:
        raise RuntimeError("Staged image checksum mismatch")
    manifest = dict(info, old_boot_sha256=old_hash, backup=str(backup), image=str(image),
                    device=DEVICE, expected_sectors=SECTORS, staged=REMOTE)
    (backups / "latest.json").write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    (WORK / "out" / "mfw-write-boot.sh").write_text(
        build_write_script(manifest), encoding="utf-8", newline="\n")
    print("READY: verified 24 MiB private boot backup and RAM image; no boot write performed.")
    return manifest


def build_write_script(manifest):
    # A separate shell file avoids interactive EXIT-trap semantics.
    return f'''#!/bin/sh
set -eu
mkdir /run/mfw-boot-update.lock
trap 'rmdir /run/mfw-boot-update.lock' EXIT
test "$(sed -n 's/^PARTNAME=//p' /sys/class/block/mmcblk0p36/uevent)" = boot
test "$(cat /sys/class/block/mmcblk0p36/size)" = {SECTORS}
test "$(sha256sum {REMOTE} | cut -d' ' -f1)" = {manifest['sha256']}
test "$(sha256sum {DEVICE} | cut -d' ' -f1)" = {manifest['old_boot_sha256']}
dd if={REMOTE} of={DEVICE} bs=1048576 conv=fsync
sync
test "$(head -c {manifest['image_bytes']} {DEVICE} | sha256sum | cut -d' ' -f1)" = {manifest['sha256']}
echo MFW_WRITE_VERIFIED
'''


def write_boot(manifest):
    local = WORK / "out" / "mfw-write-boot.sh"
    local.write_text(build_write_script(manifest), encoding="utf-8", newline="\n")
    push(local, "/tmp/mfw-write-boot.sh", "700")
    out = remote("sh /tmp/mfw-write-boot.sh", 90)
    if not re.search(r"^MFW_WRITE_VERIFIED\s*$", out, re.M):
        raise RuntimeError("Write/readback not confirmed; do not reboot automatically")
    print("Boot write and complete image readback SHA256 verified.")


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("image", type=Path)
    mode = ap.add_mutually_exclusive_group()
    mode.add_argument("--check", action="store_true", help="read-only preflight (default)")
    mode.add_argument("--stage", action="store_true", help="backup to PC and upload image to RAM only")
    mode.add_argument("--write", action="store_true", help="explicit boot-only write after backup/checks")
    ap.add_argument("--reboot", action="store_true", help="reboot only after --write and verified readback")
    args = ap.parse_args()
    if args.reboot and not args.write:
        ap.error("--reboot requires --write")
    image = args.image.resolve(strict=True)
    info = validate_image(image.read_bytes())
    print("Image validated:", info["image_bytes"], "bytes; SHA256", info["sha256"])
    old_hash = inspect_phone()
    print("Phone preflight: rootfs p46; target boot p36, writable 24 MiB.")
    if args.stage or args.write:
        manifest = stage(image, info, old_hash)
        if args.write:
            write_boot(manifest)
            if args.reboot:
                from phone_shell import run
                print("Requesting reboot after verified boot update.")
                run("sync; reboot", 10)
    else:
        print("Read-only check complete; no upload or partition write.")


if __name__ == "__main__":
    main()
