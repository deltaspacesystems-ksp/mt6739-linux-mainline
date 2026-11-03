> Bieżący potwierdzony obraz: mfw_v53.img (#39), configv51/DTv14, pełny patchv53. Aktualizacja USB działa z backupem i readback; restart możliwy, gdy właściciel jest przy telefonie (krótkiePower na LK). Nowy kod DE: phone/ui. Aktualne ograniczenia i wyniki: docs/NIGHT_WORK_2026-10-25.md.

> Aktualizacja 2026-10-24: wd_v49.img przeszedł test na urządzeniu i jest bieżącym potwierdzonym obrazem. Szczegóły: docs/PMIC_V49_TEST.md. v48 pozostaje awaryjny.

# Budowanie i wgrywanie (kernel, DTB, obraz boot)

Wszystko na PC z WSL (distro `archlinux`), sterowane z chmury mostem. Zagnieżdżone cudzysłowy psują się: pisz skrypty, `pc-put`, uruchamiaj.

## 1. Kernel 7.3 od zera (gdyby drzewo zniknęło)
```bash
cd /home/y5
tar xf linux-7.3.tar.xz && cd linux-7.3
patch -p1 < /mnt/d/y5_linux_work/export/y5-kernel-7.3.patch      # = phone/kernel/y5-kernel-7.3.patch w repo (sprawdzony: dry-run rc=0)
cp /mnt/d/y5_linux_work/export/config-7.3-y5 .config              # = phone/kernel/config-7.3-y5
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- olddefconfig     # ZAWSZE z ARCH=arm64 (inaczej .config traci symbole arm64)
nohup bash /mnt/d/y5_linux_work/bld.sh >/dev/null 2>&1 &          # bld.sh: yes "" | make ... -j6 Image > /home/y5/build73f.log; na koncu 'done'
```
Toolchain: `aarch64-linux-gnu-gcc` (Arch). Pełny build trwa kilkanaście–kilkadziesiąt minut (CPU 100%); przyrostowy kilka minut.
Po zmianie symbolu: `scripts/config --enable X` -> `make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- olddefconfig` -> sprawdź `grep -c '^CONFIG_ARM64=y' .config` = 1.
Kluczowe symbole: `PHY_MTK_TPHY`, `USB_MUSB_MEDIATEK`, `USB_MUSB_GADGET`, **`MUSB_PIO_ONLY`**, `USB_CONFIGFS`(+`_RNDIS/_ECM/_NCM/_ACM`), `CONFIGFS_FS`, `MMC_MTK`, `REGULATOR_FIXED_VOLTAGE`, `DEBUG_FS`, `DEVMEM` (`STRICT_DEVMEM` wyłączone), `MEDIATEK_WATCHDOG`, `INITRAMFS_SOURCE=""`. `USB_ETH` wyłączone (Windows przypinał COM).
Wynik: `/home/y5/linux-7.3/arch/arm64/boot/Image` (ok. 12,7 MB) -> kopiuj do `D:\y5_linux_work\out\Image_wdNN`.

## 2. DTB
`out\stock_0.dts` (zrzut stockowego DTB) + `extras_vN.dtsi` (nasze węzły: framebuffer, wdt, mmc, usb, t-phy, fixed-clocki, regulatory) sklejane `cat`, kompilowane `dtc` w WSL:
```bash
bash /mnt/d/y5_linux_work/build_dtbv9.sh      # -> out/v9.dtb (aktualny, dobry)
```
`phone/stub/build_dtbv9.sh` kopiuje `extras_v9.dtsi` z `C:\Users\pc\Documents\y5_linux_work\`. Ostrzeżenia `unit_address_vs_reg` w stockowych węzłach są normalne.

## 3. Narzędzia na telefon (userland, statyczne)
`phone/stub/build_tools.sh` (flagi: `-static -nostdlib -O2 -ffreestanding -fno-tree-loop-distribute-patterns`): `out\y5txt`, `out\y5cal`, `out\wdt`. Czcionka: `python3 mkfont.py` -> `font8x16.h` (wymaga źródeł kernela).

## 4. Składanie obrazu boot
```powershell
cd D:\y5_linux_work
$env:Y5_IMAGE="D:\y5_linux_work\out\Image_wd19"; $env:Y5_DTB="D:\y5_linux_work\out\v9.dtb"; $env:Y5_OUT="D:\y5_linux_work\wd_v41.img"
python bm_wd41.py      # Image.gz-dtb + ramdisk (Alpine + init + narzedzia) w nagłówku wzorowanym na D:\huawei_y5_backup\boot.bin
```
`bm_wdNN.py` (najnowszy `bm_wd41.py`) ma `INIT` (skrypt init) w środku: to tu zmienia się zachowanie telefonu po starcie. Kolejne wersje = kopia poprzedniej z poprawką.
Obrazy `*.img` **nie** są w repo (duże; budowane lokalnie).

## 5. Wgranie (robi właściciel, BROM)
```
cd D:\mtkclient_work
python mtk.py w boot D:\y5_linux_work\wd_v41.img
```
Przywrócenie Androida: `python mtk.py w boot D:\y5_backups\20261023_201905\boot.img`.
Po wgraniu telefon uruchamia się sam; ekran LK z czerwonym napisem to norma. Kernel pokazuje tekst po ok. 20–30 s. Kolejność oczekiwana dla v41: tekst diagnostyczny na ekranie + karta RNDIS na PC.

## 6. Diagnostyka na ekranie (wd_v38+)
Góra: dmesg przefiltrowane (`mmc|musb|phy|usb|udc|clk|depend|defer|fail|error|warn|oops|bug`). Środek: `udc:`, `net:`, `mmc:`, `blk:`, `def:` (devices_deferred),
`dev <adres>: drv=<sterownik>`, `drivers:`. Dół: ostatnie 28 linii dmesg. Ekran 90x90 znaków (czcionka 8x16), wiersze zawijane po 90 znakach.

## 7. Eksport zmian z powrotem do repo
Patch kernela: `diff -u` plików zmienionych względem `linux-7.3.tar.xz` (skrypt jak `exp_k.sh` w `archive/pc_work/`). Skrypty z PC: `D:\y5_linux_work\*` (repo ma ich archiwum w `archive/pc_work/`).
Przed `git add` przeskanuj pod kątem sekretów i danych urządzenia (IMEI, MAC, numery seryjne, tokeny mostu).

## Kandydat v49: PMIC i przyciski (Codex, 2026-10-24)
Aktualnie potwierdzony jest v48; v49 oczekuje na ręczny test właściciela. Pełna procedura, ograniczenia i pomiary: docs/PMIC_BATTERY_BUTTONS.md. Nowy pełny patch phone/kernel/y5-kernel-7.3-pmic.patch stosować na czystym 7.3 zamiast poprzedniego patcha; config-7.3-y5-pmic, zawsze ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu-. Kernel out/Image_wd23, DT v12, builder bm_wd49.py, obraz wd_v49.img (limit boot 24 MiB).
Samo nohup z krótkiej sesji WSL nie utrzymało tutaj budowania. Uruchamiać supervise_pmic_build.sh w długiej sesji procesu wsl.exe i pozostawić ją aktywną do końca (narzędzie exec oddaje session_id). Skrypt używa nohup bash bld.sh oraz wait; log nadal /home/y5/build73f.log. Sprawdzać datę Image i symbole sterownika, nie samo stare done. Skrypty bash i wejścia kernela mają wymuszone LF w .gitattributes.

Kandydat v50 (NTC; jeszcze bez BROM): wd_v50.img, Image_wd24, v12.dtb bez zmian, bm_wd50.py. Pełny patch alternatywny phone/kernel/y5-kernel-7.3-v50.patch, config jakv49. Źródła, testy i ładowarka HL7005D: docs/PMIC_V50_TEMPERATURE.md. v49 pozostaje potwierdzony.

Aktualizacja v51: nowe nazwy mfw_vNN.img. Potwierdzony v50, kandydat D:\y5_linux_work\mfw_v51.img (Image_mfw_v51, v13.dtb, bm_mfw_v51.py). Pełny patch/config: phone/kernel/mfw-kernel-7.3-v51.patch, config-7.3-mfw-v51; nakładka mfw-v51.dtsi. Aktualizator tools/mfw_update_boot.py: check/stage przetestowane, write oczekuje na zgodę. Kopie boot prywatnie poza repo. Procedura: docs/MFW_V51_UPDATES.md.

Potwierdzenie2026-10-25: mfw_v51.img wystartował (#35). Bieżący obraz: D:\y5_linux_work\mfw_v51.img; awaryjny wd_v50.img. USB updater i readback działają, ale reboot starego kernela zawisł i wymagany był Power. Nowe sysfs ładowarki i CPU działają. Szczegóły: docs/MFW_V51_UPDATES.md.

Kandydat restartu v52: mfw_v52.img, Image_mfw_v52, v14.dtb, bm_mfw_v52.py; pełny patch mfw-kernel-7.3-v52.patch, config jakv51. Staging zweryfikowany, zapis/boot/test jeszcze nie. Ważny test dwuetapowy: docs/MFW_V52_RESTART.md.

## V58: ograniczony test scanoutu DMA (jeszcze oczekuje na zapis)
Obraz D:\y5_linux_work\mfw_v58.img, build#43. Zapis/restart tylko po zgodzie wlasciciela; backup i staging juz wykonane. Pelny patchv58 jest alternatywa, nie nakladac na pelnyv57.
Po zgodzie: `python D:\y5_linux_work\mfw_update_boot.py D:\y5_linux_work\mfw_v58.img --write --reboot`; sprawdzic pelny readback. Krotko Power na LK, potwierdzic uname#43, flip_state0 i licznikiRDMA. Jesli brak sieci, zapytac o ekran; nie zgadywac startu.
Tylko przy wlascicielu patrzacym na panel: brak aktywnego seatd/compositora i lease dotyku; skrypt archive/pc_work/run_dma_flip_trial.sh (kopia /var/cache/mfw-ui) robi odliczanie5s, wyzwala kernelowy test zielony5s->magenta5s->oryginal, zapisuje status/log. Wylacznie sysfsflip_test, zadnych komend /dev/mem/DSI. Oczekiwana ocena panelu i osobno kernelowe return/readback/licznikiunderflow/abnormal. Kernelowy wynik0 bez fizycznej zmiany koloru NIE dowodzi latch. Bufory testowe pozostaja zaalokowane do reboot dla ochrony przed zwolnieniem w trakcie scanoutu. Test nie jest jeszcze KMS/GPU ani gotowym zerocopycompositorem.

Wynik po akceptacji: v58#43 zapis/readback/start PASS. TestDMA result0/restore0, adresy0x44800000/0x44c00000, powrot775eb000, underflow0/abnormal0. Wlasciciel potwierdzil oba kolory i poprawny powrot. Status "oczekuje" powyzej jest historyczny. Aktualne dowody: phone/hardware/display-dma-trial-v58.json.

## V59 GPU identity bringup (przygotowany, jeszcze bez zapisu)
Po zgodzie wlasciciela: `python D:\y5_linux_work\mfw_update_boot.py D:\y5_linux_work\mfw_v59.img --write --reboot`; readback musiPASS, potemPowernaLK. Potwierdzicuname#44 oraz /sys/bus/platform/devices/1000d000.pwrap/vcore_status i /sys/bus/platform/devices/10006000.mfw-gpu-id-probe/identity. W stanieidleGPUnie jest wlaczane, identityresult-EAGAIN. OdczytVCORE ma miec enabled1 i1050000..1312500uV; nizsze napiecie zatrzymuje test, NIE zmieniacnapieciaCPU/VCORE.
Po jawnej zgodzie na ten test, bez GUI i touchlease: /var/cache/mfw-ui/run_gpu_id_trial.sh (z archive/pc_work). Prober sam serializuje odczytyregulatora/PMIC i chroniPMIC przedunbind. Przy timeout/bladziepower latchedfault i zachowanieclockjezeliONniepewne; nie powtarzac testu na sile. Skrypt nie zlecareboot ani niepisze partycji. WynikIDrozpatrywac razemzrawrejestrami i poprawnympowerdown; nie traktowac go jako testuakceleracji.

V59 po zgodzie uruchomiony#44: pelnyreadback/startPASS. VCORE1150000uV/selector101/enabled1,read-idprobePASS ->HW22.49.21.16. PowerdownobuMFG/gateOFFpotwierdzonyniezaleznie,PLL/VCOREunchanged. Pozostale statusy "przygotowany" powyzejsahistoryczne. Nie traktowac udanegoprobejakoakceleracji; niepowtarzac bezcelu. Dowody phone/hardware/gpu-hwid-v59.json.

## V60 native GPU (2026-10-26, prepared; NOT written)

`D:\y5_linux_work\mfw_v60.img`, kernel #45, Image13719560B, boot9089024B,
SHA256 `ab33cc6a26370fd8512d5bf700886fb15bf5f5af42d4fbf4c09d7e4f97334092`.
Full627-file self-contained source patch `phone/kernel/mfw-kernel-7.3-v60.patch`
dry-runs on pristine7.3 PASS; separate `config-7.3-mfw-v60`. Build used nohup bld.sh,
ARM64/CROSS_COMPILE for olddefconfig and Image. Integrated compile caught three
old-style definitions not caught by the isolated build; fixed with `(void)`.
Config delta only DRM_MFW_ROGUE_DDK19=y. Initramfs/cmdline/addresses identicalv59.
DT19 adds nativeGPU SPI226/low/resources and disables old ID probe; no other delta.
New driver defaults disabled. Full boot backup24MiB + RAM staging PASS, no boot write.

After explicit owner approval only: `python D:\y5_linux_work\mfw_update_boot.py D:\y5_linux_work\mfw_v60.img --write --reboot`.
Then short Power on LK and verify uname#45, charging, USB and default-unbound GPU.
Do not run the old ID probe: it is intentionally disabled to establish one owner.
Runtime test source `archive/pc_work/run_v60_native_firmware_trial.sh`: upload to RAM
and execute as a file with `sh`, not phone_shell --file (EXIT trap must cover a real
script process). It checks build/VCORE/noGUI, reads only rgx.fw.signed from vendorp41
ro,noload, verifies pinned size/SHA, copies firmware to tmpfs, then explicitly binds
native driver once via module enable + platform drivers_probe. Unbind attrs remain
suppressed. The old ID probe must be absent. Fault/failed attempt: no blind retry;
inspect and reboot with owner if necessary. No NVRAM, voltage or PLL writes.

IMPORTANT lifecycle: DDK starts firmware in the FIRST render-node OPEN, not merely
at bind. Inspect bind logs, then explicitly run native_probe --connect on the render
node whose device is 13000000.gpu. That open/CONNECT/DISCONNECT is the firmware test,
not a read-only version query; no rendering jobs yet. Upload the native executable
again after reboot (/tmp is RAM). Firmware stays in tmpfs only, no Android runtime.
Shader rendering/native Mesa1.9 bridge integration remains pending. Procedures and
limitations: docs/GPU_DDK19_PORT.md. Phone stillv59 until approved write/readback.


## V61 prepared (2026-10-26; phone v60)
V60 write/readback/start/bind PASS. First render OPEN blocked by mandatory DRM7.3
fops unsigned-offset flag before firmware, not a GPU firmware failure.
V61#46 adds only that flag. Use completev60 source patch + v61-on-v60 delta;
config-7.3-mfw-v60 and DT19 unchanged. Boot/initramfs/config/DTvalidator and exact
source-delta apply/reproduction PASS. Private24MiBbackup+RAMstaging PASS.
After explicit approval: `python D:\y5_linux_work\mfw_update_boot.py D:\y5_linux_work\mfw_v61.img --write --reboot`.
ShortPoweronLK, verify#46, upload nativeCprobe and run_v61_native_firmware_trial.sh
toRAM. Execute scriptasfile, inspectbind, thenexplicitnativeprobe--connect; this
firstOPENstartsfw. No renderingclaim. No repeatedopenifFW/powerfault occurs.
Fallbackfullyvalidatedimage remainsv59; v60hasdocumentedopenblocker.


## 2026-10-26 — kexec feasibility and live hardware status
Owner asked about replacing the kernel without a hardware restart. kexec can boot another kernel without returning through LK, but restarts Linux/userspace and requires working shutdown/handover for this board. Current v61 configuration has CONFIG_KEXEC_FILE disabled; live phone has no /sys/kernel/kexec* or kexec_load_disabled sysctl and no kexec command. CONFIG_MODULES is disabled too. Cannot enable these simply by installing a userspace package. A future boot with kexec support, DT/initramfs handling, CPU shutdown and GPU/display/USB/charger handover testing is required, with owner present. No kexec, reset or boot write attempted. Reference: https://github.com/torvalds/linux/blob/master/arch/arm64/kernel/machine_kexec.c
Added tools/mfw-hw-status and deployed /usr/local/bin/mfw-hw-status; explicit full path required by current telnet PATH. Read-only allowlisted snapshot, no IDs/scanning/device opens. Syntax and live execution PASS: Charging, 4250225 uV, 29.2 C, CPU 949999755 Hz, GPU/FW OK HWR/CRR0; no I2C/IIO/video/PCM/HCI/wiphy/remoteproc/GNSS devices. Battery percentage unavailable, not estimated. This is availability/status reporting, not functional validation.


## 2026-10-26 — v62 ready: kexec and native hardware module foundations
Owner requested kexec and hardware support, permits one future restart. Built #48: KEXEC_FILE, HOTPLUG_CPU, MODULES; modular IIO/I2C/MC3230, cfg80211/mac80211/BT/WWAN/GNSS, V4L2/camera references and ALSA/MT6357. Existing native GPU retained disabled by default. 41 modules, depmod and boot validator PASS; Image14184456, boot9332736. DT19/initramfs/cmdline/addresses byte-identical v61. Signed native Alpine kexec-tools installed, version/help PASS; no kernel load/jump. Private24MiB backup and RAM staging PASS, boot NOT written, phone stillv61. Framework configuration does NOT prove functioning radios/cameras/sensors/audio/acceleration. Missing MT6739 drivers and transport remain. Full details docs/MFW_V62_KEXEC_HARDWARE.md, evidence phone/hardware/mfw-v62-validation.json. Await concrete v62 boot-write approval under owner rule9; no speculative reboot.


## 2026-10-26 — standing authorization for MICROFW updates
Owner explicitly authorized: "jak cos to juz mozesz wgrywac kernel i wszystko bez mojej zgody". This grants standing authorization for subsequent MICROFW kernel/boot updates, native driver/userland deployment and associated restart/kexec tests; do not request per-version boot-write consent again. It supersedes the earlier rule9 requirement for per-update confirmation within this established update scope. Existing partition boundaries remain: boot p36 and microfw rootfs p46 only. No authorization to format partitions, change GPT, write protected partitions/NVRAM, read secrets or publish/private-data exports. Preserve verified private boot backup, image validation and full readback before restart. Any future owner instruction prohibiting restarts takes precedence.
Kexec API and module loading are proven in v62; kexec handover and unattended update recovery are NOT yet proven. Do not claim automatic LK bypass until measured. Current owner wording permits update/restart testing; no new restart executed in this entry.


## 2026-10-26 — first actual kexec PASS
Owner explicitly requested test. v62/#48 native GPU N; kexec disabled0, loaded0. Uploaded exact uncompressed Image and initramfs extracted from validated v62 boot; checked both SHA256. kexec -s -l --initrd=... --reuse-cmdline returned0, loaded1. Command line kept only inside phone, not printed/exported. Then sync, remount rootfs ro (abort on failure), kexec -e. USB returned without any requested owner intervention. At first shell uptime36.56s, RAM trial files absent, loaded0, /chosen/linux,booted-from-kexec exists: definitive kexec boot marker. SMP4CPUs, rootfs rw, Charging USBonline1, CPU~950MHz, GPU N. No boot partition write in this test. Same kernel/DT only; active GPU, different kernel and visual panel behavior remain untested. No second jump attempted. Evidence phone/hardware/kexec-v62-trial.json. Archived scripts are fixed v62 trial scripts, not generic unattended updater; do not reuse for arbitrary images without changing validated hashes/version guards. Modules remain installed but not automatically loaded after reboot.
