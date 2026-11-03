> Bieżący potwierdzony obraz: mfw_v53.img (#39), configv51/DTv14, pełny patchv53. Aktualizacja USB działa z backupem i readback; restart możliwy, gdy właściciel jest przy telefonie (krótkiePower na LK). Nowy kod DE: phone/ui. Aktualne ograniczenia i wyniki: docs/NIGHT_WORK_2026-10-25.md.

> Aktualizacja 2026-10-25: bieżący potwierdzony mfw_v52.img; v53 to kandydat obsługi ładowania, pełny patch22plików i procedura w docs/MFW_V53_CHARGING.md. Skrypty odtwarzania w archive/pc_work; DTv14 i configv51 bez zmian.

> Aktualizacja 2026-10-24: wd_v49.img przeszedł test na urządzeniu i jest bieżącym potwierdzonym obrazem. Szczegóły: docs/PMIC_V49_TEST.md. v48 pozostaje awaryjny.

# microfw / y5linux: jak to działa i gdzie co leży

**microfw** = nazwa systemu: Linux 7.3 (mainline, nasz patch) + userland Alpine, na Huawei Y5 2018 (MT6739, DURA-L21).
Repo/projekt nazywa się `y5linux`. Dokument opisuje stan na 2026-10-24 (noc), obraz `wd_v44.img` (POTWIERDZONY na urządzeniu: rootfs z flash działa).

## 1. Jak to działa (w skrócie)

```
BROM (PC, mtkclient)  --zapis-->  partycja `boot` telefonu (p36)  =  obraz Android-boot (nagłówek stockowy):
    kernel Image.gz + DTB (v9.dtb)  +  ramdisk (initramfs: Alpine + init + narzędzia)

Start:  preloader -> LK (czerwony ekran "failed verification" = normalny) -> kernel 7.3 -> initramfs /init:
    1. montuje /dev /proc /sys, wyłącza watchdog (wdt x 0x10007000 0x22000000)
    2. tworzy gadżet USB z configfs: RNDIS + MS OS descriptors (microfw, VID 1d6b:0104), usb0 = 192.168.7.2/24
    3. udhcpd (PC dostaje 192.168.7.10-20) + telnetd na 192.168.7.2:23 (BEZ HASŁA, tylko test)
    4. jeśli userdata (mmcblk0p46) ma etykietę `microfw` i istnieje /etc/y5/ready -> mount + switch_root -> /sbin/init (busybox init)
       -> /etc/inittab -> /etc/y5/rc.sh (sieć, telnet) + /etc/y5/diag.sh (tekst diagnostyczny na ekranie co 4 s)
    5. w przeciwnym razie pętla diagnostyczna z initramfs (ekran)
Ekran: własny renderer `y5txt` pisze do skanowanego bufora (0x775E8E38, 720 px, XRGB8888) i ręcznie wyzwala DSI (MUTEX/RDMA/DSI).
Dostęp:  PC (192.168.7.20) --telnet--> telefon (192.168.7.2).  Agent w chmurze --most--> PC --telnet--> telefon.
```

## 2. Pętla pracy (każda zmiana kernela/initramfs)
1. Zmiana w drzewie kernela (WSL) albo w skrypcie obrazu `bm_wdNN.py` (zawiera INIT).
2. `bld.sh` (kernel) -> `out\Image_wdNN`; `bm_wdNN.py` -> `wd_vNN.img`.
3. **Właściciel** wgrywa przez BROM: `python mtk.py w boot D:\y5_linux_work\wd_vNN.img`.
4. Obserwacja: ekran telefonu (zdjęcie) lub telnet po USB.
Zmiany tylko w rootfs (userland) **nie wymagają BROM**: pliki idą przez USB (`tools/phone_push.py`), polecenia przez `tools/phone_shell.py`.

## 3. Gdzie co leży

### 3.1 Repozytoria
| Co | Gdzie |
|---|---|
| Repo prywatne (bez sekretów) | GitHub `deltaspacesystems-ksp/y5linux`, gałąź `main` |
| **Repo lokalne z kluczami** | `X:\y5linux` (to dysk sieciowy `\\<server>\x`; bez remote, hook blokuje push; klucze w `X:\y5linux\secrets\`, w tym `secrets.env`) |
| Roboczy klon w sesji chmurowej | `/home/user/y5linux` (znika po sesji) |
Synchronizacja GitHub -> X: jednokierunkowo, np. `git fetch <url GitHub> main` w `X:\y5linux` (wymaga logowania do prywatnego repo). Nie dodawaj remote ani nie pushuj z `X:\y5linux`.

### 3.2 Dokumentacja (ta sama treść w trzech miejscach)
Repo: `README.md`, `docs/{HANDOFF,BUILD_AND_FLASH,KLUCZE_I_ZMIENNE,MAPA_PLIKOW}.md`, `phone/NOTES.md` (dziennik), `phone/hw_inventory_73.md`, `phone/kernel73_boot_notes.md`.
Kopia na dysk D: `D:\y5_linux_work\DOKUMENTACJA\`. Prompt dla Codexa: `C:\Users\pc\Desktop\PROMPT_CODEX_microfw.txt`.

### 3.3 PC z telefonem i WSL
| Rzecz | Ścieżka |
|---|---|
| Katalog roboczy (wszystko) | `D:\y5_linux_work\` |
| Binaria/obrazy częściowe | `D:\y5_linux_work\out\` (`Image_wd20` = aktualny kernel, `v9.dtb`, `v9.dts`, `stock_0.dts`, `y5txt`, `y5cal`, `wdt`, `busybox-extras`) |
| Obrazy do wgrania | `D:\y5_linux_work\wd_v*.img` (76 szt., **aktualny: `wd_v44.img`**, awaryjny bez switch_root: `wd_v43.img`) |
| Skrypty składania obrazów | `D:\y5_linux_work\bm_wd*.py` (najnowszy `bm_wd44.py`; zmienne `Y5_IMAGE`, `Y5_DTB`, `Y5_OUT`) |
| Eksport do repo, narzędzia pobrane | `D:\y5_linux_work\export\` (patch kernela, `config-7.3-y5`, `e2fs\`, `e2extra\`, `rootfs\`, `pc_work.zip`) |
| Rootfs Alpine | `D:\y5_linux_work\alpine-minirootfs-3.24.2-aarch64.tar.gz` |
| Klient telnetu/pliki | `D:\y5_linux_work\phone_shell.py`, `phone_push.py` (kopie z `tools/`) |
| mtkclient (BROM) | `D:\mtkclient_work\mtk.py` |
| **Kopia `boot` Androida 4.4** | `D:\y5_backups\20261023_201905\boot.img` (przywracanie) |
| Pełne kopie partycji (IMEI, kalibracje! NIE do repo) | `D:\huawei_y5_backup\*.bin` (np. `boot.bin` = wzorzec nagłówka), `D:\y5_backups\*` |
| Ghidra / JDK | `D:\y5_linux_work\ghidra_k44` (projekt 4.4), `jdk21\jdk-21.0.12.1+1` |
| Kernel (WSL, distro `archlinux`) | `/home/y5/linux-7.3` (poprawki), `/home/y5/linux-7.3.tar.xz` (czysty), logi `/home/y5/build73*.log` |
| Źródło DTB (nasze węzły) | `C:\Users\pc\Documents\y5_linux_work\extras_vN.dtsi` + `D:\y5_linux_work\out\stock_0.dts` |
| Klucze mostu (jawne, lokalnie) | `C:\Users\pc\Documents\y5_linux_work\*.txt`, kopia w `X:\y5linux\secrets\` |
| Most (serwer, agent PC, nginx) | repo `bridge/`; uruchomienie `start_bridge_signed.ps1 -NginxIp <IP> -Pc -PcHours N` |

### 3.4 Telefon (microfw)
| Co | Gdzie |
|---|---|
| Kernel+initramfs | partycja `boot` = `mmcblk0p36` (24 MiB) |
| Rootfs microfw | `mmcblk0p46` (GPT `userdata`, 11117 MiB, ext4, etykieta `microfw`) |
| Konfiguracja startu | `/etc/inittab`, `/etc/y5/rc.sh`, `/etc/y5/diag.sh`, `/etc/y5/ready` (flaga: bez niej initramfs nie przełączy), `/etc/os-release`, `/etc/hostname` |
| Narzędzia | `/sbin/{y5txt,y5cal,wdt,mke2fs,e2fsck,tune2fs}`, `/bin/busybox-extras` (udhcpd, telnetd, httpd), biblioteki e2fs w `/usr/lib` |
| Nietykalne partycje | `preloader`, `lk`, `nvdata`(p6), `nvram`, `nvcfg`(p5), `proinfo`, `protect1/2`(p9...), `expdb`(p3), `flashinfo`(p47) |
| Zachowany Android | `system`(p42), `vendor`(p41), `cust`(p40), `preas`(p43), `cache`(p45) (ext4, nietknięte) |

## 4. Komendy, które się przydają
(Agent działający LOKALNIE na PC pomija most: `ps`/`pc-put` zastępuje zwykłe uruchamianie na PC; `phone_shell.py` i `phone_push.py` działają tak samo.)
```
# z sesji chmurowej (zmienne z X:\y5linux\secrets\secrets.env)
python3 bridge/y5sh_cloud.py ps "Get-Date"                          # PowerShell na PC
python3 bridge/y5sh_cloud.py pc-put LOKALNY 'D:\zdalna'             # plik na PC
python3 bridge/y5sh_cloud.py pc-get 'D:\zdalna' LOKALNY             # plik z PC
python3 bridge/y5sh_cloud.py job "polecenie" ; ... job-status ID    # dłuższe zadania (>60 s)
# na PC (przez ps): powłoka telefonu po USB
python D:\y5_linux_work\phone_shell.py "uname -a"                   # albo --file skrypt.sh
python D:\y5_linux_work\phone_push.py LOKALNY /sciezka/na/telefonie --port 9500 --mode 755
# kernel (WSL, zawsze ARCH=arm64!)
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- olddefconfig ;  nohup bash /mnt/d/y5_linux_work/bld.sh
# odzyskiwanie telefonu (BROM, właściciel)
python mtk.py w boot D:\y5_backups\20261023_201905\boot.img         # wraca Android 4.4
python mtk.py w boot D:\y5_linux_work\wd_v43.img                    # microfw bez switch_root (naprawa rootfs przez telnet)
```
Pułapki mostu: limit 100 s (Cloudflare), `ps` timeout ok. 60 s (dłuższe przez `job`), zagnieżdżone cudzysłowy (pisz skrypt, `pc-put`, uruchamiaj), agent PC bez admina.

## 5. Stan i następne kroki
Zrobione: boot kernela 7.3, ekran tekstowy, eMMC, USB RNDIS (+DHCP, telnet), rootfs na flash (v44 potwierdzone: hostname microfw, / = mmcblk0p46 ext4 rw, PID 1 busybox init, rc.sh i diag.sh działają), własne logo i ikony ładowania w ASCII (partycja `logo`).
Dalej (szczegóły: `docs/HANDOFF.md` pkt 9): potwierdzić v44 -> SSH z kluczem zamiast telnetu -> konsola fbcon z odświeżaniem panelu -> sprzątanie hooków kernela i czysty DTS -> sterownik zegarów `clk-mt6739`, PMIC -> DMA MUSB -> sterowniki (dotyk, LED, IMU, DSI/DRM, audio, GPS, modem) -> UI w C.

## Aktualizacja Codex 2026-10-24: kandydat v49
Potwierdzony obraz: wd_v48.img. Kandydat oczekujący na test właściciela: D:\y5_linux_work\wd_v49.img, kernel out\Image_wd23, DT out\v12.dtb, builder bm_wd49.py. PMIC/klawisze: docs/PMIC_BATTERY_BUTTONS.md. Pełny patch i config z przyrostem: phone/kernel/y5-kernel-7.3-pmic.patch oraz config-7.3-y5-pmic; nakładać na czysty kernel zamiast poprzedniego patcha. Nakładka DT: phone/kernel/y5-pmic-v12.dtsi. Źródła PC: archive/pc_work/y5-pmic-monitor.c i y5_pmic_probe.c; weryfikator obrazu verify_wd49.py.

Kandydat v50 (NTC; jeszcze bez BROM): wd_v50.img, Image_wd24, v12.dtb bez zmian, bm_wd50.py. Pełny patch alternatywny phone/kernel/y5-kernel-7.3-v50.patch, config jakv49. Źródła, testy i ładowarka HL7005D: docs/PMIC_V50_TEMPERATURE.md. v49 pozostaje potwierdzony.

Aktualizacja v51: nowe nazwy mfw_vNN.img. Potwierdzony v50, kandydat D:\y5_linux_work\mfw_v51.img (Image_mfw_v51, v13.dtb, bm_mfw_v51.py). Pełny patch/config: phone/kernel/mfw-kernel-7.3-v51.patch, config-7.3-mfw-v51; nakładka mfw-v51.dtsi. Aktualizator tools/mfw_update_boot.py: check/stage przetestowane, write oczekuje na zgodę. Kopie boot prywatnie poza repo. Procedura: docs/MFW_V51_UPDATES.md.

Potwierdzenie2026-10-25: mfw_v51.img wystartował (#35). Bieżący obraz: D:\y5_linux_work\mfw_v51.img; awaryjny wd_v50.img. USB updater i readback działają, ale reboot starego kernela zawisł i wymagany był Power. Nowe sysfs ładowarki i CPU działają. Szczegóły: docs/MFW_V51_UPDATES.md.

Kandydat restartu v52: mfw_v52.img, Image_mfw_v52, v14.dtb, bm_mfw_v52.py; pełny patch mfw-kernel-7.3-v52.patch, config jakv51. Staging zweryfikowany, zapis/boot/test jeszcze nie. Ważny test dwuetapowy: docs/MFW_V52_RESTART.md.

## Rozszerzenie hardware 2026-10-25
- docs/CAMERAS_SENSORS_AUDIO_GPS.md: potwierdzone interfejsy, brakujące warstwy, testy bez restartu i ograniczenia.
- phone/hardware/capture-live-dt.json: 9 wybranych węzłów liveDT, bez pełnego zrzutu/identyfikatorów.
- phone/hardware/camera-power-sequences.json: 14 kompilacyjnych wariantów, NIE wykryte sensory; enumy nadal surowe.
- phone/ghidra/k44_camera_als.c: źródło dowodu o LTR577 i camera power; cele camera_als_targets.txt.
- archive/pc_work/mfw_ltr577_snapshot.c: read-only native C, fixedI2C1/0x53, bez konfiguracji/NVRAM.
- archive/pc_work/mfw_capture_test.sh: jawne, ograniczone przechwytywanie dopiero z gotowym V4L2/ALSA; lokalne pliki prywatne, nigdy Git.

## V54 i fizyczny MStar
Dowody i procedura: docs/MFW_V54_TOUCH_BRINGUP.md. Kod nativeC reader/power/wrapper i ekstraktory w archive/pc_work; dekompilacje k44_tskit_detect, k44_touch_vendors, k44_mstar_detect/transport/reports/irq w phone/ghidra. Bieżący kernel v54, nie v53. Obrazy i prywatne backupy nadal poza repo.

## DRM/GPU i v55/v56
Bieżące dowody i ograniczenia: docs/DRM_GPU_BRINGUP.md. phone/kernel/display to początkowa walidacja scanoutu, bez aktywacji sprzętu. Pełny alternatywny patchv56/config/y5disp w phone/kernel; nie nakładać pełnychpatchy jeden po drugim. Tablica49modułów: phone/hardware/display-modules.json, dekompilacjeGPU/DSI/modułów w phone/ghidra. Buildery, walidatory, testy i odczyty nativeC w archive/pc_work. Obrazy/backups wyłącznie pozaGit.

## DRM/GPU: aktualizacja v57/v58
Biezacy bootv57 z potwierdzonym IRQ; v58 ograniczony DMA scanout test przygotowany (jeszcze nie aktywowany). Kod phone/kernel/display/mfw-rdma-dma-test.c, mfw-frame-switch.h i test_frame_switch.c; opcjonalny fragmentDT dma-test-v58.dtsi. Kernel fullpatch/configv58 w phone/kernel. GPU firmware: metadane bez bloba phone/hardware/gpu-firmware-stock-gate.json, checker archive/pc_work/check_pvr_fw_header.py. Dekompilacje i tabela portow w phone/ghidra/k44_scanout_latch.c,k44_gpu_latch_more.c,k44_gpu_enable.c,scanout_gpu_tables.txt. Aktualny stan zawsze w HANDOFF/NOTES, starsze naglowki sa historyczne.
