r"""Backup "na zywo" telefonu na PC przez USB RNDIS (172.16.42.1), przed eksperymentami z kernelem/BROM.
Uruchom NA PC (tam gdzie jest y5sh.py), telefon podlaczony jak zwykle:

    python backup_phone_live.py                 # male partycje + userdata (tar rootfs Alpine)
    python backup_phone_live.py --big           # dodatkowo system/vendor/cust/preas (duze, zwykle zbedne - masz mtkclient z 20.10)
    python backup_phone_live.py --raw-userdata  # userdata jako surowy obraz zamiast tar (11 GB)

Wynik: D:\y5_backups\<data_czas>\*.img / userdata_rootfs.tar.gz + SHA256SUMS.txt. Tylko ODCZYT z telefonu.
"""
import os, sys, socket, time, threading, hashlib, datetime, argparse
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import y5sh

PC_IP = "172.16.42.2"          # adres tego PC na lacu USB (telefon = 172.16.42.1)
OUT_ROOT = r"D:\y5_backups"

SMALL = ["boot_para", "para", "expdb", "frp", "nvcfg", "nvdata", "metadata", "protect1", "protect2", "seccfg", "sec1",
         "proinfo", "md1img", "md1dsp", "spmfw", "mcupmfw", "gz1", "gz2", "nvram", "tee1", "tee2", "oeminfo", "reserve0",
         "reserve1", "lk", "lk2", "loader_ext1", "loader_ext2", "misc", "bfr", "bootfail_info", "rrecord", "nff", "logo",
         "boot", "recovery", "erecovery", "odmdtbo", "flashinfo"]
BIG = ["cust", "vendor", "system", "preas", "preavs"]


def partmap():
    """PARTNAME -> /dev/mmcblk0pN, czytane z /sys na telefonie."""
    # /dev na telefonie jest statyczny: wezly blokowe partycji trzeba utworzyc (major:minor z sysfs), inaczej dd nie ma czego otworzyc.
    y5sh.run('for d in /sys/block/mmcblk0/mmcblk0p*; do n=$(basename $d); '
             '[ -b /dev/$n ] || mknod /dev/$n b $(cat $d/dev | tr : " "); done', timeout=30)
    out = y5sh.run('for d in /sys/block/mmcblk0/mmcblk0p*; do echo "$(basename $d) $(grep -m1 PARTNAME= $d/uevent | cut -d= -f2)"; done', timeout=20)
    m = {}
    for line in out.splitlines():
        parts = line.split()
        if len(parts) == 2 and parts[0].startswith("mmcblk0p"):
            m[parts[1]] = "/dev/" + parts[0]
    return m


def receive(port, dest, remote_cmd, timeout):
    """Otwiera gniazdo na PC, kaze telefonowi wyslac strumien (nc) i zapisuje do pliku; zwraca (bajty, sha256)."""
    srv = socket.socket()
    srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    srv.bind((PC_IP, port)); srv.listen(1); srv.settimeout(60)
    t = threading.Thread(target=lambda: y5sh.run(remote_cmd % port, timeout=timeout), daemon=True)
    t.start()
    h, n = hashlib.sha256(), 0
    conn, _ = srv.accept(); conn.settimeout(120)
    with open(dest, "wb") as f:
        while True:
            try:
                d = conn.recv(1 << 20)
            except socket.timeout:
                break
            if not d:
                break
            f.write(d); h.update(d); n += len(d)
    conn.close(); srv.close()
    return n, h.hexdigest()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--big", action="store_true")
    ap.add_argument("--raw-userdata", action="store_true")
    args = ap.parse_args()
    out = os.path.join(OUT_ROOT, datetime.datetime.now().strftime("%Y%m%d_%H%M%S"))
    os.makedirs(out, exist_ok=True)
    pm = partmap()
    print("partycji:", len(pm), "->", out)
    sums, failed, port = [], [], 3100

    names = SMALL + (BIG if args.big else [])
    for name in names:
        dev = pm.get(name)
        if not dev:
            print("  pomijam (brak):", name); continue
        dest = os.path.join(out, name + ".img")
        t0 = time.time()
        n, sha = receive(port, dest, "dd if=%s bs=1M 2>/dev/null | nc 172.16.42.2 %%d" % dev, 600)
        port += 1
        if n == 0:
            print("  !!! %-14s 0 B - BLAD (nic nie odebrano), plik pominiety" % name)
            failed.append(name)
            os.remove(dest)
            continue
        print("  %-14s %10d B  %5.1fs  %s" % (name, n, time.time() - t0, sha[:12]))
        sums.append((sha, name + ".img"))

    if args.raw_userdata:
        dest = os.path.join(out, "userdata.img")
        t0 = time.time()
        n, sha = receive(port, dest, "dd if=%s bs=4M 2>/dev/null | nc 172.16.42.2 %%d" % pm["userdata"], 3600)
        print("  userdata (raw) %d B %.0fs" % (n, time.time() - t0)); sums.append((sha, "userdata.img"))
    else:
        dest = os.path.join(out, "userdata_rootfs.tar.gz")
        t0 = time.time()
        cmd = ("cd /mnt/root && tar czf - --exclude=./proc --exclude=./sys --exclude=./dev --exclude=./run --exclude=./tmp "
               "--exclude=./aroot --exclude=./ro --exclude=./vendor . 2>/dev/null | nc 172.16.42.2 %d")
        n, sha = receive(port, dest, cmd, 1800)
        if n < 10_000_000:
            print("  !!! userdata tar tylko %d B - BLAD (rootfs powinien miec setki MB)" % n)
            failed.append("userdata_rootfs.tar.gz")
        else:
            print("  userdata tar   %d B %.0fs %s" % (n, time.time() - t0, sha[:12])); sums.append((sha, "userdata_rootfs.tar.gz"))

    with open(os.path.join(out, "SHA256SUMS.txt"), "w") as f:
        for sha, name in sums:
            f.write("%s  %s\n" % (sha, name))
    if failed:
        print("UWAGA - NIE ZAPISANO: %s. Backup NIEKOMPLETNY, nie traktuj go jako zabezpieczenia." % ", ".join(failed))
        sys.exit(1)
    print("gotowe (kompletne):", out)


if __name__ == "__main__":
    main()
