# Y5 (Huawei, MT6739) - notatki robocze, sesja 2026-10-23

## Zrobione dzis
- Most chmura -> telefon: https://<domena>/y5-<losowy>/ (Cloudflare -> nginx -> PC 192.168.1.64:2324 -> telefon). Pliki: ../bridge_proxy/
- Latarka: przycisk w centrum sterowania steruje /sys/class/leds/torch-light{0,1} (control_center.py).
- Prawdziwy multitouch: y5-touchb.py (event3 typ A -> uinput typ B) + labwc WLR_BACKENDS=x11,libinput (obraz dalej Xvfb+gpupresent).
  - na telefonie: /usr/local/bin/{y5-touchb.py,y5-udev-prep.sh,y5-ui}; y5-session przepisany (kopia: y5-session.legacy)
  - seatd musi byc z SEATD_VTBOUND=0 (kernel bez VT); eudev doinstalowany (tylko baza udev dla libinput)
  - y5-ui mt|legacy przelacza tryb bez ruszania Xvfb/gpupresent
- Bateria: capacity=-1 (brak fuel gauge) -> poziom szacowany z batt_vol (krzywa Li-ion 4.4 V), status_bar.py
- Ikona baterii w stylu iOS: pigulka bez konturu i wypustki (icons.py, kopia przed zmiana w /root/backup_20261023/)

## Decyzje uzytkownika (nowy UI)
- Powloka UI pisana w **C** (szybka, responsywna), otwarta na styl "liquid glass".
- Okna: domyslnie **pelny ekran + multitasking** (jak iOS). Title bar z przyciskami tylko jako tryb okienkowy - NA POZNIEJ.
- Etap 1: przelacznik aplikacji + gesty paska home. Reszta (centrum sterowania, lock screen, ekran glowny) potem.
- Restart telefonu dozwolony, gdy uzytkownik jest przy nim (czerwony ekran LK -> Power).

## Pomiar renderowania programowego (pixman/NEON, 736x1440, bench_cpu.c, 2026-10-23)
- memcpy/SRC pelny ekran 3.2 ms; OVER pelnoekranowy 5.4 ms; 4 polprzezroczyste panele 1.1 ms;
  karta 0.5x (bilinear) 4.5 ms; blur szkla panel 360x300 0.29 ms; blur calego ekranu (1/4) 3.7 ms.
- Wniosek: rysowanie CPU wystarcza na ~60 fps typowej klatki UI. Waskie gardlo = lancuch prezentacji
  (klient -> labwc/pixman -> Xvfb -> gpupresent: GL tekstura + PBO + kopia do fb0, ~20 fps), nie samo rysowanie.
- GPU (PowerVR GE8100) dostepny tylko przez biblioteki bionic (libEGL_mtk) w chroot Androida; Alpine ma tylko mesa programowa,
  brak sterownika dla tego GPU. Aplikacje z Alpine nie wyrenderuja na GPU.
- Plan: wlasny kompozytor w C na wlroots (pixman) + libinput, wynik prosto do fb0 (mmap + FBIOPAN_DISPLAY), bez Xvfb/labwc/gpupresent.

## Kernel 7.3: ustalenia z rozruchu (2026-10-23, testy przez BROM, obrazy w D:\y5_linux_work)
- recovery/erecovery sa weryfikowane (nie bootuja nieautoryzowanego obrazu) - testy tylko przez `boot` + przywracanie BROM.
- LK honoruje kernel_addr z naglowka (0x40200000 dziala), LK NIE weryfikuje zawartosci kernela (zmiana 1 bajtu bootuje).
- Obraz z malym kernelem (gzip ok. 21 KB) NIE jest wykonywany; stockowy rozmiar/struktura (gzip kilka MB) jest. Prawdziwy kernel 7.3 (gzip 4.5 MB) dziala.
- DTB: zmodyfikowany `stockbase` (reserved-memory + chosen dopisane do stockowego DTB) PSUJE start (restart po 20 s). DTB `v1` = stock + zwykle wezly
  (simplefb, watchdog, mmc, usb, stale zegary i regulatory), BEZ reserved-memory i chosen -> LK przyjmuje.
- Wczesny hak w arch/arm64/kernel/head.S (primary_entry): wylacza watchdog TOPRGU 0x10007000 (MODE=0x22000000) i zamalowuje fb 0x771e0000.
  Z haka kernel 7.3 NIE restartuje sie (img: h3_real_v1dtb.img). Panel NIE odswieza sie od zapisu do fb (brak triggera DSI/DISP w 7.3) -> ekran bez podgladu.
- USB gadget (RNDIS) w 7.3 nie wstaje: stockowy usb0@11200000 uzywa PHY pod 0x11cc0000 + bramek zegarow (infracfg/topckgen); w DTB v1 tylko stale zegary i brak PHY.
- LK nakleja sprzet plyty z partycji odmdtbo (jedna nakladka DTB 42 761 B @1024): led_simple, flashlights_aw3643, i2c1 (czujniki), pio (piny), nfc, board_id.
  Odczytane piny: flash AW3643 HWEN=GPIO10, STROBE=GPIO3; led_simple led_output0/1 = GPIO94 (jeden pin). Diody red/green prawdopodobnie przez PMIC (ISINK), nie GPIO.
- Obrazy testowe: stub*.img, control_*.img, h1/h2/h3, real_dbg.img. Pipeline: D:\y5_linux_work\build_mainline.py (env Y5_IMAGE/Y5_DTB/Y5_OUT/Y5_KADDR),
  flash: `python -I flashpart.py <img> boot` z dzialajacego 4.4 albo BROM `python D:\mtkclient_work\mtk.py w boot <img>`.
  Przywrocenie 4.4: `mtk.py w boot D:\y5_backups\20261023_201905\boot.img`.
- Brakujacy kanal podgladu: UART0 apuart0@11002000 (921600) lub USB (PHY) lub trigger wyswietlacza.

## Protokoly Wayland dostepne w labwc (zmierzone narzedziem tools/globals.c, 2026-10-23)
- Powloka UI (okna, przelacznik): zwlr_layer_shell_v1 v4, zwlr_foreign_toplevel_manager_v1 v3, ext_foreign_toplevel_list_v1,
  ext_image_copy_capture_manager_v1 + ext_foreign_toplevel_image_capture_source_manager_v1 (miniatury okien),
  zwlr_screencopy_manager_v1, ext_session_lock_manager_v1 (ekran blokady), ext_idle_notifier_v1 (auto-blokada),
  zwlr_output_power_manager_v1 (ekran on/off), wp_viewporter + wp_alpha_modifier_v1 + wp_single_pixel_buffer (tanie skalowanie/zanikanie/zaslona),
  wp_fractional_scale_v1, xdg_activation_v1, ext_workspace_manager_v1, wp_presentation.
- Dotyk: wl_touch trafia tylko do powierzchni, ktora dostala "down" (niejawny grab) - gesty z krawedzi robi cienka powierzchnia
  layer-shell przy krawedzi; animacje: migawka okna (image copy capture) jako wlasna powierzchnia skalowana viewporterem.
- Architektura powloki C: jeden staly proces y5shell (wayland-client + cairo/pixman, wl_shm), powierzchnie tworzone raz i tylko pokazywane/ukrywane.

## Modem - hipoteza z analizy stockowych plikow rc (2026-10-23, tylko odczyt)
- Stock: ccci_fsd/ccci_mdinit/ccci_rpcd (class core) + **nvram_daemon (class main, root)**; ccci_mdinit czeka w wait_nvram_ready na
  service.nvram_init=Ready ustawiane przez PRAWDZIWY nvram_daemon. My od poczatku podstawiamy "Ready" w shimie i nie uruchamiamy
  nvram_daemon -> brak plikow NVRAM/kalibracji dla modemu to mozliwa przyczyna HS2_FAIL.
- nvram_daemon: 32-bit, zalezy od libnvram/libcustom_nvram/libfile_op/libhwm; potrzebuje wezlow /dev/nvram, /dev/pro_info, /dev/misc,
  /dev/mtk-adc-cali (w /aroot/dev, ktory jest osobnym rootfs), fstab w /vendor/etc, zapisu do /vendor/nvdata (ext4 p6) i partycji nvram (p20).
- UWAGA: nvram_daemon ZAPISUJE do nvdata/nvram (IMEI, MAC, kalibracje). Nie uruchamiac przed kompletnym, zweryfikowanym backupem
  (nvdata, nvcfg, nvram, proinfo, protect*) i najlepiej z nvdata zamontowanym najpierw tylko do odczytu / na kopii.
- md_ctrl dotyczy tylko szyfrowania (vold.decrypt) - nie startuje modemu.
- Bonus: stock ma fuelgauged (fuelgauged_init.rc) - to brakujacy demon poziomu baterii (capacity=-1).

## Na pozniej (zyczenia uzytkownika)
- **Dynamic notch / wyspa** u gory ekranu (animowana, rozszerza sie np. przy muzyce, timerze, ladowaniu).
  Robic DOPIERO po przepisaniu powloki na C; uzytkownik rozwaza tez Quickshell.
  Uwaga: Quickshell = Qt6/QML, a tu renderowanie programowe (pixman) i 1.8 GB RAM. Przed decyzja zrobic maly
  prototyp (czas startu, RAM, plynnosc animacji) i porownac z notchem rysowanym w C (cairo/layer-shell).
- Okna z title barem i przyciskami (tryb okienkowy) - domyslnie wszystko pelnoekranowe.
- Kreski sygnalu pokazuja zawsze 4/4 (brak modemu) - zmienic, gdy modem zadziala.

## Przyczyny wolnosci obecnego UI
- Kazdy gest paska home uruchamia nowy proces `python3 app_switcher.py` (import GTK ~1.3 s + budowa okna = ~3 s).
- app_switcher uruchamia aplikacje jako kolejne procesy (np. drugi control_center.py).
- Cala grafika programowa (pixman), obraz przez Xvfb -> gpupresent (~20 fps).

## Do zrobienia pozniej
- Modem (HS2_FAIL po ~35 s, patrz project_microfw_y5.md), Bluetooth demon, audio, H.264 w Firefoxie.
- Zrewokowac klucze auth Tailscale (2 sztuki) i rozwazyc nowy token mostu.

## Stan na koniec sesji (2026-10-24, noc)
- Kernel 7.3 z PHY (`PHY_MTK_TPHY`) i gadzetem (`USB_ETH`+RNDIS, `USB_MUSB_GADGET`) zbudowany w WSL: /home/y5/linux-7.3 (Image -> D:\y5_linux_work\out\Image_usb), DTB v2 (`phone/stub/extras_v2.dtsi`, PHY generic-tphy-v1 @0x11cc0800 - adres z analogii MT6735/MT2701, niepotwierdzony).
- Test `usb_v2.img`: telefon "booting now", BEZ restartu, bez urzadzenia USB.
- Nastepny krok: `D:\y5_linux_work\usb_v2log.img` (init zapisuje raport dmesg/deferred/iomem/clk do partycji `expdb` po ~25 s). Wgrac przez BROM, odczekac 2 min, przywrocic 4.4 (`python mtk.py w boot D:\y5_backups\20261023_201905\boot.img`), odczytac `expdb` przez most: `dd if=/dev/block/.../expdb bs=4096 count=64 | head -c 300000` (naglowek `Y5LOG<8 cyfr rozmiaru>`).
- Zmienne srodowiskowe mostu: patrz docs/KLUCZE_I_ZMIENNE.md (BRIDGE_URL/TOKEN/SIGN_KEY, PC_URL, BRIDGE_PS_TOKEN/SIGN_KEY).
- Uwaga: build kernela w jednym `pc_job` moze przekroczyc 3600 s; uruchamiac przez nohup w WSL z logiem (/home/y5/build73*.log).

## Przelom (2026-10-24): kernel 7.3 dochodzi do init
- Punkty kontrolne (watchdog TOPRGU, max timeout 31 s! pole LENGTH ma 11 bitow, wiecej zawija modulo 32): cp_v5.img -> restart ~90 s = kernel przeszedl wszystkie etapy rozruchu (head.S, idmap, MMU, setup_arch, GIC, timer, initcalle) i dotarl do init po ok. 58 s od "booting now".
- Wczesniejszy brak sygnalu byl brakiem WIDOCZNOSCI, nie awaria kernela. Podejrzenie VA_BITS_52 i create_init_idmap to falszywy trop (moj blad: timeouty >31 s).
- Nastepne: wd_v1.img (init sygnalizuje stan restartem: bity mmc_host=1, log do expdb=2, udc=4, opoznienie 4 s na bit, potem otwarcie /dev/watchdog).

## Ankieta restartami (2026-10-24)
- Init na 7.3 DZIALA (restart ~10 s = `wdt` z init, M=0): kernel dochodzi do init szybko; mmc_host i UDC NIE sa widoczne (nie zasondowaly).
- Embedded initramfs (16.5 MB) i hipoteza "kernel > 0x40f00000" nie potwierdzone/nieistotne; wd_v4 (12.5 MB, ramdisk z LK) tez restartuje po 10 s.
- poll_v1.img: kazdy cykl (restart) odpowiada na pytanie K (licznik w RAM 0x44410010, magic 0xA5 w ...11): czas ~10 s = nie, ~16 s = tak, ~28 s = znacznik startu (K=0). Pytania 1..17 w phone/stub/build_mainline_wd5.py.

## Dane z kernela 4.4 (Ghidra + kallsyms), 2026-10-24
- /proc/kallsyms 4.4 -> phone/kallsyms_44.txt; Ghidra 12.1.4 (JDK21: D:\y5_linux_work\jdk21) headless: projekt D:\y5_linux_work\ghidra_k44, skrypt phone/ghidra/Y5Decomp.java (dekompiluje funkcje z listy), wyniki k44_dec*.c.
- Tablica bramek infracfg (0x10001000): phone/infra_gates_44.txt (set/clr/sta + bit). infra_icusb: set 0x80, clr 0x84, sta 0x90, bit 8. infra_msdc0 bit 2 (0x94 grupa).
- Tablica muxow topckgen (0x10000000): phone/top_muxes_44.txt. usb_top_sel: CLK_CFG @0xb0 (set 0xb4, clr 0xb8), shift 24 width 1, gate bit 31, upd @0x4 bit 30.
- Polaryzacja przerwan: stockowy root ma interrupt-parent = sysirq (phandle 1); urzadzenia daja level-low (8), GIC dostaje inwersje z sysirq.
- 7.3: mmc nie wstaje mimo pinctrl; USB wlaczony bez ungate zegarow = panika. Hak y5_clk_hack (early_initcall) odblokowuje infra_icusb i usb_top_sel.

## Model watchdoga (2026-10-24): restart po 2xT
- Watchdog TOPRGU z bitami trybu zostawionymi przez LK restartuje telefon po **2×T** (tryb dual: pierwszy timeout = IRQ, drugi = reset), nie po T. Czas od "booting now" ~ X(1,5 s) + t + 2T.
- Potwierdzenie na 3 pomiarach: panic hook T=2 -> 5,5 s; T=31 -> 63,5 s; T=1 -> 3,5 s => kernel z USB wlaczonym PANIKUJE ok. 1,5 s po wejsciu (przy sondowaniu PHY/USB), a nie "dziala 60 s".
- Wczesniejsze tablice czasow (T+X) byly wiec bledne o czynnik 2; wnioski o "init dziala po 58 s" i "restart ~10 s = init" uniewaznione.
- wd_v12.img: hak panic wylaczony; punkty kontrolne w really_probe (phy T=4/6, usb 8/10, mmc 12/14): restart po 1,5+2T s.

## PRZYCZYNA PANIKI/ZAWIESZENIA USB (2026-10-24)
- wd_v12 (punkty kontrolne w really_probe, bez haka panic): restart 17,5 s = 1,5 + 2*8 => zawieszenie WEWNATRZ probe musb (11200000); PHY probe OK.
- mtk_musb_init (mainline) pisze rejestry MAC (TXTOGEN) PRZED phy_init; na MT6739 PHY/MAC wymaga najpierw sekwencji usb_phy_recover() z 4.4 (PHY @0x11cc0000: rejestry 0x81c,0x868,0x86c,0x820,0x818,0x814,0xf10,0xf00,0xf0c = pomiar czestotliwosci).
- wd_v13: ta sekwencja wykonywana w y5_clk_hack (early_initcall) przed sondowaniem. Dekompilacja: phone/ghidra/k44_dec1.c.

## EKRAN DZIALA (2026-10-24)
- wd_v20: kolory biały/niebieski/zielony/czerwony widoczne co ~3 s => panel odswieza sie z framebuffera LK (0x771e0000, 720x1440, stride 2944, a8b8g8r8) na kernelu 7.3; kanal wizualny bez UART/USB.
- Kernel ma juz FRAMEBUFFER_CONSOLE + DRM_SIMPLEDRM + VT; wd_v21 = init pisze na /dev/tty0 (dmesg) i wylacza watchdog (wdt x 0x10007000 0x22000000).
- Wniosek o watchdogu: kanal "czas do restartu" okazal sie niewiarygodny (v17 45,7 s; v18 21,5 s; v19 brak) - nie opierac sie na nim.

## Ekran: adres skanowanego bufora (2026-10-24)
- Test pasków (wd_v26): panel skanuje bufor od 0x7730B000 (= 0x771e0000 + 416 wierszy * 2944 B), stride 2944 B, format XRGB8888 (R=bity 23:16), 720x1440 widoczne.
- Wyzwolenie odswiezenia: MUTEX0 EN (0x14015020=1) + RDMA0 (0x1400b010=1) + DSI_START (0x14012000: 0 potem 1). Samo DSI_START nie wystarcza; petla DSI co 0,3 s powoduje miganie starego obrazu.
- phone/stub/y5txt.c: renderer tekstu 8x16 (czcionka z kernela) -> /dev/mem -> bufor + wyzwolenie; init wd_v27 pokazuje dmesg na ekranie.

## y5txt: nieskonczony memset (wd_v33)
Wlasny `memset` w -nostdlib byl przez GCC -O2 zamieniany na wywolanie samego siebie (`b memset`), wiec y5txt wisial na zerowaniu tablicy.
Kompilowac z `-ffreestanding -fno-tree-loop-distribute-patterns` (patrz phone/stub/build_tools.sh). Skan panelu: stride 720 px, baza 0x775E8E38 (po korektach z siatki kalibracyjnej).

## Czcionka: komentarze w font_8x16.c (wd_v35)
`mkfont.py` lapal tez `0x..` z komentarzy `/* 65 0x41 'A' */` -> 4352 bajty zamiast 4096, glify przesuniete (17 bajtow na znak). Teraz komentarze sa wycinane i jest `assert len==4096`.
Stan v35: boot, init, y5txt i ekran dzialaja (tekst rysuje sie), tylko glify byly zle.

## Pierwszy dmesg na ekranie (wd_v36)
- `phy-11cc0000.t-phy.0: can't request region [mem 0x11cc0800-0x11cc08ff]`, `mtk-tphy probe failed -16` => reg rodzica t-phy (0x1000) nakladal sie na dziecko u2port@0x800. v9: reg rodzica 0x100.
- `UDC core: g_ether: couldn't find an available UDC` - nastepstwo (musb nie wstal bez PHY).
- Brak jakichkolwiek linii mmc w ostatnich 80 liniach dmesg (do sprawdzenia w v37: filtr grep na gorze ekranu, naglowek na dole).
- Bez bledow: simpledrm fb0, mtk-wdt, init start 0.45 s, crng init 20 s.

## v37: tphy OK, MUSB pada na "DMA controller not set" (-ENODEV)
- v9 DTB naprawil t-phy (brak bledu -16). Teraz `musb-hdrc: DMA controller not set` + `musb_init_controller failed -19`.
- Zrodlo: drivers/usb/musb/musb_core.c ~2421: bez CONFIG_MUSB_PIO_ONLY wymagane ops->dma_init/dma_exit, a mediatek.c ich nie ma => -ENODEV. To dev_err, nie info.
- Poprawka: CONFIG_MUSB_PIO_ONLY=y (+ DEBUG_FS=y dla /sys/kernel/debug/devices_deferred). Potem mozna wlaczyc DMA Inventra.
- UWAGA: `make olddefconfig` ZAWSZE z ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- (bez tego .config traci symbole arm64). Odtworzone z .config.old.
- mmc: brak jakichkolwiek linii w dmesg; v38 pokazuje platform devices 11230000/11200000/11cc0000 + driver.

## v38/v39: USB po stronie telefonu dziala
- PIO-only MUSB: UDC `musb-hdrc.1.auto` + usb0 (g_ether). PC widzi VID_0525:PID_A4A2 jako "USB Serial Device (COM15)" - Windows przypina usbser do CDC.
- eMMC: `vmmc regulator not available` = brak CONFIG_REGULATOR_FIXED_VOLTAGE (wlaczone w v39).
- v40: g_ether wylaczony, USB_CONFIGFS (RNDIS/ECM/NCM/ACM). Init tworzy gadzet g1 z deskryptorami MS OS (compatible_id RNDIS, 5162001), VID 1d6b:0104, usb0 = 192.168.7.2/24.

## v40: eMMC i USB dzialaja
- eMMC: mmc0 HS200, mmcblk0 14.6 GiB, p1..p18 (+boot0/1, rpmb). Ostrzezenia GPT (first_usable_lba/crc) typowe dla MTK.
- USB: configfs gadget RNDIS + MS OS desc => Windows sam wybral "Remote NDIS Compatible Device" (Ethernet 4, host MAC <MAC zanonimizowany>). udc state=configured, usb0 carrier 1.
- Agent PC nie ma admina -> nie da sie nadac IP karcie; v41: udhcpd na telefonie (192.168.7.10-20), telefon 192.168.7.2.
- v41: telnetd -b 192.168.7.2 -l /bin/sh (TYLKO TEST, bez hasla; zamienic na SSH z kluczem / dropbear).

## v41: DHCP/telnet nie wstaly (brak apletow w minirootfs)
- RNDIS dziala (Windows: Ethernet 4, Remote NDIS Compatible Device), ale PC zostaje na 169.254.x: Alpine minirootfs nie ma udhcpd ani telnetd (sa w pakiecie busybox-extras).
- v42: do initramfs dodany /bin/busybox-extras (apk 1.37.0-r31 z dl-cdn.alpinelinux.org/alpine/v3.24/main/aarch64, skrypt archive/pc_work/fetch_extras.py) + dowiazania usr/sbin/udhcpd, telnetd; ekran pokazuje 'svc:' (pid-y i adres usb0).

## v42: brak DHCP mimo busybox-extras -> brak CONFIG_PACKET
- udhcpd odpowiada klientowi bez IP przez AF_PACKET; `# CONFIG_PACKET is not set` => brak lease (PC zostaje 169.254.x). v43: CONFIG_PACKET=y (Image_wd20).
- Do sprawdzenia przed rootfs: F2FS/VFAT/FUSE/OVERLAY wylaczone (kernel 12,7 MB; limit rozmiaru nieznany, 16,5 MB z initramfs zepsulo rozruch).

## v43: SIEC DZIALA, powloka na telefonie (2026-10-24, wieczor)
- CONFIG_PACKET=y => udhcpd dziala: PC dostaje 192.168.7.20 (Ethernet 4), ping 0% strat, telnet 192.168.7.2:23 (busybox-extras telnetd, bez hasla, TEST).
- Klient: tools/phone_shell.py (surowe gniazdo, IAC); na PC: `python D:\y5_linux_work\phone_shell.py "polecenie"` lub `--file skrypt.sh` (pc-put skryptu).
- eMMC: 47 partycji (/proc/partitions): boot=p36 (24M), recovery p37, system p42 (1712M ext4), vendor p41 (368M ext4), cust p40 (192M), preas p43 (880M), cache p45 (128M ext4), userdata p46 (11117M, blkid NIE rozpoznaje: zaszyfrowana lub pusta), expdb p3 (20M), nvcfg p5, nvdata p6, metadata p7, protect1 p9, flashinfo p47, logo p35.
- Nazwy: /sys/class/block/mmcblk0pN/uevent (PARTNAME). RAM 1647 MiB. Initramfs ma blkid, switch_root, mkfs.vfat; BRAK mkfs.ext4/e2fsck (trzeba e2fsprogs z Alpine) .
- Wszystkie partycje Androida to ext4 => F2FS niepotrzebny.

## microfw: rootfs na userdata (2026-10-24, noc)
- Nazwa systemu: **microfw** (etykieta ext4, hostname, os-release, gadzet USB). Nazwa GPT partycji zostaje `userdata` (nie ruszamy tablicy).
- userdata (mmcblk0p46) sformatowana ext4 (mke2fs 1.47.4 z Alpine, narzedzia wgrane do /tmp przez tools/phone_push.py), etykieta `microfw`, Alpine minirootfs + narzedzia (y5txt, wdt, busybox-extras, e2fsprogs) + phone/rootfs/etc/{inittab,y5/rc.sh,y5/diag.sh,os-release,hostname,motd}.
- v44: initramfs montuje p46 (tylko gdy blkid LABEL="microfw" i istnieje /etc/y5/ready) i robi switch_root na /sbin/init (busybox init, inittab -> /etc/y5/rc.sh: siec, telnetd; diag.sh rysuje ekran).
- Gdyby init z flash padl: BROM i wgraj wd_v43.img (initramfs bez switch_root) albo usun /etc/y5/ready po zamontowaniu p46.

## Logo startowe (partycja `logo`, p35, 4 MiB)
- Format MTK: 512 B naglowka (magic 0x58881688, dsize, nazwa "logo"), potem u32 count(41), u32 total, offsety, strumienie zlib (720x1440 BGRA 4147200 B lub male ikony).
- Obraz 0 = "HUAWEI powered by android" (to ono pojawia sie przy power on), 1 = niski poziom baterii, 2 = ikona ladowania, 3.., 4-24 male ikony/cyfry, 25-40 klatki ladowania.
- tools/make_logo.py ORYGINAL WYNIK [indeksy] zamienia wskazane obrazy na czarne. Kopia oryginalu: D:\y5_backups\20261023_201905\logo.img. Wgranie przez BROM: `python mtk.py w logo D:\y5_linux_work\logo_black.img`.
- Czerwony ekran "Your device has failed verification" jest rysowany przez LK (weryfikacja boot) - NIE jest w partycji logo, nie znika bez zmiany LK/podpisu.

## Logo microfw (obraz 0 w partycji logo)
- Wlasne logo (v2): czarne tlo, biale napisy, supersampling 4x; duzy ASCII art na ok. 42% wysokosci (580 px), na samym dole "POWERED BY" (JetBrains Mono Bold; SF Mono niedostepny/licencja) + maly ASCII art (250 px). Podglad: phone/logo/microfw_logo_preview.png.
- Render: tools/render_logo.py ASCII.txt WYNIK.raw PODGLAD.png ; skladanie: tools/make_logo.py ORYGINAL.img WYNIK.img --raw 0=WYNIK.raw. Wynik: D:\y5_linux_work\logo_microfw.img (tylko obraz 0 sie rozni od oryginalu).
- Do zrobienia (na prosbe wlasciciela): nowe ikony baterii/ladowania (obrazy 1-3, 4-24 male, 25-40 klatki ladowania).

## Komplet obrazow partycji logo w stylu microfw (ASCII, czern/biel)
- Zawartosc oryginalu (41 obrazow): 0 logo startowe; 1 niski poziom baterii; 2 czerwony piorun w kolku; 3 "Error! Please plug out charger!"; 4-13 cyfry 0-9 (45x64); 14 znak % (57x64);
  15-24 nakladki fali 163x29 (animacja pelnej baterii); 25-35 klatki pustej baterii z mala fala na dnie (3-10% wysokosci); 36 linia 163x1; 37 pelna bateria + "100%"; 38 drugie logo HUAWEI;
  39 zielony piorun w kolku; 40 ekran "META" (tryb fabryczny). Wymiary/pozycje zmierzone z oryginalu (bateria x270-450 y606-946, kolko srodek 359,389 srednica 286).
- Wszystkie podmienione: tools/render_logo.py (0), tools/render_battery.py (1-40), tools/build_logo_all.sh => D:\y5_linux_work\logo_microfw_all.img. Podglady: phone/logo/microfw_*_preview.png.
- Mapowanie zielony/czerwony (2/39) i etykiet (np. "LOW BATTERY" w 1) to ODGADNIECIE na podstawie wygladu; wlasciciel potwierdzil, ze oba piorunowe ekrany sie pojawiaja przy ladowaniu.
- Nakladki fali (15-24) i obraz 36 sa czarne; jesli animacja 100% bedzie wygladac zle, przywrocic je z oryginalu.

- Aktualizacja stylu (na prosbe wlasciciela): ekrany 2 i 39 (piorun w kolku) to teraz ASCII z cieniowaniem znakami (`.:+=#`, komorka 7.2x12 px), 2 = przerywany pierscien, 39 = pelny.
  Wszedzie, gdzie jest nazwa microfw, jest ASCII art (figlet "slant": phone/logo/{microfw,meta,mode}_ascii.txt; META i MODE wygenerowane pyfiglet-em).
  Plik: D:\y5_linux_work\logo_microfw_all.img (sha256 d522e510...5d885).

## v44 POTWIERDZONE (2026-10-24, noc)
- microfw startuje z flash: hostname microfw, `/` = /dev/mmcblk0p46 ext4 rw, PID 1 = busybox init, /etc/y5/rc.sh wykonany (siec, telnetd, udhcpd), diag.sh rysuje ekran; RAM 1647 MiB, uzycie 37 MiB.
- PC dostaje adres z DHCP (192.168.7.x, nazwa karty zmienia sie po ponownym enumerowaniu: Ethernet 4/5), ping 0% strat.
- Drobiazgi: po switch_root zostaja zombie `[udhcpd]`/`[telnetd]` z initramfs (PID 1 busybox je sprzata lub nie - kosmetyka); brak RTC (data 1970-01-01).

## Przejęcie lokalne przez Codex (2026-10-24)
- Przeczytano docs/HANDOFF.md, docs/MAPA_PLIKOW.md, docs/BUILD_AND_FLASH.md oraz phone/NOTES.md. Dokumenty opisują starsze v41/v44; aktualny stan v48 pochodzi z przekazania właściciela, nie z identyfikacji obrazu na urządzeniu.
- Pomiar: ping 192.168.7.2: 2/2 odpowiedzi, <1 ms; phone_shell.py działa. uname: Linux microfw 7.3.0 #30 SMP PREEMPT Sat Oct 24 22:07:40 CEST 2026 aarch64; hostname microfw; uptime 4 min; RAM 1647 MiB, użyte 28 MiB, swap 0.
- Sam uname nie potwierdza wersji obrazu boot. Właściciel wskazuje wd_v48.img (Image_wd22, v11.dtb) jako działający obraz z konsolą fbcon i banerem; nie powtarzać wykonanej już pracy nad konsolą bez sprawdzenia.
- Obowiązuje tryb lokalny Windows/WSL, bez mostu i dostępu do secrets. Flash boot wykonuje wyłącznie właściciel przez BROM. Zakaz dalszych prób zmiany partycji logo oraz zapisów do pozostałych partycji poza dopuszczonym rootfs; stare instrukcje logo/expdb w dokumentacji są nieaktualne i nie stanowią zgody na zapis.
- Następny krok: odczyt konfiguracji startu i dostępnych narzędzi, przygotowanie SSH Dropbear z kluczem i nasłuchem na 192.168.7.2; wyłączyć telnet dopiero po sprawdzeniu dostępu SSH. Zmiana initramfs wymaga nowego obrazu i ręcznego flashowania przez właściciela.

## Priorytet: używalność jako drugie urządzenie (2026-10-24)
- Właściciel odłożył Dropbear: teraz rozwijamy funkcjonalność do codziennego używania jako drugie urządzenie. Telnet pozostaje na czas prac.
- Odczyt działającego systemu: /proc/bus/input/devices pusty; brak węzłów /dev/input; /sys/class/power_supply pusty; /sys/class/backlight nie istnieje. Są /dev/fb0 i /dev/dri/card0. Rootfs ma 10.5 GiB wolnego.
- To potwierdza brak zarejestrowanych urządzeń wejściowych i interfejsów baterii/podświetlenia, a nie identyfikuje fizycznych układów ani nie dowodzi stanu ładowania.
- Najbliższy kierunek: identyfikacja kontrolera dotyku i jego połączeń na podstawie zapisanych danych stockowego kernela/DT; potem obsługa dotyku, zasilania i minimalna powłoka C. hw_inventory_73.md wskazuje i2c-0 0x5d i hipotezę Goodix GT9xx, wymagającą potwierdzenia.

## PMIC, bateria i przyciski — priorytet właściciela (2026-10-24)
- Właściciel wskazał D:\y5_linux_work jako istotne źródło materiałów; bieżący zakres to PMIC, bateria i przyciski, przed dotykiem/UI.
- stock_0.dts: PWRAP 0x1000d000, IRQ <0 0xaf 4>; keypad 0x10010000, IRQ <0 0xb0 2>; węzeł gauge compatible mediatek,mt6357_gauge. To dane stockowego DT, nie nowy odczyt identyfikatora PMIC.
- Sprawdzono lokalne źródła /home/y5/linux-7.3: mtk-pmic-wrap.c obsługuje PMIC MT6357 jako slave, ale brak dopasowania hosta MT6739; mtk-pmic-keys.c obsługuje mt6357-keys; mt6359-auxadc.c zawiera mt6357-auxadc. W .config MTK_PMIC_WRAP i IIO są wyłączone. Nie zmieniano .config.
- Inwentarz z 4.4 wskazuje aktywny FAN54005 i2c-5 0x6a; stock DT zawiera również rt9458, więc sama obecność w DT nie identyfikuje zamontowanej ładowarki. Rozdzielić ADC/odczyt baterii od sterowania ładowaniem.
- Rozpoczęto dekompilację lokalnego projektu Ghidra k44/stock_Image w trybie -readOnly -noanalysis, funkcje PWRAP i klawiszy; wynik D:\y5_linux_work\out\k44_pmic_codex.c, log ghidra_pmic_codex.log. Nie wykonywano zapisów rejestrów ani flashowania.
- Narzędzia: WSL nie ma rg; używać grep. Drzewo kernela nie jest repo Git. Dla poleceń WSL pisać plik .sh; zagnieżdżone cytowanie ponownie zawiodło.

Pierwszy wynik Ghidry: stockowy pwrap_wacs2_hal używa WACS2 CMD/RDATA/VLDCLR pod offsetami 0xc20/0xc24/0xc28; FSM bity 18:16, INIT_DONE bit 21. To dekompilacja, nie test rejestrów. Szczegóły i kolejność prac: docs/PMIC_BATTERY_BUTTONS.md. Uwaga: analyzeHeadless.bat rozbił listę celów po przecinkach; bieżący wynik obejmuje tylko pierwszą funkcję. Dalsze cele przekazywać przez plik txt.


## PRZEŁOM: PMIC odpowiada na 7.3 bez nowego boot (2026-10-24)
- Wgrano wyłącznie do /tmp statyczny y5_pmic_probe (C), po weryfikacji SHA256 przez phone_push. Sprawdzono brak sterownika platformowego PWRAP przed uruchomieniem.
- Wrapper status 0x8070000e: INIT_DONE=1, FSM=0. Użyto wyłącznie komend odczytu PMIC (bit31=0) i potwierdzeń WACS2_VLDCLR; brak zapisów rejestrów PMIC, zasilania czy flash.
- Zmierzone: HWCID[0x8]=0x5721, SWCID[0xa]=0x5721 — potwierdzony MT6357. TOPSTATUS[0x24]=0x000e; AUXADC_ADC0[0x1088]=0xda20; ADC1/ADC3=0. ADC0 jest wynikiem zastanym, nie potwierdzono świeżości ani jeszcze nie przeliczono napięcia.
- Transport bootloadera działa pod Linux 7.3. Następny krok: ustalenie świeżej konwersji ADC i mapowania klawiszy; bez inicjalizacji regulatorów ani ustawień ładowarki.
- Źródło PC D:\y5_linux_work\y5_pmic_probe.c; build_pmic_probe.sh; out/y5_pmic_probe. Do wyeksportowania do archive/pc_work po przeglądzie.

## v49 gotowy do BROM: napięcie baterii i trzy przyciski (2026-10-24)
- Kontrolowane testy właściciela potwierdziły Power=MT6357 TOPSTATUS bit1 aktywne zero, VolumeUp=bit3 aktywne zero. VolumeDown=KPD_MEM0 bit0 aktywne zero (powielony sygnał co 9 bitów macierzy; tylko jedno zdarzenie).
- Świeża konwersja ADC0: raw 23507 -> 3873834 uV. Request0 0x110e: start 1 i stop 0, bez resetu/zapisów regulatorów/ładowarki. Nadal brak wiarygodnego capacity, temperatury i stanu ładowania.
- Przygotowano tymczasowy y5-pmic-monitor.c: polling/debounce trzech klawiszy, evdev i power_supply voltage_now. Brak IRQ/suspend; zachowana konfiguracja LK. To etap bootstrap, nie pełny sterownik PMIC/ładowarki.
- wd_v49.img, Image_wd23 (12828680 B), v12.dtb, bm_wd49.py. SHA256: 44a28e13ad9da185b745dc193657ceda79ca0290193579db479bb633d8c12c9e. ROZRUCH v49 JESZCZE NIEPRZETESTOWANY; v48 pozostaje obrazem potwierdzonym.
- Kernel zbudowany, symbole sterownika i evdev potwierdzone; obrazy porównano skryptem verify_wd49.py (identyczny initramfs, cmdline i adresy jak v48; nowe payloady Image/DTB; SHA1 boot i limit24MiB poprawne).
- Pełny alternatywny patch 14 plików: phone/kernel/y5-kernel-7.3-pmic.patch; config-7.3-y5-pmic; y5-pmic-v12.dtsi. Dry-run patcha na czystej bazie przeszedł. Kod PC w archive/pc_work, dekompilacje w phone/ghidra.
- Pułapki: power_supply_config w 7.3 ma fwnode, nie of_node; dev_fwnode wymaga linux/property.h. stdout narzędzia watch ustawiono unbuffered przed pierwszym wypisaniem. Sam nohup z kończącej się sesji WSL nie utrzymał budowania; potrzebny żywy proces nadzorujący (supervise_pmic_build.sh, wait), potem sprawdzenie mtime i symboli; stare done nie dowodzi nowego builda.
- Następnie właściciel flashuje TYLKO boot komendą z docs/PMIC_BATTERY_BUTTONS.md; sprawdzić boot/USB, sysfs napięcia i eventy klawiszy. Dokładna procedura i ograniczenia w tym dokumencie. Ładowarka FAN54005, temperatura i dalszy PMIC pozostają do zrobienia.

## v49 POTWIERDZONY NA TELEFONIE (2026-10-24)
- Właściciel wykonał BROM i zgłosił poprawny start oraz reakcje wszystkich trzech przycisków na konsoli.
- Ping 2/2 <1ms, powłoka działa. Linux 7.3.0 #33 (23:13:02), MT6357 0x5721; power_supply microfw-battery i input event0 poprawnie zarejestrowane.
- Kolejne świeże pomiary voltage_now: 3849114, 3845819, 3845324 uV. Status Unknown jest zamierzony; nie dowodzi braku ani obecności ładowania.
- Kontrolowany test evdev po zgłoszeniu właściciela Gotowe: KEY_POWER=116, KEY_VOLUMEUP=115, KEY_VOLUMEDOWN=114; każdy value1/value0 oraz SYN_REPORT po obu krawędziach. To rzeczywiste zdarzenia odczytane przez aplikację z /dev/input/event0, nie tylko log sterownika.
- v49 jest teraz bieżącym potwierdzonym obrazem (zachować v48 awaryjnie). Szczegóły pomiarów/ograniczeń: docs/PMIC_V49_TEST.md. Narzędzie y5_evtest.c i build_evtest.sh wyeksportowane do archive/pc_work.
- Następny etap: temperatura baterii (ADC/termistor/skala z 4.4) i FAN54005 (magistrala I2C, identyfikacja/status), bez zgadywania parametrów ładowania. Rozpoczęto readOnly dekompilację get_device_adc_out, BattThermistorConverTemp, BattVoltToTemp, force_get_tbat_internal, fan54005_get_chip_status i fan54005_driver_probe do out/k44_battery_thermal_codex.c.

Dekompilacja kolejnego etapu zakończona: phone/ghidra/k44_battery_thermal.c. BattVoltToTemp ma dzielnik 16900 omów/1800 mV i warunkowy tor BIF; tabela termistora pod 0xffffff80094b57f8 pozostaje do wyeksportowania i sprawdzenia. FAN54005 status: reg0, mask3, shift4; stock driver_probe wykonuje zapisy i nie nadaje się do prostego testu RO. Szczegóły w docs/PMIC_BATTERY_BUTTONS.md.

## v50 gotowy: temperatura; ładowarka zidentyfikowana jako HL7005D (noc24/25 października2026)
- Z 4.4 wyeksportowano tabelę21 punktów NTC (-40..60°C,10k przy25°C), dzielnik16900omów/1800mV. ADC BATTEMP: request0x110e bit3, ready/output0x108e bit15/mask0xfff. BIF weak stub faktycznie zwraca0 (instrukcje z Ghidry).
- Dwa świeże pomiary na v49, przy czasowo odpiętym sterowniku: raw1432,629296uV, ok.27.7°C. Sterownik ponownie podpięty i zweryfikowany (voltage_now/input event0). Pułapka: phone_shell --file podaje linie interaktywnej powłoce; trap EXIT bez podpowłoki nie przywrócił go od razu, wykonano jawny bind. Poprawiony skrypt używa (...).
- v50 dodaje TEMP w power_supply; tabela i interpolacja y5-ntc.h, timeouty/mutex jak wcześniej. Pełne testy0..4096 z UBSan,3151 poprawnych kodów, granice/monotoniczność/znane punkty PASS. Bez kompensacji spadku zależnego od prądu; temperatura na razie diagnostyczna, bez sterowania ładowaniem.
- I2C5 z LK działa w PIO, bez zmian zegarów/DMA/pinów: 0x11016000, adres0x6a, repeated START odczytu. IDreg3=0x41 dwukrotnie, reg0=0xf6 potem0xd0, reg0x10=0xc6. Nie zapisano danych konfiguracyjnych do ładowarki.
- PRZEŁOM IDENTYFIKACJI: stock fan54005_hw_component_detect mapuje ID0x41 na HL7005D (string0xffffff80094b6558). Fan54005 to wspólny sterownik, a nie pewna identyfikacja układu! Inwentarz poprawiony. Nie stosować w ciemno map/parametrów FAN54005 do HL7005D.
- Pełne wyniki: docs/PMIC_V50_TEMPERATURE.md; tabele phone/battery_tables_44.txt; dekompilacje phone/ghidra/k44_thermal_detail.c, k44_i2c5.c, k44_charger_id.c; odtwarzający eksport Y5BatteryTables.java.
- Kandydat wd_v50.img: Image_wd24 + ten sam v12.dtb; SHA25689be48726adf9f5edf8e74630fe629dba4fe279f32134ebf9ca87fbd5b314878. PEŁNY alternatywny patch15plików y5-kernel-7.3-v50.patch, config jakv49. Build, dry-run patcha i weryfikacja obrazu PASS; BOOT v50 JESZCZE NIE TESTOWANY. v49 pozostaje obrazem potwierdzonym.
- Następne: właściciel testujebootv50/temperaturę/regresjęprzycisków; analiza mapy HL7005D i logiki stockdriver, kernelowy odczyt statusu, dopiero później konfiguracja ładowania. Nadal brak capacity/prądu baterii i pełnego PMIC/IRQ/suspend.

## 2026-10-25: v50 potwierdzony, mfw_v51.img gotowy; aktualizacja USB przygotowana
- Właściciel potwierdził v50; powłoka: Linux7.3 #34, temp277, napięcie3843841uV. Nowe nazwy obrazów od teraz mfw_vNN.img (pierwszy mfw_v51.img), stare nazwy zachować do odzyskiwania.
- HL7005D: świeży pomiar ID0x41, status0xf6 (STAT3, fault6), control0x70, special0x24. PMIC CHRDET pole0x4c1 =0xa88 bit4; raw0x1b przy USB, online1. Unbind/bind z podpowłoką przywrócił sterownik (temp279).
- v51 dodaje microfw-usb online/status/model_name, battery/status z HL7005D i charger_registers. Odczyt PIO, bez konfiguracji ładowarki i bez karmienia watchdoga. Uwzględnione CE/HZ/CDIS, nie samo zezwolenie PWM. Kod6 to timer fault; dalsze zarządzanie ładowaniem pozostaje do zrobienia.
- CPU_FREQ/CPU_IDLE brak. Odczyt z kodu4.4 i hardware: ARMPLL@1000c204=0x811244ec, TOPCKGEN_AO@1001b000 mux0x100 i div@+0x2c=0. Nominalnie949999755Hz (~950MHz). Dodano read-only provider CCF mfw_cpu oraz cpu_rate_hz w platform/mfw-cpu-clock. Nie zmieniano PLL/dzielników/napięć; to jeszcze nie DVFS. Usunięto stare deklarowane clock-frequency z DT, wstawiono provider do czterech CPU.
- v51: Image_mfw_v51, v13.dtb, bm_mfw_v51.py. SHA256 8cb66e3f9a997219f1127828860629f7d5e4a86237d076e26c6bd332fb108fe8. Pełny patch20 plików mfw-kernel-7.3-v51.patch, config-7.3-mfw-v51 (SYSCON i provider). Build, UBSan/testy matematyki i wszystkich kombinacji wyłączenia ładowarki, dry-run patcha oraz weryfikacja obrazu PASS. BOOT v51 JESZCZE NIE TESTOWANY.
- Na pytanie o update bez BROM przygotowano mfw_update_boot.py: domyślnie check, stage robi prywatną kopię boot i RAM upload, write oddzielny, p36 na sztywno, fsync/sync i pełny readback obrazu przed opcjonalnym reboot. Pięć testów PASS; check i stage wykonane na v50. Kopia pełnych24MiB zweryfikowana w D:\y5_linux_work\boot_update_backups (NIE do repo/czatu), obraz RAM zweryfikowany. Gotowy skrypt out/mfw-write-boot.sh przeszedł sh -n, NIE uruchomiono zapisu ani reboot.
- Oczekuje pytanie do właściciela o zgodę na pierwszy zapis boot z MICROFW i restart do gotowego v51, ponieważ pierwotnie ustalił ręczny BROM i zgodę przed zapisem eMMC. Bez tej zgody NIE wykonywać --write. Po autoryzacji komenda w docs/MFW_V51_UPDATES.md; BROM pozostaje odzyskiwaniem.
- Szczegóły i ścieżki: docs/MFW_V51_UPDATES.md. Następny test: v51 sysfs ładowarki i CPU, temperatura i przyciski; potem zarządzanie HL7005D/PMIC i początki UI.

## 2026-10-25: zgoda i zapis v51 bez BROM
Właściciel odpowiedział 'zapisujmy!' na konkretne pytanie o zapis mfw_v51.img do boot przez USB i restart. To autoryzuje bieżący zapis p36 oraz restart zamiast ręcznego BROM, bez rozszerzania na inne partycje.
Aktualizator ponownie wykonał preflight, prywatny backup całej partycji boot (24 MiB) oraz staging do RAM z kontrolą sum. Zapis do p36 i odczyt całych 8794112 bajtów nowego obrazu zakończyły się poprawnie: Boot write and complete image readback SHA256 verified. Wysłano reboot. Pierwszy ping tuż po restarcie 0/2; wynik rozruchu v51 jeszcze do sprawdzenia.

Po restarcie v51 kolejne ping 0/2; odczyt Windows: brak widocznej karty RNDIS i brak IPv4 z192.168.7.x. Nie ustalono przyczyny braku USB. Potrzebne zdjęcie ekranu od właściciela. Nie wykonywano kolejnego flashowania ani rollbacku. Zapis v51 i readback SHA256 są potwierdzone; boot nadal niepotwierdzony.

## 2026-10-25: aktualizacja USB — zatrzymanie przy restarcie
Właściciel widzi 'requesting system reboot' i stojący ekran. To wskazuje najprawdopodobniej na zatrzymanie restartu starego v50, przed uruchomieniem zapisanego v51; nie jest jeszcze dowodem awarii rozruchu v51. Zapis obrazu i pełny readback SHA256 wcześniej potwierdzono. USB nadal nie odpowiada.
Właścicielowi zalecono wymuszenie restartu długim przytrzymaniem Power. Huawei podaje Power >=10 s jako ogólną metodę; wariant Power+VolumeDown dotyczy ich nowszych systemów, nie jest potwierdzony dla DURA-L21.
Odczyt lokalnych źródeł (bez modyfikacji): PSCI register_restart_handler priority129; mtk-wdt priority128. Obie funkcje są włączone w config. PSCI jest wywoływany najpierw; gdyby firmware utknął w SYSTEM_RESET, watchdog nie zostanie wywołany. To HIPOTEZA mechanizmu zawieszenia, nie pomiar wywołania. mtk_wdt_restart przełącza tryb resetu zamiast IRQ i pisze SWRST w pętli. Po odzyskaniu telefonu sprawdzić obsługę restartu; nie zmieniać teraz zapisanego v51 bez diagnozy rozruchu.

## 2026-10-25: mfw_v51 POTWIERDZONY po aktualizacji USB i ręcznym restarcie
Właściciel zgłosił, że telefon wstał po wymuszeniu restartu. Powłoka potwierdza Linux microfw7.3.0 #35, build Sun Oct25 00:51:26 CEST2026, uptime30s. Wcześniej v50 miał build #34. Logi nowych sterowników i ich sysfs potwierdzają uruchomienie v51, nie tylko zapis obrazu.
Zmierzone przy pierwszej kontroli: temp288 (28.8°C), voltage_now3876800uV; microfw-battery/status=Charging; microfw-usb/model_name=HL7005D, online=1, status=Charging. Surowe rejestry: id0x41, status0xd0, control0x78, special0x04, fault0, online1. To bieżący stan układu; nie potwierdza długotrwałego ładowania ani pomiaru prądu. Wciąż nie dodano zapisu parametrów HL7005D/karmienia jego watchdoga.
CPU nominal949999755Hz (~950MHz), provider mfw-cpu-clock wystartował przy0.337s, HL7005D przy0.405s; brak DVFS zgodnie z implementacją. USB/powłoka i odczyty baterii działają. Nie powtarzano jeszcze osobnego testu regresji przycisków na v51; urządzenie zgłasza stan spoczynkowy wszystkich3.
Bieżący potwierdzony obraz to D:\y5_linux_work\mfw_v51.img; v50 zachować awaryjnie. Aktualizacja boot bez BROM i jej readback zweryfikowane na urządzeniu. Problem restartu programowego nadal otwarty: po reboot z v50 było 'requesting system reboot', konieczny Power. Nie traktować tego jako uszkodzonego boot v51. Następny etap: naprawa restartu i dalsze HL7005D/PMIC oraz początki UI według kolejnego polecenia właściciela.

## 2026-10-25: następny priorytet — rozwój hardware i sterowników
Właściciel pyta o dalsze prace i wskazuje rozwój hardware/driverów. Plan kolejności: (1) niezawodny restart, (2) HL7005D i watchdog/parametry ładowania oraz pomiary baterii, (3) właściwe PWRAP/MFD/IRQ/regulatory MT6357, (4) zegary MT6739 i później CPUFreq/DVFS/idle po weryfikacji VPROC/VSRAM oraz OPP, (5) I2C/GPIO/pinctrl, dotyk/podświetlenie/LED/latarka/czujniki; potem dalsze DRM/USB DMA/audio/łączność. Bootstrap v51 nie zastępuje pełnych sterowników tych bloków.
Dodatkowy pomiar v51 przy uptime2:20: battery/status=Not charging; temp287, voltage3870867uV; chwilę później charger_registers=id0x41 status0xd0 control0x70 special0x24 fault0 online1. CPU949999755Hz. Status i raw pobrano kolejno, nie atomowo względem siebie; nie traktować rozbieżności jako dowodu stałego braku ładowania lub błędu dekodera.
W porównaniu z pierwszym odczytem v51 (control0x78, special0x04) zmieniły się control/special pomimo braku zapisów konfiguracji w naszym sterowniku. Możliwy związek z 32-sekundowym watchdogiem i autonomicznym powrotem ustawień HL7005D; to hipoteza do sprawdzenia kontrolowanym monitoringiem. Najpierw zapisać pełny zestaw i czas zmian rejestrów, sprawdzić stockową obsługę watchdoga; nie zgadywać prądów i napięć. Log potwierdza rejestrację mtk-wdt z timeout30s; userland wyłącza timer, lecz restart-handler pozostaje istotny.

## 2026-10-25: v52 przygotowany wyłącznie do restartu
Właściciel ustalił kolejność: restart, po sukcesie ładowanie. Przygotowano board-specific zmianę mtk-wdt priority192 przed PSCI129. Warunek root compatible huawei,dura-l21; DT zachowuje też mediatek,mt6739. Inne platformy nadal128. Nie zmieniono kodu samego SWRST, konfiguracji WDT, ładowarki ani pozostałych sterowników. Hipoteza utknięcia w PSCI nadal wymaga testu.
Build i Image PASS, nowy komunikat restart-priority192 obecny w vmlinux, fdtget v14 kompatybilność PASS. Config identyczny z v51. mfw_v52.img (8794112B), Image_mfw_v52 (12828680B), v14.dtb, SHA25610f1bd8e0c23491ce13b3cbdf062fd4698be13ebc50bad412baae5953d535d90. Pełny alternatywny patch21plików mfw-kernel-7.3-v52.patch i dry-run PASS; weryfikator obrazu PASS.
Wykonano tylko --stage: nowy prywatny backup boot24MiB i RAM upload/checksum PASS. ZAPIS/RESTART v52 JESZCZE NIEWYKONANY; v51 nadal aktualny i potwierdzony. Przed zapisem gotowego v52 potrzebna zgoda zgodnie z zasadą właściciela dotyczącą eMMC; wcześniejsza zgoda była na v51.
Test dwuetapowy: reboot przy przejściu51->52 nadal wykonuje stary kernel i może wymagać długiego Power. Dopiero po wejściu w52 i potwierdzeniupriority192 testować zwykły reboot nowego kernela. Odróżnić długie Power wymuszające reset od krótkiego Power na ekranie LK. Ładowanie dopiero po potwierdzeniu restartu. Szczegóły: docs/MFW_V52_RESTART.md; skrypty w archive/pc_work.

## 2026-10-25: autoryzacja zapisu i testu v52
Właściciel odpowiedział 'robimy oczywiście' na pytanie o zapis gotowego mfw_v52.img i test restartu. Uruchomiono mfw_update_boot.py --write --reboot; wynik i test zostaną dopisane. Zgoda obejmuje ten zapis boot i test zwykłego restartu po potwierdzeniu, że działa nowy v52. Przy pierwszym przejściu reboot wykonuje jeszcze stary v51 i może wymagać długiego Power. Ładowania nie zmieniać przed sukcesem restartu.

Zapis v52 i pełny readback SHA256 PASS; reboot ze starego v51 wysłany. Pierwszy ping0/2, oczekujemy na fizyczny restart/ekran od właściciela. Właściwy test poprawki dopiero po potwierdzeniu uruchomionego52 i TOPRGUpriority192. Ładowanie nadal bez zmian.


V52 uruchomiony: właściciel zgłosił System już wstał, a powłoka potwierdziła Linux7.3.0 #36 (01:14:04), uptime10s. Root compatible zawiera huawei,dura-l21 oraz mediatek,mt6739. Dmesg potwierdza DURA-L21: TOPRGU restart priority192 (before PSCI), więc warunek płytki i zmiana priorytetu rzeczywiście działają. NTC288 i CPU949999755Hz bez regresji odczytów.
Wykonano następnie właściwy test sync; reboot już z v52 (bez ponownego zapisu obrazu). Właściciel dostał prośbę o obserwację bez długiego Power; wynik restartu jeszcze do ustalenia.

## 2026-10-25: restart v52 POTWIERDZONY — przejście do ładowania
Właściwy test sync; reboot z uruchomionego v52 sam doprowadził do czerwonego ekranu LK. Właściciel potwierdził: trzeba krótkie Power na LK, po czym system wystartował; nie był potrzebny długi Power wymuszający reset. Powłoka po tym teście: Linux7.3 #36, uptime27s, ponownie widoczny TOPRGUpriority192. Test resetu SoC PASS; krótkie potwierdzenie LK pozostaje wymaganym ręcznym etapem bootloadera, nie awarią reboot. Nie próbować zmieniać LK ani mechanizmu weryfikacji.
Odczyty po restarcie: HL7005D id0x41/status0xd0/control0x70/special0x24/fault0/online1; temp291, napięcie3876141uV. Bieżący potwierdzony obraz: mfw_v52.img. Zgodnie z poleceniem właściciela po sukcesie restartu przechodzimy do diagnostyki i obsługi ładowania, zaczynając od watchdog32s oraz zachowania rejestrów. Nie zakładać jeszcze stabilnego długotrwałego ładowania.

## 2026-10-25: HL7005D watchdog, CE/TE i kandydat v53
V52 nadal działa (#36), ostatni odczyt NTC293. Krótkie Power wymagane na LK potwierdzone ponownie przez właściciela. Badanie profilu HL7005D: d0/70/8e/41/81/24/7b (reg0..6); napięcie docelowe4.20V, wejście500mA. Bez pomiaru prądu baterii. Dziesięć impulsów reg0=c0 co8s, przez80s: fault0, profil niezmieniony, wynik0. Po zakończeniu obsługi zaobserwowano f6/TimerFault6. Oddzielny test reg1=78 ->7c ->70 z readback PASS; przywrócono sterownik v52. Dowód phone/hl_watchdog_result.txt, kod stocka phone/ghidra/k44_hl_watchdog.c.
Przygotowano v53: worker8s TMR_RST i CE/TE, wyłącznie wąski rozpoznany profil4.20V/100lub500mA, NTC10<=T<45C, limit sesji12h. Brak zapisu napięcia/prądu/safety, brak pełnej JEITA/fuel gauge. Błędy magistrali/nieznany profil pauzują manager; nie są gwarancją wyłączenia ładowarki. Testy UBSan masek/profilu/granic PASS, build/config/boot verifier/dry-run patch22plików PASS. DTv14 i initramfs jakv52, restart fix zachowany.
Obraz mfw_v53.img 8796160B, SHA256 09d14a4e62b8d953ab56fc0ab819e5586d16cae961ecd0a9b9edc5617a258fd5. Wykonano staging do RAM i prywatny backup boot; zapis v53 JESZCZE NIEWYKONANY. Wymagana zgoda przed eMMC według zasady9; v52 pozostaje ostatnim potwierdzonym. Procedura, źródła, ograniczenia i kryteria akceptacji: docs/MFW_V53_CHARGING.md. Nie pomylić zakończonych natywnych eksperymentów z nieprzeprowadzonym testem kernela v53.

## 2026-10-25: zgoda na v53 i ograniczenie nocne
Właściciel zatwierdził zapis v53: „wgrywamy”. Najpierw zabronił nocnych restartów; potem doprecyzował: jeden test teraz, gdy jest jeszcze przy PC, jest dozwolony. Autoryzacja obejmuje zapis przygotowanego v53 i jeden restart do testu. PÓŹNIEJ PRZEZ NOC ŻADNYCH RESTARTÓW, WYŁĄCZANIA, SUSPEND ANI DZIAŁAŃ WYMAGAJĄCYCH POWER/ODŁĄCZANIA USB. Kolejne obrazy można przygotować; aktywowanie i testy dopiero z właścicielem rano. Trwa aktualizator --write bez automatycznego --reboot; reset zostanie wysłany osobno dopiero po pełnym readback PASS.

## 2026-10-25: v53 uruchomiony, noc BEZ RESTARTÓW

Właściciel zatwierdził zapis i jeden test, gdy był jeszcze przy PC. --write bez --reboot zakończył pełny backup/zapis/readback SHA256 PASS. Następnie oddzielny sync; reboot (ostatni dozwolony tej nocy), krótkie Power na LK wykonane przez właściciela. Uruchomiony Linux7.3 #39, build01:54:21 CEST. Od tego momentu właściciela nie ma przy telefonie: żadnych kolejnych restartów/poweroff/suspend/resetów ani testów wymagających USB/Power.

Istotna obserwacja: LK po restarcie zostawił control78/special04/oregb6/ibatf1/safety7b. W 19. sekundzie kicks0/errors3/-ERANGE, zgodnie z guardem nieznanego profilu; manager niczego nie przepisywał. Około pierwszych32s timer sprzętu powrócił do profilu domyślnego8e/81/24. Próbka47s: kicks2, errors4, timer_faults_seen1, last_error0, profile_ok1, blocks0, control78, fault0. To NIE była awaria rozruchu ani trwała pauza sterownika.

Dalsze 12 próbek do uptime236s: kicks25, errors nadal4, timer_faults_seen nadal1, fault0, NTC290, VBAT3882073uV. Odczyt przy uptime414s: kicks47, errors4, timer_faults_seen1, last_error0, profile_ok1, blocks0, NTC291, VBAT3886029uV, statusCharging. CPU949999755Hz, input event0 nadal z trzema klawiszami. To potwierdza kilka minut obsługi, nie pełny cykl ładowania ani pomiar prądu. Wczesny TimerFault przy przejściu do domyślnego profilu jest znanym ograniczeniem bootstrap; przyszła inicjalizacja wymaga osobnego, zweryfikowanego projektu. Nie rozluźniać guardów do wyższego napięcia b6.

Dowody bez identyfikatorów: phone/v53-first-boot.txt. Monitor PC mfw_night_monitor.py (archive/pc_work) działa co60s przez10h; zbiera tylko sysfs do D:\y5_linux_work\out\mfw-night-telemetry.jsonl i mfw-night-latest.json. Nie wykonuje żadnych zapisów na telefonie. Zatrzymanie: utworzyć lokalny plik out/mfw-night-monitor.stop. PID w out/mfw-night-monitor.pid. PC musi pozostać uruchomiony; nie zmieniano ustawień zasilania Windows.

## 2026-10-25: kontynuacja po nocy, DE i pomiary
Właściciel wrócił do telefonu i ponownie pozwala na restart/testy. Żadnego nowego restartu jeszcze nie wykonano. UI: C/C++, prawdziwe DE, angielski, znacznie mniejsza skala wszystkiego, normalny desktop zamiast home screen. Fullscreen aplikacji domyślny, gesty/multitouch i pasek dolny, powrót do zwykłych nakładających się okien ze standardowym toolbarem Waylanda. Zastępuje to wcześniejsze ustalenia odkładania trybu okienkowego. Prototyp C/Wayland oraz tracker wielu kontaktów w phone/ui; testyPC nie oznaczają działającego DE ani dotyku na telefonie.
Nocne579prób/546poprawnych/33błędy odczytu, 10h, bez spadkówuptime w próbkach. Watchdogkicks17->4408, errors4 i timer_faults_seen1 bez wzrostu, fault0. NTC28.3–29.3C, VBATmax4.256652V i nadalCharging; brakFull, brak pomiaru prądu. Wyjaśnić kalibracjęADC vs CV8e=4.20V. Uruchomiono dalsze2h odczytów do testu granicy12h; wynik jeszcze nieznany. Zegar telefonu1970, brakRTC. Pełny wynik, eksport tablicGPIO DO/DI/DIR, błędny trop innych slotów i GPU/BVNC: docs/NIGHT_WORK_2026-10-25.md. ŹródłaPC wyeksportowane, prywatne obrazy/backupy pozostają poza repo.

## 2026-10-25: zegar UTC i strefa Warsaw bez restartu
Telefon miał1970-01-01 i brakRTC. Narzędzie tools/mfw_sync_time.py --set ustawiło zegar ścienny z aktualnego UTC komputera; readback różnił się o-2s. Nie zmienia jiffies ani licznika sesji ładowania. Skopiowano publiczny plik zoneinfo Europe/Warsaw z WSL do /etc/localtime i zapisano /etc/timezone na rootfs; wcześniej oba pliki nie istniały. Odczyt telefonu: 2026-10-25 13:07:39 CET +0100, uptime41563s, elapsed_s41558, kicks5070/errors4/faults_seen1. Restart nadal niewykonany. Strefa przetrwa na rootfs, zegar UTC po nowym boot nadal może wrócić do1970, dopóki brakRTC/NTP; narzędzie PC należy wtedy wykonać ponownie. Skrypt instalacji strefy wyeksportowany do archive/pc_work/set_local_timezone.sh.
Prototyp desktopu: make test previews po integracji pełnego trackera wielu kontaktów PASS; -Wall -Wextra -Werror bez wyciszania ostrzeżeń. Brak fizycznego testu dotyku i zarządzania obcymi oknami pozostaje jawnym ograniczeniem.

## 2026-10-25: natywny desktop zarządza rzeczywistymi oknami na PC
Dodano tryb --desktop: trzy powierzchnie layer-shell (tło/status/home), lista rzeczywistych foreign-toplevel, fullscreen nowych aplikacji i kontrola aktywacji/minimalizacji/zamknięcia. Overview pokazuje prawdziwe tytuły/stan okien; klik aktywuje lub zamyka wybrany handle. Settings otwiera się jako oddzielny natywny klient C, Terminal uruchamia weston-terminal naPC. Files pozostaje nieaktywnym placeholderem. Standardowe dekoracje dostarcza labwc, bez własnego malowanego toolbara.
Test labwc0.20.2/wlroots0.20.2/pixman z dwoma rzeczywistymi klientami: fullscreen -> przedstawienie overview ponad zasłoniętym tłem (potwierdzony callback) -> restore -> minimize -> activate -> maximize -> unmaximize -> close PASS. Stan każdej operacji potwierdzały zdarzenia kompozytora; close zakończyło rzeczywistego klienta. Wyłapano błąd pierwszego testu: callback zasłoniętego tła może nie przyjść nigdy; zmiana warstwy musi zostać zatwierdzona osobnym commit przed oczekiwaniem na callback. Test regresji to obejmuje. Drugi testC++17 wywołujący rzeczywiste CAPI modelu i kontaktów PASS. Transportxdg-shell i testyUBSan kontaktów równieżPASS. Dowód bez tytułów terminala/nazwyPC: phone/ui-desktop-integration.txt.
To działający początek DE naPC, nie gotowy system telefonu. Brak pakietu Alpine/aarch64, fizycznego dotyku, thumbnail capture, ogólnego launchera i pełnych globalnych gestów w obrębie aplikacji. Insety pasków przy fullscreen oraz reguły transient/dialogów do dopracowania; polityka bootstrap obecnie rozciąga każdy nowy toplevel. Własnego kompozytora projektu jeszcze nie napisano: używany jest labwc (C/wlroots). Kod powłoki nativeC, API kompatybilneC++, bez interpreteraPython w runtimeUI. Telefon nadal na v53, żadnego kolejnego resetu nie wykonano.
Brakujące paczkiPC glycin2.2.3-1 i librsvg2:2.63.3-1 pobrano w dokładnych wersjach z oficjalnego archive.archlinux.org wraz z podpisami; normalny pacman potwierdził integralność i podpisy. labwc zainstalowany. Nie wyłączano weryfikacji zależności/podpisów i nie robiono ogólnego upgrade systemu. Reprodukcja: archive/pc_work/{fetch_librsvg_pc.py,prepare_ui_deps_pc.sh}; binaria paczek poza repo.

## GPU: konkretny trop BVNC ze stockowego kodu
Eksporter odnalazł jedyny null-terminated string22.49.21.16 pod VA0xffffff8008e7a006 w stock_Image oraz buildDDK Rogue_DDK_Android rogueddk1.9@4893595. To wariant kompilacyjny/fallback, NIE potwierdzony odczyt rejestru GPU. Stock DT: mediatek,AUCKLAND, MMIO13000000/80000, IRQ SPI226/level-low przez stocksysirq, clock-frequency570MHz wDT (nie pomiar), mfg_cfg13ffe000, zależności CLK_TOP_MFG/OFF/ON iCG_SCP_SYS_MFG0/1. Nie aktywowano GPU ani jego zegarów/napięć. Materiał phone/hardware/gpu-stock-evidence.json, dekompilacja phone/ghidra/k44_gpu_bvnc.c. Funkcja rgx_bvnc_pack_hw mimo nazwy tylko pakuje przekazane parametry, nie jest dowodem czytaniaMMIO. Następne: prześledzić pobieranieGPUId i wybór firmware, dopiero potem zweryfikować sprzęt przy prawidłowym zasilaniu; nie podstawiać innegoBVNC ani firmware na zgadywanie.

## 2026-10-25: RZECZYWISTY limit sesji12h PASS, bez restartu
Próbka elapsed43188: blocks0/control78/Charging/kicks5269. Próbka43204: blocks2/control7c/Not charging/kicks5271. Kolejne próbki do43294: blocks2, CE nadal wyłączone7c, kicks rośnie do5282, errors nadal4, timer_faults_seen nadal1, last_error0, fault0. Obserwacja po wyłączeniu przekroczyła32s watchdoga. To pomiar na telefonie, nie tylko unit test. VBAT spadł do4.169641V, NTC32.3C; nie interpretować tego jako pomiaru prądu lub dowodu kalibracjiADC. Dowód phone/charge-limit-12h.json i kod obserwacji archive/pc_work/observe_charge_limit_12h.py.
Ładowanie pozostawione zatrzymane zgodnie z limitem. Właściciel dostał prośbę o odłączenieUSB na20s i ponowne podłączenie bez wyłączania, aby sprawdzić CHRDET/reset sesji/wznowienie. Wynik tego osobnego testu JESZCZE NIEZNANY. Nie symulować odłączenia resetem kontrolera. Telefon nadal v53, bez kolejnego reboot.

## 2026-10-25: fizyczne odłączenie/podłączenie USB po limicie
Właściciel odłączył kabel na około20s i podłączył ponownie bez wyłączania telefonu. Świeży odczyt: uptime43448s (bez reboot), elapsed_s16, blocks0, profile_ok1, control78, online1, statusCharging, kicks5297, errors4, timer_faults_seen1/fault0. Sesja została wyzerowana i ładowanie wznowione. Nie odczytaliśmy online0 podczas braku połączenia: dowodem odłączenia jest czynność właściciela oraz nowy licznik sesji przy ciągłym uptime. Dłuższy odczyt powtórny trwa, wynik zostanie dopisany. Nie wykonywano restartu kontrolera ani kernela.

## Narzędzie USB: potwierdzony błąd markera zakończenia
Dłuższy odczyt po reconnect przerwał się z RuntimeError telemetry failed, ale świeży stan telefonu/ładowania był poprawny. Odtworzono przyczynę w trzecim kontrolowanym wywołaniu: phone_shell.run zwrócił rc='$?' i complete=True, ponieważ pump uznał echo komendy echo __Y5END__ $? za rzeczywistą linię wyniku. To błąd klientaPC, nie dowód awarii sieci lub sterownika. Może tłumaczyć część33 błędów nocnego loggera; nie mamy surowych wyników wszystkich33, więc nie przypisywać mu automatycznie wszystkich.
Poprawka tools/phone_shell.py czeka na pełną linię __Y5END__ <liczba0..255> zakończonąLF; sam prefiks/telnetecho ani częściowo przesłany kod127 nie kończą odczytu. Trzy testy regresji (echo, segmentacjaTCP/kod127, poprawne kody i odrzucanie błędnych) PASS. Dowód phone/shell-marker-failure.json i kod probe_shell_completion.py w archive/pc_work. Nie zmieniono API run ani mechanizmu wykonywania komend. Aktualizator boot nie został uruchomiony ponownie.
LoggerPC dostał lokalny plikstop do łagodnego zamknięcia starego procesu, aby kolejny uruchomić już z poprawionym modułem. Nie jest to restart telefonu. Fizyczny retest ładowania na nowym kliencie trwa.

Retest po poprawce klienta PASS: cztery próbki nowej sesji253->303s, kicks5326->5332, errors4 i timer_faults_seen1 bez zmian, control78/online1/blocks0/Charging/fault0 w każdej próbce, uptime>12h. Dowód phone/charge-reconnect.json. Poprawiony loggerPC uruchomiony na dalsze2h (bez żadnych automatycznych działań na telefonie).

## 2026-10-25: natywny ARM64 UI, SHM, dotyk i radio — praca trwa
Właściciel poleciłUI/GPU/touch oraz badanieWiFi/BT/modemu; zero-copy później. Wykonano natywną instalację podpisanych paczekAlpine i buildARM64/musl CUI zAPItestemC++. Naprawiono brak/dev/shm trwale wrc.sh. Wayland3warstwy commit/callback720x1440 i fizyczny bufor desktopu potwierdzone; widoczność na PANELU w dłuższym teście nadal do potwierdzenia przez właściciela (wcześniejsze odpowiedzi wskazywałykonsolę). Dotyk: GPIO119resetlow,GPIO6IRQhigh,VLDO28off/2.8V. OdczytPWRAP wymagał chwilowegounbind/rebind, co wyzerowało statystykiPMIC bezreboot. Buduje się kernel54 zUINPUT iI2CCHARDEV, nie wgrany. Nie aktywowanoGPU/radia, NVRAMnietknięty. Szczegółowy checkpoint, błędne tropy i lokalizacje: docs/UI_TOUCH_RADIOS_20261025.md.

### 2026-10-25: desktop widoczny, flicker i praca bez restartów

## Aktualizacja: właściciel poza domem, bez restartów
Najnowsza dyspozycja właściciela: nie ma go przy telefonie; następny test ekranu dopiero gdy wróci. Bez reboot/poweroff/suspend, aktywacji v54 i nowych prób zasilania sprzętu. Dalsza praca: analiza offline i odczyty działających sterowników.
Właściciel zobaczył desktop podczas testu 10 min, zgłosił ogromne migotanie. Wyłączono cykliczne 25 Hz (y5disp/hz=0); NIE mamy jeszcze oceny, czy to pomogło. Test GUI zakończony. Nie interpretować wcześniejszych callbacków jako dowodu braku migotania. Konsola nie ma aktywnej pętli diag/y5txt.
Zoptymalizowano desktop.c: próbka telemetrii co sekundę nie odrysowuje już całego pulpitu, a jedynie status po zmianie danych; realne zmiany okien/strony nadal odrysowują widoki. Test integracji desktopu na PC PASS; natywny rebuild Alpine/aarch64 PASS. Ta wersja nie była jeszcze oceniona na panelu. Zero-copy nadal osobnym etapem.
Kernel v54 zbudowany (#40): INPUT_MISC=y, INPUT_UINPUT=y, I2C_CHARDEV=y, reszta sprzętowa jak v53. Image12828680B, boot8802304B; SHA256 mfw_v54.img: 146815f0426e2f980762fb287ed1131d66ba1d66262e9c3d835c48226876ab00. verify_mfw_v54.py PASS: te same DT/ramdisk/parametry boot co v53. NIE zapisano boot ani NIE restartowano telefonu. Config i skrypty w repo; obraz wyłącznie D:\y5_linux_work.
Odczyt kontrolny: kernel7.3.0, uptime13h34, hz0. Native build/rebuild-idle.log zakończony rc0. Reset liczników PMIC z wcześniejszego unbind/rebind opisano wyżej; nie był rebootem.
Offline potwierdzono w callerze mtk_pmx_set_mode: tabela soc_data+0x30 rzeczywiście steruje funkcją pinu. mtk_pinctrl_update_gpio_value bierze maskę z szerokości rekordu, więc dla GPIO6/119 jest to4bity; nie ogólna maska3bity. Brak jakichkolwiek nowych zapisów GPIO/regulatora dotyku.

Zegary: z stock_Image i callerów Ghidra odtworzono 6 PLL (mainpll,mfgpll,mmpll,univpll,msdcpll,apll1), 40 fixed-factor PDN, fixed26MHz, 24 mux i79gate. ARMPLL nie ma w tej tablicy PLL. To dane statyczne, nie pomiar częstotliwości ani gotowy sterownik. Szczegóły i ograniczenia docs/RADIOS_CLOCKS_BRINGUP.md. Radio nie aktywowane; NVRAM nietknięty.

### 2026-10-25: kamery, ALS/proximity, mikrofony i GPS bez restartu
Właściciel dodał te cztery obszary i zezwolił na testy kamer zdjęciem oraz próbkami sensorów/mikrofonów bez jego obecności przy ekranie. Nadal bez reboot; pliki próbek lokalnie poza repo. Nie przechwycono obrazu/audio: aktualny v53 nie ma /dev/video/media/snd/gnss/i2c, IIO, MEDIA_SUPPORT/SOUND/MODULES. Nie udawać sprawności sprzętu po kompilacji narzędzia.
LiveDT i stockinventory potwierdzają LTR577 I2C1/0x53, pollingALS/IRQPS. Ghidra: ID0x06 (gałąź0xb1), PS11bit0x08, ALS24bit0x0d, dodatkowy kanał0x0a, enableALS bit1. LTR501 ma inną mapę, nie obsługuje577. Native read-only diagnostyka C z kontrolą adresu/sterownika skompilowana -Wall/-Wextra/-Werror; poprawnie odmówiła na braku /dev/i2c-1. Nie zapisano sensorów, GPIO, regulatorów ani IRLED.
Kamery: z kodu odtworzono14 wariantów sekwencji zasilania; nie zidentyfikowano matryc fizycznych. Pierwsza próba platformowej tablicy została zatrzymana granicami pliku — adres jestruntime/BSS; tego nie dekodujemy jako stałych. Enumy napięć/pinów surowe. Jestmainline HI556/HI846/OV8856, brak gotowego toru SENINF/CAMSV MT6739. Kodek MT6357 ma już mainline capture, ale brakuje regmap/MFD PMIC, AFE/ADDA/DMA MT6739 i routingu mikrofonów. GPS zależy od CONNSYS/STP/WMT; statyczne symbole init nie są dowodem transportu. Anteny historycznie odłączone, obecny stan fizyczny niezweryfikowany.
Szczegóły, powtarzalne ekstraktory i selektywny eksport9węzłów: docs/CAMERAS_SENSORS_AUDIO_GPS.md. Test runner foto/mikrofon bez autostartu, z timeout i prywatnymi katalogami przygotowany; sh -n oraz --check PASS, nie uruchomiono capture. Telefon nadal7.3/v53, uptime14h03 w odczycie kontrolnym.

### 2026-10-25: dodano g?o?niki i pe?ny routing audio
Zakres w?a?ciciela obejmuje te? playback g?o?nika multimedialnego, s?uchawki rozm?w, s?uchawek oraz mikrofony. Bez potwierdzenia fizycznych tras/uk?ad?w. Zidentyfikowano stockowe cele Speaker_Amp_Change/Set, Ext_Speaker_Amp_Set, speaker_amp_i2c_probe i speaker_amp_type; nie s? dowodem zewn?trznego wzmacniacza. Szczeg??y docs/CAMERAS_SENSORS_AUDIO_GPS.md. Niczego nie odtwarzano ani nie restartowano. GPU: nadal software/pixman, ID ze stocku niepotwierdzone sprz?towo i brak gotowego wsparcia BVNC/firmware.

### 2026-10-25: desktop bez flickeru przy właścicielu

## 2026-10-25: panel bez migotania — potwierdzenie właściciela
Właściciel wrócił do telefonu. Uruchomiono ograniczony10min test run_ui_phone_event.sh, cały czas y5disp/hz=0, nowa binarka z ograniczeniem odrysowań w idle. Wszystkie3warstwy mają FRAME_DONE. Właściciel odpowiedział dokładnie: „Desktop widoczny, bez migotania”. To pierwszy potwierdzony wizualnie wynik tej wersji; nie jest dowodem działania GPU/touch ani płynności animacji pod obciążeniem. Nie rozdzielamy przyczyn między wyłączeniem25Hz a optymalizacją repaint, bo zmieniono oba. Nie wracać do cyklicznego25Hz jako domyślnego sposobu uruchamiania UI. Nowy skrypt zachowuje i przywraca poprzedniehz, ogranicza czas działania, nie rebootuje.
Kernel nadal v53/#39; uptime14h37, hz0 przed testem. Powrót właściciela znosi zakaz restartów z powodu nieobecności; zapis boot pozostaje osobną operacją według jego zasady9. V54 ponownie przeszedł verify_mfw_v54.py (Image/DTB/initramfs/cmdline/adresy i SHA1 boot). Do wgrania proponowany jako podstawa uinput dla dotyku, nie gotowy sterownik dotyku/I2C/sensorów.

### 2026-10-25: v54 i przełom fizycznego dotyku MStar
Właściciel wrócił, zaakceptował zapis v54 i reboot. Backup/readback/start #40 PASS, /dev/uinput obecne, ładowanie poprawne po fazie startowej; czas UTC ustawiony ponownie z PC. VLDO28 bitenable pulse/restore PASS, selector2.8V nietknięty. Goodix5d i dokładny stockFocal38/a3 NACK, ale LCDbias3e/reg0 odczyt0f PASS. Framework hw-ts/005d nie dowodzi producenta:4.4 zmienia client->addr per dostawca. StockMStar reset50ms+HIGH10/LOW10/HIGH25, komenda37 pod62 ACK; normalny26 też ACK. Nie wykonywano get_chip_type z kontroląflash/stopMCU ani firmwareupdate; get_chip_type ponadto ma fallbackbf.
Po ruchu palca pierwszy prawidłowy43B/5a/checksum0: raw702/1026,pressure6,slot0. Native C + uinput + runtimeDBlibudev dla własnego urządzenia + restart samego labwc. Właściciel: „Reaguje i trafia poprawnie”. 180s bridge bez blokowaniaPWRAP/ładowania; EXIT0,good306,bad14691, automatyczny poweroffdotyku/cleanup PASS. Dużo badframes i wymuszanyUP po5błędach to otwarte problemy; usunięto syntetyczneUP, nowy reader kończy się po trwałym błędzie kontaktu zamiast generować wielokrotne tapnięcia. Nativebuild/selftest PASS; poprawka jeszcze bez powtórki fizycznej. Nie dodano autostartu.
Właściciel doprecyzował częściowy flicker, rozdarcia w ruchu i stare fragmenty obrazu. Bezflickeru był tylko desktopidle! Hz0, GPU nadal nieczynne/pixman. Simpledrm kopiuje do aktywnegoFB, DSIbootstrap min40ms i brak synchronizacji to podejrzenie wymagające pomiarów. Najnowszy priorytet na prośbę właściciela: rzeczywistyGPU i zero-copydisplay. Pełny dziennik, wyniki i ścieżki: docs/MFW_V54_TOUCH_BRINGUP.md.

### 2026-10-25: prawdziwy tor OVL, video i v56
Na prośbę właściciela priorytet GPU i zero-copy. Odczyty pokazały OVL3 active(addr775eb000,pitch2880,size720x1440), RDMA direct-link(MODE_MEMORYbit1=0,pitch/addr0), więc pomysł wpisania adresu doRDMAmem nie pasuje do obecnego toru. Dotychczasowy simplefb775e8e38 był przesunięty o8648B. V55 zmienia tylko regFB+nazwęwęzła, resztaDT semantycznieidentyczna; kernel#40/initramfsbez zmian. Po jawnej zgodzie zapis/readback/start i zgodnośćliveDT/OVL PASS. CałyFB mieści się w reserved771c0000..77ffffff.
Ważna korekta: DSI_MODE_CTRLjest14, nie10! Wartość1 oznacza VIDEOsyncpulse. Stock dsi_wait_not_busy dla video od razu wraca, więc BUSY1 przez1000/1000próbek nie dowodzi zawieszenia. RAM-only magenta180x60:30s właścicielprzegapił/restorePASS;90s właścicielpotwierdziłmagenta/restorePASS, bez jakichkolwiek wyzwoleń/rejestrówDSI. Zatem istniejącyvideo sam czytaRAM. Tabela49modułów ireadback potwierdziły prawdziwymutex14001000(EN1,MOD41,SOF17ec0); legacy14015020czyta0. Stare y5disp stosowało błędnybootstrap i restartowało DSI przy atomiccopy.
V56#41: tylko y5disp — video omija ręczne/okresowe kicki, readonlysysfs mode. Legacycommandfallback niezweryfikowany; nie współdzielić go z właściwymDRM. Kernel12828680B,boot8802304B,SHA256cfdf62b447b1078a088fe608da51a4c4e4ab3c046cbfa6e11404252db688c8d2. Configidentycznyv54,DTv15/initramfs jakv55. Build,bootvalidator,komunikatwImage,pełny22plikowypatchdry-run naoryginalnym7.3 i staging/backup PASS. Na checkpointcie pytanie o zapis/reboot oczekuje, jeszcze bez aktywacjiv56.
GPU MFG iCORE0OFF: SPMstatuses30005f4c,CTRL338/33c=72,bity4/31wyłączone. Rdzenia13000000 NIE mapowano; nie zmieniano zasilania/PLL/ochronymagistrali. Odtworzono sekwencje4.4 do przyszłegogenpd/CCF, dalsza weryfikacja wymagana. BVNC22.49.21.16 wciąż kompilacyjnykandydat, zgodnośćfirmwareABI niepotwierdzona. Pierwszy kod walidacjiOVLGEMDMA w phone/kernel/display, testyUBSanPASS; NIE jest to gotowyKMS ani akceleracjaGPU/zero-copy. Dokładnepomiary i plan docs/DRM_GPU_BRINGUP.md.

### 2026-10-25: v56 potwierdzony, cache UI i rzeczywiste EOF RDMA
V56 (#41) zapis/readback/start PASS po jawnej zgodzie; wlasciciel nacisnal Power na LK. Tryb DSI video1, hz0, bez manualnych kickow. GPU pozostaje OFF, brak akceleracji/zero-copy. Biezacy obraz mfw_v56.img. Wlasciciel jest przy telefonie i dopuszcza restarty do dalszych testow.
Czytnik MStar 20ms bez sztucznego UP: rzeczywiste CONTACT_COUNT1->0, Settings otwarte krotkim tapnieciem. Test180s good88/bad6340/empty17/high_good0/low_good88. Nie dowodzi to bledu samego gate GPIO; zmieniono tez okres probkowania. Pozostaja problemy dotyku, nie jest to sterownik produkcyjny. Nastepny test600s zakonczyl sie EXIT0, podobnie supervisor UI; nie zakladac ze nadal dzialaja.
Cache pelnego tla Cairo (limit16MiB, bez translacji CTM) wdrozony na telefon. Paski nadal malowane pierwotna metoda, bo wariant cache paskow powodowal roznice1piksela przy skali0.75. Test72 wariantow porownania RGB z oryginalem PASS na WSL i telefonie; test-desktop PASS. benchmark renderowania bez compositora: desktop110041->12888us, switcher107355->10774us, control118406->21561us, settings119450->22213us. To cieply cache, NIE pomiar fps calego stosu. Backup binarki /opt/microfw/ui/build/microfw-shell.pre-cache. Wlasciciel: szybciej, ale nadal czasami obraz, dotyk i obsluga okien/gestow sprawiaja problemy. Animacji jeszcze nie napisano.
RDMA EOF: y5_rdma_eof_probe.c, tylko kasowanie INT_STATUS+4 zapisem0 (jak handler mainline), bez zmian enable/adresow/zegarow/DSI. Probka duration_ns1003840923/events60/avg_period_us16813/min15982/max20002/underflow_observations0. Wskazuje okolo60Hz zdarzen; rozrzut zawiera planowanie procesu. Brak underflow dotyczy tej probki. Narzedzia nie uzywac po przejeciu RDMA przez sterownik. Nastepny etap: prawdziwe DRM atomic GEM DMA, IRQ i bezpieczny czas zycia buforow; nie uznawac obecnego walidatora za dzialajacy driver.

### 2026-10-25: v57 IRQ observer przygotowany, jeszcze NIE uruchomiony
Przygotowano kernelowy mfw-rdma-observer: wylacznie obserwacja przerwan/ack INT_STATUS0, bez zmiany enable, trybu, adresu obrazu, DSI, zegarow i zasilania. Exclusywna rezerwacja MMIO i IRQ, jawny opt-in DT, sprawdzenie aktywnego direct-link/EOF. Sysfs counters: irqs/eof/sof/underflow/abnormal/last_status. Liczniki niezalezne (nie atomowy snapshot); poczatkowe statusy moga byc historyczne. Built-in only, brak unbind, nie stosowac modulowo ani razem z /dev/mem probe. To etap weryfikacji IRQ przed DRM, NIE KMS/pageflip/zero-copy ani driver GPU.
Stock DT: disp_rdma0@1400b000, reg1000, interrupts<0 202 8>. Rodzic odziedziczony sysirq (phandle1, intpol10200620), dalej GICv3. Bezposredni GIC odrzuca LEVEL_LOW, lecz tutejszy sysirq jest kontrolerem polaryzacji; nie omijac go. Mainline simple_kms_helper oznaczony deprecated; dla wlasciwego nowego DRM wybrac regularne atomic helpers.
V57 #42 build PASS; image12828680B, boot8802304B, SHA256a2511b04d5b37a88b8965269808120d6c9c8f5ac0891b1380d69287e0a34e058. Validator sprawdza bajtowo identyczny initramfs/config/adresy/cmdline oraz semantycznie jedyne dwie zmiany DT: compatible RDMA i property opt-in. Full23-file patch dry-run na pristine7.3 PASS. Wczesniejsza izolowana kompilacja obserwatora W=1 PASS. StagingPASS: prywatny backup calego boot24MiB i obraz w /tmp RAM; BEZ zapisu boot/restartu, telefon nadal v56. Wlasciciel ma dostac konkretne pytanie o zapis/restart zgodnie z zasada9.
Test po akceptacji: Power na LK, sprawdzic start#42, sysfs /sys/bus/platform/devices/1400b000.disp_rdma0/counters (ustalic dokladna nazwe przez glob), pomiar delty EOF w5s i /proc/interrupts, stan ladowania i brak nowych bledow. Nie zakladac sukcesu przed pomiarem. Podczas obserwacji nie uruchamiac y5_rdma_eof_probe.c. Sam licznik nie usuwa tearingu; pageflip wymaga jeszcze sprawdzenia latch OVL i bezpiecznego czasu zycia GEM/fences.

### 2026-10-25: v57 potwierdzony i usuniecie blokujacej telemetrii UI
Wlasciciel jawnie zaakceptowal zapis/restart v57. Aktualizator wykonal backup boot24MiB, zapis tylko boot p36, pelny readback SHA256PASS i reboot. Wlasciciel potwierdzil Power na LK/start. uname #42, uptime13s przy pierwszym odczycie. Obserwator IRQ dziala na /sys/bus/platform/devices/1400b000.disp_rdma0/counters; /proc/interrupts IRQ14, MT_SYSIRQ202 Level. Pierwsze EOF793/SOF793/IRQ1574; po5s EOF1093/SOF1093/IRQ2174: delta300EOF i300SOF w5s (60Hz kazde). underflow0/abnormal0, takze po kilku minutach przy uruchomionym UI EOF21594/SOF21594/IRQ43176. Statusy nie sa atomowym snapshotem; EOF i SOF moga laczyc sie w jednym odczycie. Brak bledow w tych pomiarach, nie dowod niezawodnosci wszystkich obciazen. y5disp mode1/hz0; ladowanieCharging/USBonline1/temp303. Zegar po reboocie1970 ustawiony mfw_sync_time.py --set, roznica odPC-2s. RTC nadal nie naprawione.
Wlasciciel doprecyzowal: aplikacje otwieraja sie z opoznieniem, gesty raz dzialaja raz nie, brak responsywnosci. Native bench mfw_sample20odczytow: avg55691us/min49953/max56173. Ten odczyt byl synchroniczny w glownym watku UI raz na sekunde. Dodano sampler.c/h: osobny pthread, sprzet poza mutexem, kopiowanie tylko telemetrii, zadnych Wayland/Cairo/windowstate w workerze, CLOCK_MONOTONIC do okresu i budzenie stop/join. Pierwszy odczyt startowy zostaje synchroniczny (nadal koszt startu aplikacji), animacji nie dodano.
Test z celowo zablokowanym samplerem:1000UIreads bez czekania, publikacja danych, zachowanie okien/page/scale i start/stop PASS (WSL UBSan oraz natywnie telefon). Test-desktop pelnej sekwencji okien PASS. Native snapshot200reads avg347ns/max1616ns, to NIE pomiar end-to-end reakcji UI i nie porownywac go z calkowitym czasem otwierania aplikacji. Build -Wall -Wextra -Werror PASS.
Wdrozenie rootfs bez reboot: /opt/microfw/ui/build/microfw-shell, backup microfw-shell.pre-async; status DEPLOYED_ASYNC. Rozpoczety nowy test600s touch+desktop (logs touch-supervisor-async.log/display-supervisor-async.log w /var/cache/mfw-ui), 3LAYER_FRAME_DONE i720x1440. Czytnik MStar dostal tylko diagnostyke bus_errors/invalid_reports/longest_contact_gap_us i monotonic_ms przy zmianie count; okres20ms/protokol/zasilanie bez zmian, selftest PASS. Wlasciciel dostal pytanie o fizyczna poprawe i gesty, wynik jeszcze oczekuje. Nie zakladac sukcesu poprawki przed feedbackiem. GPU nadal OFF, brak prawdziwego DRM pageflip/zero-copy; observerIRQ to potwierdzona podstawa dalszych prac.

### 2026-10-25: fizyczny test UI po async telemetry - poprawa potwierdzona
Wlasciciel odpowiedzial: "Wyraznie lepiej, gesty dzialaja" podczas testu UI+MStar na v57#42 z cache tla i osobnym workerem telemetrii. Potwierdza poprawe responsywnosci i dzialanie gestow w tej probie; nie dowodzi usuniecia wszystkich artefaktow obrazu ani globalnych gestow w dowolnej aplikacji. Czytnik zarejestrowal szesc sekwencji CONTACT_COUNT1->0, wszystkie irq_high0, good38/bad3823 na ostatniej zmianie (monotonic_ms439248). Brak ramek podczas bezczynnosci tez liczy sie obecnie jako bad, wiec nie traktowac tego licznika jako utraty3823kontaktow. Test600s nadal RUNNING na checkpointcie, podsumowanie TOUCH_TIMING bedzie dopiero po zakonczeniu. RDMA EOF26949/SOF26949/IRQ53886, underflow0/abnormal0; ladowanieCharging/temp303. Nadal brak aktywnego KMS pageflip/zero-copy/GPU; kolejny etap to latch OVL i bezpieczna obsluga GEM z potwierdzonym IRQ.

### 2026-10-25: port OVL, firmware GPU i v58 DMA test przygotowany
Wlasciciel polecil kontynuowac DRM+GPU, potem modem i caly hardware. Readonly OVL/DSI/LARB0 (bez RDMA/GPU/M4U): SRC8,DATAPATH81000001; NONSEC port0/1/2=2, port3..6=1,port7=0; legacyf00/fc0=0. Z tabel4.4 i module_to_m4u_port: module0OVL0->port0,7RDMA0->port1,10WDMA0->port2, wszystkieLARB0. m4u_config_port ustawia bit0NONSEC380+4port jako MMUenable. Zatem aktualny OVL0 port0 omija IOMMU; NIE wnioskowac tego z samych f00/fc0. DSI mode1,PSCTRL2c030870,VSA4,VBP20,VFP12,VACT1440,HSAwc112,HBPwc208,HFPwc204; porchbytecounts NIE sa od razu porchami DRM w pikselach. GPU wciaz OFF/nietkniety.
Oryginalny firmware znaleziony w vendor p41, montaz ro,noload oraz systemp42 ro,noload, po operacji oba odmontowane; sprawdzone /proc/mounts. TYLKO allowlista ogolnych sciezek GPU, bez konfiguracji urzadzenia/IMEI/NVRAM. rgx.fw.signed1265664B SHA2568a843eb1678c0e2fd630c66d4a8070a0a6d4ca5ba7bfd16904d89d66f9fa074c; ELF32LE/e_machine8(MIPS). Blob NIE skopiowany do repo ani PC. Kernel7.3pvr_fw_validate wymaga rozmiaru4K i info_version3 wFILE_SIZE-4096; ten plik as-is dajeLE33554432 i odpada na pierwszej bramce. To dowod braku zgodnosci tego pliku as-is, nie dowod ze samo opakowanie/dopisanie naglowka naprawi ABI! Pozostale pola trailer_candidate nie sa poprawnie zdekodowanym starym formatem. Testnarzedzia syntetycznego/headergate PASS; wynik odtworzony na oryginalnym pliku na telefonie. Stock loader fallbackrgx.fw.signed potwierdzony z dekompilacji/stringa.
GPU MTKEnableMfgClock: mt_gpufreq_voltage_enable_set(1), potem zegary. Funkcja napiecia zglasza vcorefs_request_dvfs_opp(8,opp); to wspolny VCORE/DVFS, nie zidentyfikowany osobny VGPU. Nie zmieniano napiec,PLL,muxow aniMFG/SPM. Mesa oficjalna lista z dnia testu nie wymienia22.49.21.16; jest to nadal stockkandydat, nieHWID. Linux7.3 binding nie ma zgodnego MT6739/GE8100compatible. Nie podstawic GX6250/GE8300. GitLab firmware link z dokumentacji zwrocilAccessDenied, nie obchodzono blokady. Dalszy GPU wymaga prawidlowego genpd/CCF/VCORE i odczytuHWID, a takze zgodnego firmware+obsugiwariantuMesa; nie obiecywac akceleracji przez samo CONFIG_DRM_POWERVR.
V58#43: gotowy ograniczony test DMA scanoutu. Dwie alokacje dma_alloc_coherent/PAGE_ALIGN4147200/32bit, trzymane do konca zycia builtin nawet przy timeout, brak ryzyka free aktywnego testowegoDMA. Klonowanie starego obrazu i kolor prostokat64..319/128..223; na jawnym flip_test=run: green5s->magenta5s->oryginal775eb000. Adres OVL3 zmieniany naEOF, potem2dodatkoweEOF do zakonczenia. To konserwatywny odstep testowy, NIE udowodniona semantyka latch/fence/finalny timestamp KMS! Guard:OVLonlylayer3/con21ff/720x1440/pitch2880,LARBport0MMUoff,DSIvideo1. Brak zmianDSI,trybu,zegarow/zasilania/GPU. W razie brakuEOF fallbackodtwarza oryginalnyadres (moze jednorazowo rozerwac obraz); buforow nie zwalnia. Brak alokacji lub zapisu adresu przy samymboocie; test tylko na jawne polecenie. Nie uruchamiac aktywnego compositor GUI podczas tej proby. Pierwsza wlasciwa rejestracja DRM/GEM/pageflip nadal przed nami; ten test ma zweryfikowac scanout/latch bez dodatkowych komplikacji.
KompilacjaARM64W=1 i UBSan sekwencera PASS; kernel#43/image12828680B/boot8802304B/SHA2569869148977f087d7d0b0c06114e9b0aea8f2e52462699868a7c0e25f2cb2d562. Validator: config/initramfs/cmdline/adresyidentycznev57; DTzmienia tylko regRDMA(z3dodatkowymimapami) i propertyoptin. Pelny24plikowypatch dry-runpristine7.3 PASS. Prywatny backupboot24MiB iRAMstagingPASS. JESZCZE BEZ zapisu/restartu; telefonv57. Potrzebna zgoda wlasciciela na zapisboot/restart/test przy panelu.
Poprzedni600s UI/touch async zakonczony EXIT0 obustronnie, power/lease cleanup bez bledu. good38/bad21391/empty6/high_good0/low_good38; bus_errors1/invalid_reports21390/longest_contact_gap_us28291. invalid_reports zawiera normalny brak gotowej ramki w bezczynnosci, NIE dowodzi21390gubionychkontaktow. Wlasciciel potwierdzil poprawe i dzialajacegesty. RDMA przy pozniejszymodczycie EOF89557/SOF89557/underflow0/abnormal0;Charging.

### 2026-10-25: v58 uruchomiony, fizyczny DMA scanout potwierdzony
Po jawnej zgodzie wlasciciela wykonano prywatny backup calego boot24MiB, zapis tylko bootp36 i pelny odczyt kontrolny SHA256PASS, reboot. Wlasciciel PowernaLK/startpotwierdzil. uname#43; w5s EOF640->940 iSOF640->940 (60Hz), underflow0/abnormal0. DSIvideo1,hz0,flip_state0 przedtestem; brak aktywnego seatd i lease dotyku. LadowanieCharging/USBonline1/temp303. UTCz1970 przywrocono zPC,offset-2s.
Jawna proba run_dma_flip_trial.sh z odliczaniem5s: alokacje dma_alloc_coherent0x44800000 i0x44c00000, kazda4147200B uzywanegoobrazu. ZdarzeniaEOF zmienily adresOVL3 na bufor0(zielony5s),bufor1(magenta5s),potem775eb000. Driverresult0/restore0/original_readback775eb000/flip_state2. Wlasciciel odpowiedzial: "Oba kolory i poprawny powrot". Fizycznytest potwierdza, ze obecnytorOVL->RDMA->DSI pobiera obraz z nowych DMAalokacji poza starym obszaremLK, bez manualnego restartuDSI. BuforystartowezostalysklonowaneCPUjednorazowo na potrzebyproby, NIE deklarowac pelnego zerocopyaplikacji ani akceleracjiGPU. Nie jest jeszcze zarejestrowany KMS/GEM/pageflip/fences.
Logkernel: buff0@31.996725s,buff1@32.032967s,start32.067708s,restore42.504768s. Przed probaEOF1596/SOF1596,po2521/2521; underflow0/abnormal0 przezprobe. Przydzialy pozostaja zaalokowane/pinned do kolejnegoreboot (celowe w pierwszymtescie, brakfreeaktywnychbuforow). Przytrzymanie2dodatkowychEOF jest konserwatywne, nieudowodnionefinalnetimingiKMS. Nastepny etap: wlasciwyDRM atomic/GEMDMA, referencjebuforow, odroczonyretire, event/fence/vblank i odsunieciesimpledrm; obecny test sysfs nie jest publicznym ABI sterownika. GPU nadalOFF; firmwareproblemy i VCOREopisane powyzej.

### 2026-10-25: akceleracjaGPU - CCF i zaleznosci portowania
Na zlecenie wlasciciela kontynuacja GPU przed reszta hardware. Utrzymany telefonv58#43; nie aktywowano nowego kernela ani GPU. Publiczny kodMT6739 z Power535/android_kernel_common_MT6763, commit47f925eac07c660002cb85939c94cd8a191e1c53, pobrano jako referencje do D:sources (bez remote/fetch/push w prywatnymrepo). Cztery pliki clk-mt6739.c,clk-mt6739-pg.c,mtk_gpufreq.c/h; SHA/provenance w phone/hardware/mt6739-gpu-source-provenance.json. LicencjaGPL2 wg naglowkow. To nowszy vendorcommon, nie dowod tozsamosci z DURAstock; waznepola porownane z binary4.4.
Nowa dekompilacjaRGXRegisterDevice pokazuje wyborBVNCzapphint/fallback22.49.21.16 i tabelfeatures, a nie dowod odczytuHWID. Linux7.3pvr_device.c odczytuje PBVNC0x20/64bit, fallbackCORE_ID0x18 iREVISION0x20/32bit. Nadal NIE czytano rdzeniaGPU.
Readonly alwayson snapshot:TOP_CFG1=01000000,parentindex1,gate_disabled0;SPMstatuses30005f4c/MFG338=72/core33c=72 (obaOFF);INFRA_SI0_CTL050000f0,PROTECT_ENABLE98,PROTECT_STA1fe1f99b8;MFGPLL_CON0181/CON10215ec4e/CON20015ec4e/PWR80000001. Wyliczony PLL569999694Hz, nie pomiar czestotliwosci. Port MFGmux zgodny z phone/top_muxes_44.txt:CFG50,set54,clr58,shift24,width2,gate31,UPD4bit7. Parent1mmpll_ck jest aliasem mfgpll w vendorcode; nie zmieniac samejPLL, moze byc wspoldzielona z innymi galeziami! Nowy kodclock zmienia wylacznie selektorMFG i jego gate.
Przygotowano clk-mfw-mfg-probe.c: maly CCF clockdo26MHzprobe, nie pelny clk-mt6739 ani DVFS. Recalc czyta aktualny PLL/mux; set_rate przyjmuje tylko26MHz i wymaga obu domenGPUOFF; enable odmawia innego parenta; disable nie gasi zegara przy aktywnej/niepewnej domenie. Brak zapisuPLL i napiec, CLK_IGNORE_UNUSED zapobiega automatycznemu wylaczeniu. Rejestracja nie powinna nic zmieniac w sprzecie; kod jeszcze NIE wImage/DT ani na telefonie. KompilacjaARM64W=1 PASS po dostosowaniuAPI7.3 z usunietego round_rate na determine_rate. UBSan PLLdecode tests PASS(569999694Hz,safeenable/isolation/dividers). Nie nazywac tego akceleracjaGPU.
Dalsza zaleznosc: CONFIG_PMnieaktywny, MTK_PMIC_WRAPwylaczony, brak MFD/regulatoraVCORE(tylko monitorPWRAP). Stock regulator_get("vcore") i DVFSzaleznosc potwierdzona publicznymkodem, PMICMT6357 mainline:VCOREDBG0=1522/selector7bit/min518750uV/step6250uV,enableCON01508. Przed probazasilania trzeba dodac serializowany odczytVCORE i sprawdzic rzeczywista wartosc; zadnych zmian wspolnego napieciaCPU/VCORE. Regulatorewentualny readonly wymaga uwzglednienia krotkiego unbind/rebindPMIC przez dotyk - nie trzymac stalego consumerref w eksperymentalnym proberze. Plan: bounded26MHz power/IDprobe z weryfikacjaSPM/SRAM/busprotect/timeout, bezfirmware/HWAPM, dopiero rzeczywistyBVNCwybiera drogefirmware+Mesa. Nadal brak zgodnego firmwareformatu3 dla stockbloba, brak renderowaniaGPU.

### 2026-10-25: v59 GPU identity probe przygotowany - jeszcze NIE wgrany
Dopisano kontrolowany testerzasilania/IDGPU, NIE akcelerator ani sterownikrenderowania. Trzy komponenty: CCF MFGprobe26MHz; serializowany readonly regulatorVCORE w istniejacymPMICmonitorze; jawny sysfsprobe w mfw-gpu-id-probe.c. Na boot wylacznie rejestracja/readonly pomiary, bez wlaczaniaGPU/zmianyPLL/VCORE. DT dodaje wylacznie trzywezlyVCOREchild/clock/probe i nowyphandlePMIC. Konfiguracja/initramfs/cmdline/adresybootidentyczne jakv58. NowyDEBUGvcore_status pokazujeDAselectorDBG01522,programmedELR0152a iCON01508; readonlyprovider nie ma opsset_voltage/enable/disable. Callback .get_voltage zablokowany p->lock jakinneodczytyPWRAP. Rejestracjajestopcjonalna i bladniezabija ladowania/klawiszy.
Jawnytestprobe_id=read-id: tymczasowy regulator_get(vcore), voltage1050000..1312500uV i enabled1, obiedomenyOFF/control72, selektorMFG26MHz, clkprepareenable, boundedSPMmain->core/SRAM/busprotection zgodniezpublicznymkodem i binary4.4. Dopiero przy potwierdzonympowerON/control0d mapowanieGPU13000000/4K i readonlyCORE_ID18/PBVNC20,odmapowanie,coreOFF->mainOFF i clockdisable. Brak firmware/HWAPM/jobs/IRQ/GPUwrite. Poczatkowy570MHzPLLpozostaje bez zmian; po udanejprobie selektorMFG26MHz i gategaszony (nie przywraca stanuLK z otwartym570MHzclockiem). To przejsciowybringup, niepelnyclkMT6739/genpd/DVFS.
Tester trzyma device_lockPMIC przez operacje, by krotkiunbind/rebinddotyku nie wylaczyl monitora w trakcie. Regulatorconsumerref tylko w czasie testu, putprzedunlockPMIC; nie blokuje stalego unbindprzez touchwrapper. Sysfs uruchamiacprzez run_gpu_id_trial.sh (brakseatd/lease,dotykuprobe rootlock). Kazdypoll100ms; na niepewnympowertransitionfaulted i zatrzymanie, zachowanyclock gdy domena moze bycON, bez slepegorollback; dalszeprobyblokowane do reboot. GPIO/charger/NVRAMniezmieniane. Sam odczyt rejestruGPU nadal wymaga fizycznego testu i nie jest dowodem zeuserland/firmwarezadziała.
ARM64W=1: clock/prober/PMICreadonlykompilacjePASS. UBSan: PLLdecode/live570MHzregcandidate569999694Hz, PBVNC+legacydecode/zero/allonesreject, parentcoreordering iwszystkie6powerup/7powerdownpolltimeoutstoppoints PASS (symulacja, nieHWID). PierwszycompilePMIC/probe wymagaljawnegoof.h. Image#44 SMPPREEMPT SunOct25 19:35:34CET2026,12896264B,boot8808448B,SHA256c623b3ae6128773af1d069ef0a1e17f3d90c1ca9319f7fc7e66d96ddf3fcd791. Pelny31plikowypatch dry-runpristine7.3 PASS. ValidatorDT/boot/config/ramdiskPASS po poprawieniukolizjinazwyzmiennejnewwvalidatorze (slowniknadpisywalblob; plikuobrazu toniezmienialo).
WSL pierwotnie odmowil startuDTC ERROR_NO_SYSTEM_RESOURCES; nastepnybuildkernelaruszylpoprawnie, DTCponowiony osobnoPASS. Windowsmialok.756MBwolnejpamiecifizycznej, virtualcommitwolny~31GB. Nie zabijanoprocesowuzytkownika/niemodyfikowanoWindows ani ustawienWSL. Backupboot24MiB iRAMstagingv59 PASS; telefon nadalv58, brakzapisu/restartu/GPUpowerrequest. Kolejnekroki: zgoda nazapisboot/restart i readVCORE; tylko przy prawidlowymnapieciu kontrolowany HWIDprobe. JesliVCOREnizejprogu, nie podnosicgozeby"odblokowac"test - najpierw prawidlowy regulator/DVFS/OPP.
Akceleracja nadalwymaga dopasowanejparyfirmware+kernel+Mesa dla rzeczywistegoBVNC. Stockrgx.fw.signed odrzuconyprzezLinux7.3headergate; nie dopisywacnaglowka/podstawiacinnegoGPU wceluominieciaABI.

### 2026-10-25: ABI bibliotekGPU sprawdzone readonly
Na vendor p41(ro,noload) odczytano tylko ELFheadery iDT_NEEDED znanych bibliotekGPU; nie kopiowano ani nie uruchamiano blobow, mountposprzatanyprzeztrap. Sa wersjeELF32ARM(e_machine40) iELF64AArch64(183): libEGL_mtk.so,libGLESv1_CM_mtk.so,libGLESv2_mtk.so orazlibIMGegl.so. Nie zakladac, ze telefon ma tylko32bitGPUuserland. ELF64GLES2 zalezy odlibIMGegl,libsrv_um,libusc,libglslcompiler,libm,libgpu_aux,libgralloc_extra,libsync,libc.so; IMGegl takzeodlibged,libhardware,libz,libdrm. To zaleznosci vendor/Android, nie gotowy natywny stosAlpine; mostABI/HAL wymagalby osobnej swiadomej decyzji i pracy, nie uruchamiano chrootAndroida/libhybris. CONFIG_COMPATnieset, ale ELF64biblioteki istnieja; brakCOMPATnie jest sam w sobie blockerem64bitwariantu. Wyniki phone/hardware/gpu-userspace-abi.json. V59nadal przygotowany, zgoda na zapis oczekuje, telefonv58.

### 2026-10-25: v59 potwierdzony, rzeczywisty HW BVNC22.49.21.16
Wlasciciel jawnie zaakceptowal zapisv59 iGPUtest. Backupboot24MiB,zapiswylaczniebootp36,pelnyreadbackSHA256PASS,reboot; wlascicielPowernaLK/startpotwierdzil. uname#44. Przedtestem VCOREselector101/programmed101/enabled1/1150000uV; GPUidentity result-11(EAGAIN),faulted0,clock569999694Hz - prober nie wlaczylGPUprzyboocie. Readonlyregulatorzarejestrowany; log"Voltage range but no REGULATOR_CHANGE_VOLTAGE" jest skutkiem celowegobrakuprawazmianynapiecia, nie bledem pozwalajacymdodacset_voltage.
Jawnyrun_gpu_id_trial.sh przeszedl: VCORE1150000uV,26MHz,powerupobuMFGdomeniSRAMack, bezfirmware/HWAPM/GPUwrite. Rzeczywisty CORE_ID08470000,PBVNC0016003100150010 -> BVNC22.49.21.16. result0/faulted0. To juzHWodczyt, nie stockstring/apphint! Kernelczas56.107590s start,56.115614s rawID, testzakonczonypoprawnie. Nie oznaczaGLES/Vulkananiakceleracji.
Po probie niezaleznyalwaysonread:TOP_CFG180000000(parent0/26MHz,gate_disabled1),SPMstatuses30005f4c,CTRL338/33c72 (obiedomenyOFF),INFRA_SI0_CTL05000072,PROTECT_ENABLE00600098,STA1fe7f99b8. Poprawnyvendorpoweroff ustawia ochronyMFG iwaydisable; NIE odtwarzac staregoLKstanuOFFbezprotection. MFGPLLrejestry181/0215ec4e/0015ec4e/PWR80000001 identyczneprzed/po. VCOREnadal1150000uV/selector101; napiecia iPLLniezmienione. LadowanieCharging/temp287,displayEOF4223/SOF4223/underflow0/abnormal0. GPUlockposprzatany, statusRESULT0. UTCz1970ustawionozPC,offset-3s. Testraz wystarczyl, bez zbędnegopowtarzania.
Aktualny obrazmfw_v59.img, awaryjnyv58. Otwartaakceleracja wciazwymaga zgodnego firmwareformatu/ABI i supportu22.49.21.16 wMesa+kernel; GPUobecniewylaczone. Doszly rzeczywistesprawdzoneCCF/powercycle/readonlyregulator/ID, ale brakGPUjobow,rendernode,DRMGEM/KMSpageflip. Dowodmaszynowy phone/hardware/gpu-hwid-v59.json.


### 2026-10-25: initial Rogue DDK1.9 port, compile-only
Public Wiko GPL source pinned to 096323c2d5db8a05e84df31608fc0c9d22e47b31,
m1.9ED4893595 subtree: all 590 Git blobs verified exactly. MT6739 config selects
HW BVNC22.49.21.16 and stock rgx.fw.signed. Complete userspace/firmware ABI not
yet proven; Android ION/sync and Bionic/HAL dependencies remain.
Four-file Linux7.3 overlay: explicit DRM headers/lifecycle, render-only flags,
removed legacy fields/DRM_UNLOCKED, kernel stddef and external Kbuild fixes.
Fresh ARM64 pvr_drm.o W=1/-Werror PASS without warnings; exact-source replay,
patch application and fresh compilation PASS. Not linked or loaded.
Memory object BLOCKED by dma-buf attach/kmap/vmap API, sync by Android sync.h,
osfunc by removed pfn_t.h. No compatibility casts/fake shims added. Next memory
and fence port, validated power integration, firmware ABI audit, then linking.
Phone readonly check: Linux7.3.0 uptime3h23, still v59, no reboot/write/GPU test.
Details and reproduction docs/GPU_DDK19_PORT.md; source/patch metadata
phone/kernel/gpu/ddk19; full reference stays on D:, no vendor blobs in repo.


### 2026-10-25: DDK dma-buf and native dma_fence port
Fresh exact-source replay + cumulative ten-file patch + seven ARM64 objects
W=1/-Werror PASS without warnings. Units: pvr_drm, physmem_dmabuf,
pvr_sync_dma_fence, pvr_counting_timeline, pvr_sw_fence, pvr_fence,
dma_fence_sync_native_server. Native fence implementation already exists in
DDK; Makefile now selects it instead of Android staging sync, with
SUPPORT_DMA_FENCE. access_ok/strscpy/u64 seqno logs updated; no fake sync shim.
dma-buf imports use modern reservation-locking unlocked map helpers and retain
iosys_map through PMR release. Bounds checked; raw-pointer PMR rejects IOMEM.
Fill supports iosys_map RAM/IOMEM dispatch, correct paired DMA_TO_DEVICE CPU
access, partial pages, cleanup and propagated errors. DMA SG uses mapped
sg_dma_len; rejects zero/misaligned segments. PMRCreate error mapping leak fixed.
Actual helper fault injection ASan+UBSan PASS (mock exporter): page poison,
tail/guards/zero, alloc/begin/vmap/null/end errors and first-error preservation.
These tests do not prove actual IOMEM/DMA cache coherency, PMR runtime or GPU.
Whole external default build refused CONFIG_MODULES=n; direct composite .o has
no rule in external built-in setup. No config changes to work around it.
No linked driver, firmware, jobs, GPU power request, phone write or restart.
Next OS APIs/platform power/ION replacement and proper full linking/ABI audit.
Sources/overlay/replay/test helpers exported; docs/GPU_DDK19_PORT.md has details.


### 2026-10-25: entire userspace native; complete DDK archive/link and native C client
Owner explicitly rejected isolated Bionic/HAL: entire stack must remain native.
Mesa has an open direct-ioctl pvrsrvkm backend, pinned/reference files verified
from upstream Google mirror commit2736dd1220d58192f5591ca6f9fcc5e60ee368ad.
It requires1.17@6256262; real1.9 bridge adaptation and22.49.21.16 device/compiler
support remain pending. CONNECT same16/17-byte sizes but different fields/offsets:
1.9 output error@13 and noBVNC;1.17 error@8/BVNC@0. Do not fakeversion/BVNC.
Native C probe uses generated1.9 headers/staticasserts; host+staticAArch64buildPASS.
Uploaded only /tmp/tmpfs and ran on Alpine: --abi RC0, --connect /dev/null RC1,
exact1.9@4893595/in16/out17/error13/options80000010. NoGPU node/firmware touched.
Kernel-side cumulative89-file overlay: full built-in archive and ARM64 relocatable
link PASS; text586839/data37856/bss59356. All remaining undefined symbols supplied
by workingv59 vmlinux after removing optionalMTK stats hooks. Exact-source replay
andfullcompile/link/symbolcheckPASS. Built-inKBUILD_BUILTIN=1 archive target solves
the earlier externalCONFIG_MODULES blocker without configchanges. WSLext4copy
compiles faster; D: source authority. String147cases+dmabuffaultinjection ASan/UBSanPASS.
Port coversmodernPFN/mmap/vmap/shrinker/timers/clock/cacheAPI/debugfs, nativefences,
private drainable bounded-retry fence teardownqueue. Narrowwarningexceptions only
releaseunusedvars/consts andsuggest-format-attribute; API/type/format remainerrors.
Newnativeplatform/powerintegration compiled, NOT runtimeverified: defaultbinding
disabled, uniquecompatible, nohotunbind/built-inonly; fixed26MHz/readonlyVCORE and
boundedv59sequence; faultlatches/clockretained. FutureDTmustdisableoldIDprobe.
StockDTGPUresource13000000/80000 andSPI226/LEVEL_LOW confirmedfromsavedstockevidence,
GPUhardwareIRQnotyettested. Nextkernelintegration/newimagepreparation, thenexplicit
approvedbootwrite/Power andcontrolledfirmwaretest. Phone stillv59; noImage/config
change, restart orGPU power request in this phase. Details docs/GPU_DDK19_PORT.md.


### 2026-10-26: mfw_v60 native GPU Image prepared, no boot write
Integrated595 DDk source files into Linux7.3 with dedicated boolean Kconfig.
Fullkernel compile found3old-style definitions missed in isolated build; changed
PMRInit/PMRDeInit/PVRSRVGetPVRSRVData to(void). Image#45 built withnohup/ARM64cross.
Image13719560B, boot9089024B, SHA256ab33cc6a26370fd8512d5bf700886fb15bf5f5af42d4fbf4c09d7e4f97334092.
Config delta onlyDRM_MFW_ROGUE_DDK19=y; initramfs/cmdline/addressesidenticalv59.
DT19 addsnativeGPUreg13000000/80000,SPI226/low,validatedCCF/VCORE/PMICreferences;
oldIDprobedisabled topreventdualownership. NootherDTchanges. Defaultnativebinding
disabled. Fullselfcontained627filekernelpatch9830162B dryrunpristine7.3PASS.
Backupboot24MiB+RAMstagingv60PASS; no write/reboot yet. Phone stillv59/#44.
Importantcodeaudit: CommonDeviceInit onlysetsbridges/sync; firmwareinitialization
happensonFIRSTrendernodeOPEN (CommonDeviceOpen). Thusnative_probe --connect is
anexplicitfirmware/power test evenbeforeversionioctl, notreadonlyquery. Prepared
script onlystagesFW(readonlyvendor->tmpfs) andbinds; inspectlogs thennativeCopen.
NoBionic/HAL,shaderjobs yet. ActualFWboot/IRQ/renderingremainunverified.
Preservedpre-DDKv59kernel-symbolbaselineonD/out, SHA2563d5faac8c9cd4590ad108212b9d838dea796f88f06c4522d4d09174d46da3bcb,
andcheckscriptrefusesbaselinecontainingDDK topreventself-resolvingsymbolmistakes.
Firmwaretrialscriptgatesbuild#45/noGUI/nooldID/VCORE,onlyvendorp41ro,noload,
fixedfirmwarefilename/hash/size, tmpfsstage, singleattemptmarker. Built-inonly,
suppressedunbind; faultclock/contextremainpinneduntilreboot. Needexplicitowner
bootwrite/restartconsent under originalrule9; runtimeFWtestrequiresownerPoweronLK.


### 2026-10-26: v60 boot/bind confirmed; v61 fixes measured DRM-open blocker
Owner explicitly approvedv60 write/reboot/nativeFWtest. Bootp36write+fullimageSHA
readbackPASS; ownerPower/startconfirmed. uname#45,defaultenableN,VCOREselector101
1150000uV/enabled1,Charging. Readonlyvendorp41 firmware copied onlytotmpfs with
size1265664/SHAverified; vendorunmounted. ExplicitnativebindPASS: card1/renderD128,
LinuxGPUirq17(fromstockSPI226low), compiletimeBVNC22.49.21.16. This is notnewHWID.
FirstnativeC--connect: openEINVAL. KernelWARN drm_file.c:329 beforedriveropen:
new7.3core requires filp->f_op->fop_flags & FOP_UNSIGNED_OFFSET. No firmware startup
in this attempt; do not describebindRESULT0 as firmware/accelerationPASS.
VCORE/Chargingunchanged. BroadinitialGPUlogfilter includedbootcmdline; narrowed
toexcludeKernelcommandline, sanitizedownedRAMlog. Neverexportdeviceidentifiers.
V61solefunctionalchange addsmandatoryFOP_UNSIGNED_OFFSET. Image#46,13719560B,
boot9089024B,SHA2561970fa060e0be018dc43f72c141940f31088be70520466de0d1722bd3e2d3f13.
DT19/config/initramfs/cmdline/addressesidenticalv60; validatorPASS; deltaapply/repro
exactcompiledsourcePASS. v61patchisdeltaONv60, notanotherfullpatch. Backup24MiB
andRAMstagingPASS; no v61write/reboot yet. Phonev60. Needownerconsent(rule9).
NativeCclient extendedwith genuine1.9heapqueries (IDs28/30, not1.17IDs30/32),
generatedstruct/staticasserts. Host+AArch64buildPASS; ASan/UBSanmockbridge8casesPASS:
wirefields,versionrefusal,countbounds,unterminatednames,IOerrors,disconnectcleanup.
Realheapquerynotrun becausefirstopenblocked. NewclientcurrentlyinphoneRAM.
PrimaryMediaTekMT6739specnamesGE8100(up570MHz), notGE8300. Runtimeidentityremains
actualBVNC22.49.21.16. DDKconfig22features/5errata/3enhancements extractedasreference,
only14Mesafieldsdirectlymapped. NotcompleteMesaGPUprofile: unknownUSC/tile/compiler
valuesmustnotdefaultfalseorcopyGE8300. Sourceandconfidencegpu-ge8100-feature-reference.json.


### 2026-10-26: offline native Mesa ABI audit while v61 approval pending
Mapped42 static Mesa1.17 bridge constants against DDK1.9:9namesmissing,7numericIDs
changed. EqualIDs donotmean equalwirelayouts; runtimecapabilitymasksstillrequired.
Results phone/hardware/mesa-ddk19-bridge-command-map.json; nativeheapclientuses
generated1.9layouts and hostmock tests, notunchecked1.17structs.
OnlystaticELFsymbolmetadata read from genericvendorlib64libusc/libsrv_um/
libglslcompiler (p41ro,noload,unmountedafter). FoundPVRUniFlex/PVRSRVsymbolnames;
readelfwarnedaboutdynsymsh_info; symbolreferencesnotassumedallexports. No binaries
copied or executed, noBionic/HALruntime, no newhardwareprofilederived. This is
referenceanalysisonly, consistentwithowner'snative-onlyrequirement.
Phone stillv60; v61prepared/staged, approvalrequestpending. No additionalGPUopen,
powerrequest,partitionwriteor restart during theseoffline/readonlyaudits.


### 2026-10-26: v61 boot written after explicit approval
Owner approvedv61 write/reboot/firmwaretest. Backup24MiB verified, writeonlybootp36, completeimage readbackSHA256PASS (1970fa060e0be018dc43f72c141940f31088be70520466de0d1722bd3e2d3f13). Rebootrequested; pingnotyetresponding, owneraskedshortPoweronLK. No v61startup/firmware result yet.


### 2026-10-26: v61 native firmware/CONNECT/heap queries confirmed
OwnerPower/startconfirmed. uname#46,initialenableN,VCORE1150000uV/selector101/enabled1,
Charging. Repeatedguardedbindwithreadonlyvendor->tmpfsfirmwarepreparationPASS;
nativecard1/renderD128. FirstnativeCOPEN/CONNECT/DISCONNECTnowPASS:error0,arch64,
caps00000100,pvrbridges0008a847,rgxbridges00001447. Firmwarebasenamefallbacknormal:
twoBVNCsuffixcandidatesENOENT, thenpinnedgenericrgx.fw.signedloaded. Debugfsstatus:
DriverOK,FirmwareOK,HWR0,CRR0,APM0,GPUutil0%. IRQ17/MT_SYSIRQ226count1.
Native--heapsPASS17GPUVAheaps: General65c0000000/80000000, PDSa800000000/40000000,
USCba00000000/40000000, VisTeste400000000/100000, plusotherdriver-definedheaps.
Driverdebugversionreports22.49.21.16(HW); bindlogusedcompiledfallback, so do not
callthisanindependentrawIDread. ActualrawIDwasverifiedv59. NativeCPUstackonly:
noBionic/HAL/GPUvendorCPUlibraryexecution. Firmwareboot+IRQ+serviceAPIconfirmed,
NOTshader/renderingacceleration/zero-copy/video. NextnativeRAM/VMAPItestsandMesa
bridge/profileport; no need to restart for userland changes.
OWNERNEWCONSTRAINT: nowawayfromphone; NO further phone restarts. If GPUacceleration
blockedbynewkernel/restartrequirements, continue modem/WiFi/BT/otherhardware safe
work andsourcepreparation. No protectedpartition/NVRAMwrites, no nvram_daemon.


## 2026-10-26 — v61 native PMR CPU mapping, no reboot
Measured on v61/#46: native C client allocates a zeroed locked 4096-byte PMR, maps via the opaque handle, verifies all 1024 words, unmaps and frees successfully. Repeated at uptime 6h59; Driver/Firmware OK, HWR=0, CRR=0, utilisation=0%. This proves CPU mapping, not GPU execution or acceleration. Exact generated DDK1.9 ABI uses 32-bit allocation flags, 52-byte allocation input and 12-byte output; do not copy 1.17 structures. Host mock suite: 13 scenarios including allocation/mmap/initial-zero/free failures, ASan+UBSan PASS. Owner away: no reboot/reset/suspend or boot writes; continue native userspace and offline radio work.


## 2026-10-26 — live v61 radio memory audit, no restart
Read-only live DT, early reserved-memory log and /proc/iomem agree: CONNSYS has a dynamically allocated 2 MiB no-map reservation at 0xbfe00000; CCCI reservations are 0x6c000000/1 MiB (no-map), 0x5c000000/256 MiB (mapped but reserved), and 0x8e000000/12 MiB (no-map). All four are excluded from the allocator. The CONNSYS DT node has size/alignment 2 MiB and no fixed reg: do not assume this dynamically selected address matches stock EMI/remapping expectations. Reserved memory alone does not establish safe DMA or firmware compatibility. Current netdevs: lo/sit0/usb0; no Bluetooth or remoteproc class. No radio activation, NVRAM access or reboot. Evidence: phone/hardware/radio-memory-v61.json; repeatable read-only script archive/pc_work/radio_reserved_audit.sh. Next: verify stock CONNSYS EMI base/remap and CCCI region usage before implementing power/transport.


## 2026-10-26 — kexec feasibility and live hardware status
Owner asked about replacing the kernel without a hardware restart. kexec can boot another kernel without returning through LK, but restarts Linux/userspace and requires working shutdown/handover for this board. Current v61 configuration has CONFIG_KEXEC_FILE disabled; live phone has no /sys/kernel/kexec* or kexec_load_disabled sysctl and no kexec command. CONFIG_MODULES is disabled too. Cannot enable these simply by installing a userspace package. A future boot with kexec support, DT/initramfs handling, CPU shutdown and GPU/display/USB/charger handover testing is required, with owner present. No kexec, reset or boot write attempted. Reference: https://github.com/torvalds/linux/blob/master/arch/arm64/kernel/machine_kexec.c
Added tools/mfw-hw-status and deployed /usr/local/bin/mfw-hw-status; explicit full path required by current telnet PATH. Read-only allowlisted snapshot, no IDs/scanning/device opens. Syntax and live execution PASS: Charging, 4250225 uV, 29.2 C, CPU 949999755 Hz, GPU/FW OK HWR/CRR0; no I2C/IIO/video/PCM/HCI/wiphy/remoteproc/GNSS devices. Battery percentage unavailable, not estimated. This is availability/status reporting, not functional validation.


## 2026-10-26 — v62 ready: kexec and native hardware module foundations
Owner requested kexec and hardware support, permits one future restart. Built #48: KEXEC_FILE, HOTPLUG_CPU, MODULES; modular IIO/I2C/MC3230, cfg80211/mac80211/BT/WWAN/GNSS, V4L2/camera references and ALSA/MT6357. Existing native GPU retained disabled by default. 41 modules, depmod and boot validator PASS; Image14184456, boot9332736. DT19/initramfs/cmdline/addresses byte-identical v61. Signed native Alpine kexec-tools installed, version/help PASS; no kernel load/jump. Private24MiB backup and RAM staging PASS, boot NOT written, phone stillv61. Framework configuration does NOT prove functioning radios/cameras/sensors/audio/acceleration. Missing MT6739 drivers and transport remain. Full details docs/MFW_V62_KEXEC_HARDWARE.md, evidence phone/hardware/mfw-v62-validation.json. Await concrete v62 boot-write approval under owner rule9; no speculative reboot.


## 2026-10-26 — v62 boot written after explicit owner approval
Owner said "wgrywaj". Updater verified private24MiB backup, wrote boot p36 only, full9332736-byte image readback SHA256 PASS: 197dc9dfa30ada4ffa94b1ceb34e66c4f6c2656c95ccb4b158439b6eb599df05. Reboot requested. Startup #48 and kexec API not confirmed yet; await owner shortPower on LK. Module bundle remains on PC; no new modules loaded and no kexec jump attempted.

V62 first startup check after owner "chyba wstal": ping0/2, shell TCP timeout; Windows has no matching RNDIS adapter or192.168.7.20 address. Does not identify whether LK, kernel or USB failed. Requested screen photo; no further reset/write attempted.


## 2026-10-26 — v62 startup and native module loading PASS
After owner reported booting, ping2/2 and RNDIS returned. Linux7.3.0 #48 confirmed at uptime77s. Charging/USBonline1, VBAT4250555uV,26.9C,CPU949999755Hz. NativeGPUdisabledN. kexec_loaded=0 and kexec_load_disabled=0; userspace present. No kexec load/jump or second restart performed. Verified module archive SHA256, installed41modules to rootfs, depmodPASS. Explicit modprobe of industrialio/bluetooth/gnss/wwan/snd_soc_core and dependencies PASS with Not tainted. No physical radio/sensor/codec driver forced; framework presence does not prove hardware works. No autoload setup added.


## 2026-10-26 — standing authorization for MICROFW updates
Owner explicitly authorized: "jak cos to juz mozesz wgrywac kernel i wszystko bez mojej zgody". This grants standing authorization for subsequent MICROFW kernel/boot updates, native driver/userland deployment and associated restart/kexec tests; do not request per-version boot-write consent again. It supersedes the earlier rule9 requirement for per-update confirmation within this established update scope. Existing partition boundaries remain: boot p36 and microfw rootfs p46 only. No authorization to format partitions, change GPT, write protected partitions/NVRAM, read secrets or publish/private-data exports. Preserve verified private boot backup, image validation and full readback before restart. Any future owner instruction prohibiting restarts takes precedence.
Kexec API and module loading are proven in v62; kexec handover and unattended update recovery are NOT yet proven. Do not claim automatic LK bypass until measured. Current owner wording permits update/restart testing; no new restart executed in this entry.


## 2026-10-26 — first actual kexec PASS
Owner explicitly requested test. v62/#48 native GPU N; kexec disabled0, loaded0. Uploaded exact uncompressed Image and initramfs extracted from validated v62 boot; checked both SHA256. kexec -s -l --initrd=... --reuse-cmdline returned0, loaded1. Command line kept only inside phone, not printed/exported. Then sync, remount rootfs ro (abort on failure), kexec -e. USB returned without any requested owner intervention. At first shell uptime36.56s, RAM trial files absent, loaded0, /chosen/linux,booted-from-kexec exists: definitive kexec boot marker. SMP4CPUs, rootfs rw, Charging USBonline1, CPU~950MHz, GPU N. No boot partition write in this test. Same kernel/DT only; active GPU, different kernel and visual panel behavior remain untested. No second jump attempted. Evidence phone/hardware/kexec-v62-trial.json. Archived scripts are fixed v62 trial scripts, not generic unattended updater; do not reuse for arbitrary images without changing validated hashes/version guards. Modules remain installed but not automatically loaded after reboot.


## 2026-10-26 — I2C1 and native light IIO without reboot
Built/loaded two native GPL modules on v62/#48: restricted PIO I2C1 adapter plus stock-c2 raw-light IIO driver. Sensor responds stably at0x53 with ID0xc2, not assumed0xb1. Stock ltr577 non-b1 ALS/PS callers use the same raw register map; exact part remains unknown. InitialCTRL0/rate0x22/gain1. Explicit2-second ALS-only trial CTRL2 returned ALS16/channel0a56, restoredCTRL0; PS/IR never enabled. IIO device mfw-stock-c2-light exposes in_illuminance_raw16 and in_intensity0_channel0a_raw54. Values are RAW, not lux; freshness and controlled response not established. Owner away, cover/lamp test deferred.
PIO transport reserves controller resource, refuses other owner/gated/busy controller, uses20ms bounded poll, preserves timings/config and masks/restores IRQ config. Only allowlisted reads; optional ALS control permits only reg0 values0/2. Any transaction fault latches and pins module until reboot. No PMIC/GPIO/clock/power manipulation. Initial I2C_RDWR attempt rejected before MMIO due i2c-dev-added DMA_SAFE; confirmed kernel source, accepted annotation without enabling DMA, rebuilt/reloaded. Default old snapshot rejectedIDc2 correctly; explicit c2 raw mode added based on stock non-b1 callers.
ARM64W=1/-WerrorPASS; ASan/UBSan exact allowlist1280read+65536write cases and malformed messages; exact IIO callback restoration/failure/mutex casesPASS. Own module O taint expected. Modules installed extra/,depmodPASS, explicit /usr/local/bin/mfw-light-start tested; no autostart. GPU firmware and PMR_CPU_MAP_PASS repeated successfully after kexec, HWR/CRR0. Source phone/kernel/sensors, evidence phone/hardware/light-iio-v62.json. No further reboot/kexec performed.


## 2026-10-26 — native GPU address space and media dependency audit
--gpu-memoryPASS: context/heap/VA reserve/map4096B at0x65c0000000, unmap/unreserve/destroy, PMRrelease/DISCONNECT; no render job. Driver/FirmwareOK,HWR/CRR/APM0,VCORE1.15V,Charging. Native generated1.9 ABI and22mockscenarios ASan/UBSanPASS, including8VM forward/teardown failure stages; stop parent releases on failedteardown. Evidence phone/hardware/gpu-va-v62.json.
User expanded scope: rendering, zero-copy, H264/HEVC acceleration, cameras/mics/audio and radios/GPS. docs/NATIVE_RENDER_MEDIA.md records measured live DT engine resources and blockers. DDK export rejects externalDMA-BUFattach (ENOSYS); candidate path is DRM-allocated/exported buffers imported by GPU, pending properKMS+sync. Exact mainlineMT6739codec match absent; stock vcodec_lockhw/waitisr ABI is a research lead, not decoded-video proof. No new reboot/kexec or media-engine activation. Owner away, optical physical response remains pending.


## 2026-10-26 — exact stock4.4 CONNSYS/WMT driver analysis
User asked to search4.4driver. Decompiled exact local GPL wmt_drv.ko read-only (not loaded into7.3), relevant26functions incl power/reset/PMIC/EMI/STP-BTIF. Callback returns0x6739. consys_read_reg_from_dts maps4DTresources in order:18070000/200,10007000/100,10000000/2000,10006000/1000. Clocknameconn; DTclockphandle1a/index3; IRQspecifiersSPI233flags8 and235flags2 (not LinuxIRQs).
EMI remap offset1380 inresource2 ->physical10001380. Objdump confirms ubfx #21,#13 then OR old and2000, store+DSB. Native bounded O_RDONLY/PROT_READ AO-register probe measured0, remappingdisabled. ReservedRAMbasebfe00000 from earlieraudit would encode25ff ifold0; calculationonly, NEVER written. Stock MPU caller protectsbase+80000 throughbase+size-1 region16/rawpermission4b6da28; permission semantics unresolved.
Stockpower paths requirevcn18/vcn28/vcn33_bt/vcn33_wifi; commonvcn18 routine also touchesvcn33_bt. pmic_set_register_value selectors are ENUMS, notPMICaddresses. Exactkernel tableVAffffff8008bd8390 stride12 decoded index/addr/mask/shift withbounds+IDchecks: c2f->1a58bit0,c31->1a5abit0,d03->1b1ebit1,d08->1b24bit1,d3b->1b8abit1,d41->1b90bit1. Matched localmainlineMT6357 names VCN18CON0/OP_EN,VCN33OP_EN/OP_CFG,VCN28OP_EN/OP_CFG. No PMICwrites or radioactivation. Ghidrasignatures/indirectcallbacks need instructionverification beforeport. Source/evidence phone/ghidra/wmt_consys.c,k44_pmic_fields.c,phone/hardware/consys-stock-v62.json.
Publicpinned WikoGPLtree contains onlyconnectivityadapters (4files), notWMTimplementation; sameforPower535tree. NewerRedmireferencehas6765/6768platforms, notexact6739; do not copyregistersequences. Independently fetched7MT6739vcodecGPLreferencefiles, allGitblobverified, manifestinphone/hardware/vcodec-mt6739-reference.json. Theyconfirm oldLOCKHW/WAITISR/mmap/controlABI research direction, notLinuxdecode. No protectedpartition/NVRAMsecrets accessed.


## 2026-10-26 — full Mesa build and initial DDK1.9 adapter
Pinned completeMesa2736dd1220d58192f5591ca6f9fcc5e60ee368ad fetched toWSL/home/y5/mesa-native. FixedLLVM23 CLC APIs and NIR kernel-export handling; beforefix generatedhelperprogram enumempty despite14SPIR-Ventrypoints. Six exactpass regressions ASan/UBSanPASS; fullx86_64PowerVR Vulkanlibrarybaseline+experimentalDDK19 buildsPASS. ReferenceGX6250 helpergeneration only, noGE8100profile substitution.
ActualMesa Mesonopt imagination-mfw-ddk19 adds exactCONNECT/DISCONNECT1.9translation+driver/buildguards, skipsobsoleteINIT, rejectsalluntranslatedbridgecallsENOTSUP. FixedBVNC is explicitDURA-L21buildtarget, notnewquery. Adaptertests ASan/UBSanPASS; two patches replaypristinesourceexact7files. No phoneMesa installed, no rendering/shader claim. Remainingmemory/sync/jobABIandcompleteGE8100profile. phone/mesa/README.md and phone/hardware/mesa-stage1-validation.json.
GenericstockUSCcompiler copied onlytoD/private_gpu_reference forstaticanalysis, size/SHAverified, neverexecuted/committed. .dynsym28exports/nofullsymtab, targetBVNCpatternsabsent. StaticfunctionGetTileBufferSize returns0x3000(12288), verifiedARM64instructions; notequivalenttoprovenhardwareUSCcommonstore. Vendorunmountedverified. NoBionic/HAL. WSLbuildpackagesinstalledexactsignedversions viaArcharchiveaftermirror404; nofullupgrade/Windowschanges. Phone staysv62, GPUFWhealthy/Charging; no further restart. User wants persistence andlowtokenusage; ifGPUdata/ABIblocks, proceedWiFi/BT/modemwithoutasking.


## 2026-10-26 — optical physical tests PASS; Mesa ABI stage2 saved
Owner present. Initial45s shortIIO150ms samples stayedALS19, unclearresponse. ContinuousALS45s nativecapture180samples reacted tocover0 andlamp29897, CTRL0restored. Thus hardwarelightresponsePASS; shortreadfreshness was notproven. IIOwaitincreased500ms andinstalled, separatecontrolledIIOreteststillpending.
IR: newoptin kernelcallback savesreg0..3, usesexactstock36/10/5c PS config, enablesPS50samples100ms, restores/readbackchecksall4regs. Far14..23median18; ownerhand1-3cm182..268median235.5; bothresult0/restored1. Kernelcallbackrunsrestorationevenifwriterprocessends; faultlatchallowsoneboundedinternalPSoffattempt butnofurtherblindconfigretry. ARM64W1/Werror+ASan/UBSan67faultpositions/busy/optin/offtestsPASS, read/writeallowlisttestsPASS. NoIRQthreshold/NVRAMwrites. ReloadedmodulewithIRgateN, restoredIIOlight. mfw-hw-statusI2Ccountfixedtobusdevicespath. Evidencephone/hardware/optical-physical-tests-v62.json; no reboot.
Mesa stage2: contextcreate/destroy, heapcount30->28/ordertranslation, heapcreate/destroy. Sevenopstotal. GeneratederrorcodesRETRY25/BRIDGE37confirmed; fullhostlibrary+hostASan/UBSan+nativephoneactualDRMsmokePASS17heaps/cacheline64,FWHWR/CRR0. StillnoRendering. Patchphone/mesa/mesa-ddk19-stage2-on-stage1.patch exactreplayPASS.


## 2026-10-26 — user reprioritized radios and all sensors
Owner explicitly postponed GPU. Preserve Mesa stage2; next priorities modem/WiFi/BT/sharedCONNSYS and physical sensors. Do not continue spending work on Mesa/rendering unless asked again. Existing GPU firmware stays healthy; no attempt to hot-unbind it. Optical physical tests already PASS; next verify MC3xxx ID and exact stock4.4 register map rather than force mc3230. Radio activation still requires native serialized PMIC/clock/EMI/power/transport work.


## 2026-10-26 — priorytet radia/czujniki: MC3xxx odpowiada
GPU/Mesa odlozone na prosbe wlasciciela. Bez restartu: ograniczony adapter I2C1 dopuszcza przy 0x4c tylko odczyt pojedynczego bajtu rejestrow 0x3b i 0x18. Natywny program C odczytal product=0x66, hardware=0x23. Dokladna walidacja stock4.4 (phone/ghidra/k44_accel.c) akceptuje te pare: hardware 0x22..0x2f, product & 0xf1 = 0x60, product & 0x31 = 0x20. To identyfikacja, NIE dowod dzialajacego pomiaru/przeliczenia osi ani potwierdzenie nazwy handlowej. Brak zapisow konfiguracji i kalibracji akcelerometru. Test rzeczywistej allowlisty ASan/UBSan PASS. Nowy modul zapisany w extra/, depmod wykonany. Przywrocono klienta IIO swiatla; odczyt raw=12, ir_test=N.
Nastepnie: ustalic rozdzielczosc/range/orientacje MC3xxx na podstawie dokladnego kodu4.4; dla radii przygotowac odczyt VCN przez istniejacy mutex PWRAP (bez rownoleglego dostepu do WACS2), potem zasilanie/clock/reset/EMI/BTIF. Zadnego uruchomienia radia ani zapisu NVRAM w tym etapie.


## 2026-10-26 — powrot do GPU, Mesa stage3a PASS
Wlasciciel ponownie wybral GPU. Dodano do adaptera1.9 rezerwacje i zwalnianie GPU VA, z przestawieniem pol starego ABI oraz kontrola wyrownania/zera/overflow/rozmiarow. ASan/UBSan i pelny hostowy build Mesy PASS; patch odtwarza sie dokladnie na stage2. Na telefonie natywny standalone C uzywa dokladnego adaptera Mesy i rzeczywistych ioctl: kontekst, sterta, rezerwacja4096B i pelne zwolnienie PASS. 17stert/cacheline64, DriverOK/FirmwareOK/HWR0/CRR0. Bez restartu; bez renderowania. To nie jest pelna biblioteka Mesy uruchomiona na telefonie. Dziewiec przetlumaczonych operacji; PMR/cacheflags/map/sync/jobs i profil kompilatora pozostaja. Dowody phone/hardware/mesa-stage3-validation.json; patch phone/mesa/mesa-ddk19-stage3-on-stage2.patch.


## 2026-10-26 — Mesa stage3b PMR/map PASS
Natywny adapter Mesy tlumaczy teraz alokacje/zwolnienie PMR i mapowanie/odmapowanie GPU. Istotna roznica ABI: GPU uncached1->0, CPU write-combine0->1; brak obcinania64-bitowych flag. Zakres celowo ograniczony do jednej niesparse strony4K, pid/pdump0 i zweryfikowanych flag; reszta odrzucana. Pole out_flags nie istnieje w1.9, zwracane0 jako niedostepne, nie jako odczyt efektywnych flag (pinned wrapper go nie uzywa).
ASan/UBSan:64kombinacje cache, nieznane bity, uklad wejscia/starego wyjscia, bledy i kontrola parametrow PASS. Pelny hostowy build Mesy oraz odtworzenie patcha PASS. Na telefonie standalone C z dokladnym adapterem/real ioctl:4096B zerowanie, wzorzec CPU write/read, GPU VA map/unmap i pelne zwolnienie PASS; trialRC0, Driver/FirmwareOK,HWR0,CRR0. Bez restartu i bez zadania GPU. CPUreadback nie dowodzi koherencji odczytu przezGPU. Trzynascie operacji; szczegoly stert/sync/jobs i profil kompilatora nadal wymagaja pracy. Dowody phone/hardware/mesa-stage3b-validation.json.


## 2026-10-26 — Mesa stage4a blok synchronizacji PASS
Dodano ALLOC/SET/FREE sync: legacy placeholder4B, przestawienie wyjscia28B; SET16B/FREE8B. Wazne: hhSyncPMR ma typ PMR_LOCAL_EXPORT_HANDLE i jest dzieckiem bloku, nie zwyklym PMR. Nie mapujemy ani nie zwalniamy go zwyklymi operacjamiPMR.
ASan/UBSan, pelny hostbuild i replaypatch PASS. Telefon: utworzeniebloku4096B, SET0x12345678, SET0, FREE i regresjaPMR/map PASS, trialRC0, FirmwareOK/HWR0/CRR0. API sukces nie jest niezaleznym odczytem zawartosci ani fence/GPUcompletion; brakjob/render/restart.
Inspekcja pinnedMesa potwierdzila: buffer_create przekazujegetpid(), obecny ostroznyadapterPMR dopuszczapid0; szczegolystert reserved_size nadal nieodwzorowane. Nastepne: localimport sync i semantyka pid/heap; potem zadania. Dowod phone/hardware/mesa-stage4-validation.json.


## 2026-10-26 — sync local-import i niezalezny odczyt PASS
Sprawdzono typy uchwytow w wygenerowanym server_mm_bridge: SYNCexport ->MM3 processDEVMEM_MEM_IMPORT ->MM6 connectionPHYSMEM_PMR. DiagnosticC korzysta jawnie z legacykomend; nie dodano bezrefleksyjnego przekazywania nowoczesnych numerow. Telefon: mmapPROT_READ, odczyt dokladnie0x12345678 ustawionego przezSYNC2, munmap/MM7unref/MM4unmake/SYNC1free PASS; regresjaPMR/mapPASS, Driver/FirmwareOK/HWR0/CRR0. ImportowanyPMR zwalniaMM7, nieMM8! Bezrestartu/jobGPU.
ASan/UBSan na dokladnym helperze produkcyjnym: sukces i7wstrzyknietych bledow z kontrola kolejnosci sprzatania PASS. To odczytCPU pamieci ustawionej przezkernel, NIE potwierdzenie zakonczenia zadaniaGPU/fence. Nadal otwarte pid/getpid i reserved_size stert. Dowod phone/hardware/mesa-sync-readback-validation.json; standalone warchive/pc_work/mesa19_phone_smoke.c.


## 2026-10-26 — PMR getpid zgodne z callerem Mesy
Callerbuffer_create pinnedMesa przekazuje getpid; usunieto blokade tego przypadku: adapter akceptujepid0 lub biezacygetpid, odrzucaobcePID. Legacywire pozostaje bez polaPID. HostASan/UBSan current/foreignPID i regresjaPASS, pelnyhostbuild/replayPASS. Telefon: PMR zgetpid, zero/patternCPU/GPUmap/cleanup i syncimportreadback12345678 PASS; Driver/FirmwareOK/HWR0/CRR0, bezrestartu/render.
Inspekcja pvr_srv.c: carveout_size zheapdetails jest naprawdeprzekazywane do heapinit; staryDDK nie zwracareserved_size. Pozostawiono jawnybrak zamiast zgadywac0. Nastepna analiza: zasady static-data/carveout i ABIcontext/submission. Dowod phone/hardware/mesa-current-pid-validation.json.


## 2026-10-26 — wzorzec UI: zdjecia wlasciciela
Wlasciciel przeslal9zdjec: ciemnegrupowanekarty, szklanenaglowki/search/przyciski, interaktywneslide/paralaksa i refrakcja kart przygescie. Spec docs/UI_STYLE_REFERENCE.md. Zachowacdesktop, English, mala skale iC/C++. Pierwszykod render.c: neutralneglasshighlight, czarneSettings/grupykart, odczytytelemetrii bezfikcyjnychradiotoggles. Buildhost-WerrorPASS i podgladdemo720x1440 wizualnie sprawdzony. Bezwdrozenia/restartutelefonu. Animacje/blur/refrakcja jeszcze niezrobione; fotosnie dowodzakrzywychczasowych.


## 2026-10-26 — pierwszy silnik animacji C

Wlasciciel chce piekne animacje wzorowane na iPhone. Dodano motion.c/h: znormalizowana krytycznie tlumiona krzywa420ms (bez arbitralnego skoku ostatniej klatki), slide/paralaksa20%, dim24%, promien karty plynnie0->28->0. Monotoniczny zegar, stany koncowe renderowane do cache raz przy zmianie strony, nastepne klatki Cairo kompozytuja obrazy. Retarget pobiera biezacy obraz, nie skacze do poprzedniego endpointu. Cache zwalniane po zakonczeniu/resize/shutdown; maks4Mpix/obraz.
Integracja: klient xdg-shell i tlo/overview/control desktop layer; kolejne klatki tylko przy aktywnej animacji i po framecallback, brak idle60fps. Nie animuje jeszcze obcych okien ani nie sledzi palca przez cala droge gestu. Launch Settings jako osobnyclient nie ma jeszcze animacji compositorowej. Blur/refrakcja tez nadal otwarte.
Test exact motion ASan/UBSan: firstframe, retargetpixelcontinuity, endpoint, resize, powtorzonecleanup PASS. Build-Werror, gesty/C++ABI i realWeston/labwc protocolregression PASS; powtorzony motiontest/build po dodaniu zaokraglen PASS. Nie wdrozono na telefon, FPS/plynnosc/hardware niezmierzone.


## 2026-10-26 — UI motion uruchomione na Y5
Nowe render/motion/wayland/desktop skompilowane natywnie Alpine ARM64-Werror; staryprogram zachowany /opt/microfw/ui/build/microfw-shell.pre-motion. Wlaczono sprawdzony run_ui_phone_event.sh:600s, hz0, pixman/card0, bezrestartu. Log desktopconfigure720x1440 oraz framecallbacks wszystkich3layers PASS. Otworzono osobny Settings (--windowed --page settings), noweStyle.
Dotyku NIE uruchomiono wtej sesji: stare powertrial odpinaPMIC, a GPUterazon korzysta zjegoVCOREregulatora; nie powtarzamy tej sekwencji bez rozwiazania zaleznosci. Ten etap to uruchomienie/ocenawizualna, nie physicalgesture/FPS PASS.


## 2026-10-26 — GPU transfer-context CREATE/DESTROY PASS
Wlasciciel wybralGPU teraz. Dodano ograniczoneABI128/0create i128/1destroy; wylaczony allzero16B framework (zweryfikowany rozmiar ze stock1.9), bezkodu/rejestrow do wykonaniaGPU. Host ASan/UBSan/build/replay PASS. Telefon: CREATE i boundedRETRY DESTROY PASS, trialRC0, PMR/map/syncreadbackregressionPASS, Driver/FirmwareOK/HWR0/CRR0, bezrestartu i bezjob. Liczbaoperacji18. ModernextraCLI/USChandles nieistniejawoldABI, jawnieNULL; nadal niepelnywinsys.
Istotne: modernSUBMITTRANSFER2=3, legacy3=SETPRIORITY, legacySUBMIT=2. Innyuklad i jedentimeline zamiast2. Nastepne: jawna translacja submission + pakietFW zweryfikowanydlategoGPU; nie wysylac nowoczesnegoblobu wprost. phone/hardware/mesa-transfer-context-validation.json.


## 2026-10-26 — wykonanie komendy firmware NULL PASS
Po prosbie o ciagla prace nocna/odczekaniu2min zbadano profil i wykonanie kolejki. NativeC: RGXKICKSYNC bridge138 (bit10 wRGXmask1447), CREATE0 -> KICK2 zjednymUFOupdate -> odczyt -> DESTROY1. Dokladne typy generated1.9 wyciagniete bez zmiany ukladu:108B input/8B output. Source rgxkicksync.c buduje RGXFWIF_CCB_CMD_TYPE_NULL bezjobpayload i kieruje KCCB doDM3D. KonfiguracjaNO_HARDWARE niejestwlaczona.
Telefon: CPUodczyt12345678 przedkick, pozniej87654321 zfirmwareupdate; trialRC0, PMR/syncimport/transfercontextcleanupPASS. Driver/FirmwareOK/HWR0/CRR0. Pierwszy dowod wykonania komendy kolejki przezFW, NIE shader/3Drender. Bezrestartu. HostASan/UBSan sukces+create/submit/timeout/destroyfailure PASS; przytimeout/teardownfailure nie zwalniamy zasobowzaleznych.
Profil:88pol wstructfeatures,14referencyjniepotwierdzonych,74UNKNOWN (nie wszystkiemusza byc wymaganeprzezkazdyshader). OfflinePCO prostystore: nalezyuzyc global_2x32; po poprawieniu formatualokator zatrzymuje sie na nieznanym unified_store_depth. Nie zgadywano wartosci. Audyt obejmuje rowniez include/hwdef; samegrepypco pomijaja zaleznosci inline.
Stocklibsrv_um generic skopiowano tylko doprivateGPUfolder PC,1493008B/SHA da56783b4293f1ad7345f0ac6e53229a3e3a6c7447f8963da9739ef752316803, staticGhidraanalysis bezwykonywania. vendorro,noload/unmounted potwierdzone. Targetstring22.49.21.16 znaleziony; brak odzyskanejglebokosciUSC. Biblioteki/dekompilacja prywatne, nigdyrepo.


## 2026-10-26 — offline PCO zwykle shadery + nocne wznowienia
Zwykly compute/vertex/fragment (info.internal=false) wymaga rzeczywistych allocatorow metadanych VS/FS/CS z pvr_arch_pipeline.c. Wyciagnieto dokladne funkcje z pinnedMesa; z_replicate musi byc~0u jakwdriverze (zero wczesniej prowadzilo do blednego outputdepth). NIRstore wymaga global_2x32, nie64bitconst.
Syntetyczna siatka testow (nie profil urzadzenia):30wariantow/stage,25powodzen/stage i jeden identycznybinarnywynik na stage:compute128B, vertex176B, fragment440B. Lacznies75/90; pozostale warianty odrzucaja ograniczenia alokatora. Brak wdrozenia/wykonania shaderow; nieznanefeatureflags/errata nadal otwarte. Nie przedstawiacto jako walidacjiGE8100. Bez syntetycznych unified_store_depth/usc_slots nadal jawnybladprofilu. JSON pco-normal-shader-sensitivity.json.
StockgenericGLES1.46MB skopiowany tylko privatePC/staticGhidra, mountro,noload/unmounted. Callsite ustawiaUNIFLEXparamword0x16=0xf8; USC kopiuje do state+0x30 i sprawdzamax0x100. To budzet temporaryregisters kompilowanegoprogramu, nie dowod unified_store_depth/usc_slots. Binary/decomp nigdyrepo.
Na prosbe o nocna ciagla prace ustawiono heartbeat wtymczacie co20min/24wznowienia (8h), automationId microfw-nocna-praca-gpu. Powiadomienia tylkoistotne. StanACTIVE potwierdzony przeztool; nie dowodzi przyszlego uruchomienia. Nie tworzycduplikatu.


## 2026-10-26 — nocny audyt realnego pakietu compute
Heartbeat zbadal generatedABI i dokladny stockUM przezstatycznaanalize (bezwykonywania vendorbibliotek). HostC zoryginalnymitypamiPASS: oldCREATE32B,oldKICK132B vsMesaCREATE56B/KICK108B; staryoffsetui32CmdSize104,psDMCmd108. OldKICKfunction2 vsmodern5.
Publiczny pinnedtree590plikow nie ma rgx_fwif_client.h/definicji RGXFWIF_CMD_COMPUTE (tylko niewykorzystywane aligncheckrefs); bridge przekazuje opaquepsDMCmd irozmiar, nie definiuje jego ukladu. Wczesniejsze grep fwif.h/shared byly blednym tropem; nie zakladac, ze nowoczesne MesaFWAPI pasuje.
Prywatny stocklibsrv RGXKickCDM(wrapper0x20c84ELF) wysylaFWpayload120B. Niezalezna weryfikacjaARM64:ELF0x20fdc ORRw9,wzr,#0x78 iSTR[sp,#80]. GhidraVA ma imagebase+0x100000. Stockcaller wypelnia tez7qword zcontextdata na kazdykick; nowoczesneAPI przekazuje static_compute_context_state przyCREATE. To wymaga zachowania/odtworzenia stanu wtranslacji, nie pomijania pol. Semantyki wszystkichrejestrow jeszcze niepotwierdzono.
Bezoperacjinatelefonie/restartu/job. Dowod phone/hardware/gpu-compute-submission-audit.json; testlayout archive/pc_work/compute_submission_layout.c i generator. Nastepne: dokladne offsety120B packet + PDS + profilUSC, nie kolejny smokeuchwytow.


## Decyzja wlasciciela — styl UI macOS 27

Aktualny kierunek stylistyczny MICROFW: **macOS 27**, zgodnie z poleceniem wlasciciela. Zastepuje wczesniejszy ogolny cel kopiowania interfejsu iPhone/iOS. Zachowujemy natywny desktop z oknami, runtime C/C++, angielski interfejs, kompaktowa skale, gesty i multitouch dostosowane do telefonu. Wczesniejsze zdjecia sa referencja wybranych efektow szkla/ruchu; przy konflikcie priorytet ma nowa decyzja o stylu macOS 27. Nie dopowiadano szczegolow wygladu konkretnej wersji bez referencji. To zapis preferencji, nie deklaracja gotowego wdrozenia.


## 2026-10-27 — offsety stockCDM120B i stan contextswitch
Bezoperacjinatel. NiezaleznyARM64disasm stocklibsrvRGXKickCDM ELF209f8 potwierdza packetbase=x29-0xf0/x26:6qwordruntime0..47, niezapisanywtymwrapperze obszar48..55,7qwordcontext56..111 (ostatniORhelper30044), flags112 i deadline116. Nie utozsamiamy niezapisanegoobszaru zpotwierdzonympaddingiem. HostCsizeof120/offsetof48/56/112/116 PASS. Mapaexpression/evidence wgpu-compute-packet-map.json; nazwyrejestrow pozostajaUNRESOLVED.
StatyczneCREATE ELF205f4 alokuje72BGeneral compute-state,alignment16; context+c8 otrzymujeGPUaddress&0xfffffffff0. KolejneparyPDS maja packingcode&fffffff0 |(data>>4)<<36, zgodny z publicznymi maskamiCDM_PDS0, leczdokladnerolestore/terminate/resume wymagaja potwierdzenia helperow2f650/2fa30/2ff10/30044. Brakshaderdispatch/restartu. Prywatnabiblioteka/decomp poza repo. Nastepne: dekompilacja helperow irekonstrukcja ograniczen/stateprograms; dopiero pozniej builder.


## 2026-10-27 — potwierdzone pakowanie CDM i czesciowe role helperow
StatycznyhelperELF30044(mode1): ARM64 LSL2/ADD63/LSR6/LSL11/ANDff800. Dokladnie publiczne nonTEMPSIZE8 CDM_CONTEXT_PDS1 COMMON_SIZE; jednostka64B/16dword. To nie unified_store_depth! NativeC mfw_cdm19_pack.h koduje tylko tenfield iheap-relative32bit PDS0 offsets. Guard8176dwordmax, alignment16, beztruncation, kontrola zajetegopola. ASan/UBSan8177wartosci plusboundarycasesPASS, publicmacrostaticassertPASS, ARM64-WerrorcompilePASS. Bezpacketbuilder/dispatch/telchanges.
Helper2f650 alokuje shared-state0x4030B orazPDSstore/load/fenceterm256B iUSCstore/load512B. Nie przeliczamy wielkoscialokacji na niepotwierdzony profilhardware. Helper2fa30 ma diagnostykeRGXGenerateContextLoadUSC/store, helper2ff10 RGXGenerateFenceTerminatePDS/ContextSwitchFenceTermTask. Czescrol rozpoznana; nadal doodtworzenia zawartoscPDS/USCcontextprograms, runtimewords0..47 iobszar48..55. JSONgpu-compute-state-packing.json. Prywatnydecomp/binary poza repo.


## 2026-10-27 — CDM store/load/fence PDS1 i role7slow
Na podstawie stockhelper2fa30 (param3store1/load0, diagnostykaprodukcjiUSC/PDS),2ff10(fenceterm) i publicznychmasek potwierdzono CDMnonTEMPSIZE8control:storebase28000001,loadbase00100000,fencetermbase28100001. Unifiedunits6bitshift21; PDSdata ceil(dwords/4)6bitshift1. Nie interpretujemy unifiedunits jakoUSCdepth. Nativepackguard u<=63/data<=252, fencetermu=0, odrzucaunknownmode. ASan/UBSan16192par na kazdySTORE/LOAD+fence/boundaryPASS i ARM64-WerrorPASS, obok poprzednich8177common-sizecases.
Role packet56..111:statebase,storePDS0/1,fencetermPDS0/1,loadresumePDS0/1. Pozwala wskazac odpowiedniki modernstaticstate(percontext) i cdm_context_state_base_addr/cdm_resume_pds1(perkick). To mapa rol zahelperami, nie sledzenie firmware; alternateBstate wnewAPI nadal bezodpowiednika i nie wolnopominac. Nadal brakpelnegopacket/programcontents/profilu, bezphonechanges/dispatch/reboot. Dowod i limitations wgpu-compute-state-packing.json.


## 2026-10-27 — PDS fence/terminate: porownanie publicznego kodera PASS
Dokladne publiczne funkcje Mesa (z zachowana licencja MIT) i publiczny inline encoder wygenerowaly piec instrukcji barrier-conditional + DOUTC + HALT identycznych ze statycznie odczytanym stockhelperem ELF0x36580. Test hostowy PASS, skrypt compare_pds_fence.py oraz gpu-pds-fence-comparison.json. Vendor runtime nie byl wykonywany. Potwierdza tylko ten fragment ISA PDS; nie potwierdza profilu, wymogu dodatkowego software-barrier, USC, pelnego packetu ani renderowania. Bez zmian telefonu/restartu. Nastepne: programy context store/load oraz runtime fields packetu i rzeczywiste dane profilu.


## 2026-10-27 — PDS kick USC: 256 porownan kodowania PASS offline
Publiczny pvr_pds_encode_doutu(cc,1,offset>>1) zgadza sie ze statycznym stockhelperem ELF0x364a8 dla wszystkich128 parzystych offsetow dword0..254 i obu wartosci predykatu (256 przypadkow). Wspolny minimalny layout:8dword danych,1 instrukcja,64-bit USC task control na poczatku danych przy poczatkowym offset0. Nie potwierdza to zawartosci USC task control ani semantyki helpera alokacji/alignment. Modern shared store/load dodaja DOUTW i feature-dependent bariery: NIE kopiowac calych generatorow tylko na podstawie zgodnosci DOUTU. Rozszerzony powtarzalny compare_pds_fence.py PASS; brak zmian telefonu/dispatch/restartu. Nastepna zaleznosc: dokladne USC task control, legalny program context store/load i nieznane feature gates.


## 2026-10-27 — USC task control context store/load rozpoznany czesciowo
Stockhelper ELF2fa30 buduje taskcontrol (descriptor+48):storebase0x1000000000,loadbase0x2800000000, OR(USCaddress & fffffffc). Publiczne maski PDS niezaleznie dekoduja TEMPS encodedunits2/5, instance sample rate0 i dualphaseOFF; host assertions PASS. Publiczny setup_doutu zaokragla temps/4, co w jego konwencji oznacza8/20dword. XMLpds ma UNIT_SIZE2: interpretacje sciezki XML trzeba sprawdzic przed translacja, nie uznawac jednostek za wymienne. Adres nadal wymaga potwierdzenia heap-relative semantics. To per-task allocation, NIE unified_store_depth ani usc_slots. Zadnego blobu USC nie skopiowano do repo/telefonu; brak dispatch/restartu. Wyniki i rozszerzony test wgpu-pds-fence-comparison.json/compare_pds_fence.py.


## 2026-10-27 — XML TEMPS: brak przeskalowania w rzeczywistym consumerze
Generated pds.h pack/unpack zachowuje TEMPS encodedunits bez mnozenia/dzielenia:384kombinacje64units*3sample rates*2phase PASS. Jedyny znaleziony consumer DOUTU_SRC0 wcmd_buffer patchuje sample_rate przez unpack/repack, pozostawia TEMPS. UNIT_SIZE2 jest emitowanym makrem bez consumerow wpinned imagination source; nie dowodzi konwersji/2. Poprzednia ostroznosc co do calego XML pozostaje, ale rozbieznosc kodowania tego pola NIE wystepuje: encoded2/5 zachowac bez przeliczania. Public execution-address helper upraszcza sie do address&fffffffc zgodnie ze stockexpression; nadal nie dowodzi pochodzenia/heapbase adresu. Test source i JSON rozszerzone; bez phonechanges/dispatch.


## 2026-10-27 — pochodzenie adresow USC/PDS: heap-relative
Nowy prywatny staticdecomp helpera ELF2f3a4: po mapowaniuVA,param7!=0 pobiera heapbase(helper10f3c) i odejmuje go odVA. ComputeCREATE znajduje PDS Code and Data/USC Code; helper2f650 przekazuje param7=1 dla obu rodzajow programow. USC taskcontrol otrzymuje zatem offset wzgledem USCheap, nie pelnyVA. Osobny Compute state wGeneral ma param7=0 i zachowuje pelnyVA. Dowod statyczny, bez nowego niezaleznego disasm wtym kroku. Native mfw_cdm19_heap_offset odrzuca underflow, >32bit, zle alignment iNULL; nie sprawdza ownership/rozmiaru sterty. ASan/UBSan nowe boundaries+regresje iARM64-WerrorcompilePASS. Prywatne decomp poza repo; bez telefonu/dispatch/restartu. Nastepne nadal legalne natywne contextUSC programs i realnyprofil.


## 2026-10-27 — otwarte USC context programs znalezione; kontrakt danych inny
Pinned Mesa common.cl zawiera cs_store_sr_1024_common/cs_load_sr_256_common/cs_load_sr_16_common. Aktualne artefaktyuscgen:544/488/1040B kodu,temps10/10/22; metadane8B odjete i size_dwords sprawdzone, SHA wgpu-public-context-programs.json. To otwarty kod bez vendor runtime. Jednak COMMON jest syntetycznym profilem pco_clc(unified64,slots14), NIE potwierdzonym BVNC22.49.21.16. Nie deployowano. Publiczny PDS dostarcza przez DOUTW dwa64bit adresy do USC(vtx inputs); stockminimalPDS robi tylkoDOUTU. Nie laczyc publicUSC zstockPDS bez translacji calego kontraktu. Stockbloby312/232B roznia sie rozmiarem, co samo nie dowodzi niezgodnosciISA. Kolejny krok: DMA/shared instructions, quirks i dopasowany statebuffer/PDS/USC; profil nadal blokuje dispatch. Brak telefonu/restartu.


## 2026-10-27 — mapowanie USC shared DMA i indeks errat
Publiczny trans_nir: dma_ld_shregs ->MOV index+LD_REGBL indexedSHARED/DRC0; dma_st_shregs ->MOV index+ST32_REGBL indexedSHARED/DRC0/IDFflag; allocsize ->MOV specialSH_ALLOC_SIZE. common.cl interpretuje allocation size jako bloki16dword; publicstate reserve48B. To mapa kodu, nie potwierdzenie instrukcji naY5. Reprodukowalny indeks exactlegacy+Mesa:60084/61450/65273/63553 nie maja referencji w pinnedMesa imagination;63027 tylko dwa komentarze firmwareTA dummyregionheader. Legacy65273 zmienia uklady/nazwy heapow (wtymUSC/PDS),63553 ma startstopguard. Legacy60084/61450 sa deklarowane/raportowane wKM, bez znalezionej implementacji shaderworkaround wtym drzewie. Brak referencji NIE dowodzi braku wymagan obejscia. JSON zawiera wszystkie trafienia, bezprywatnychblobow. Nadal nie wolno wlaczac niezweryfikowanych commonUSC; next heap65273/startstop63553 i exactcompiler errata. Bez telefonu/restartu.


## 2026-10-27 — BRN65273 heap bounds i BRN63553 firmware scope
Exactsource rgxheapconfig:workaroundPDSbaseA800000000/size40000000,USCbaseBA00000000/size40000000 (kazda1GiB, default4GiB). RGXInitHeaps wybiera je runtimebitmask65273; sa tez zmianyGeneral/non4K/VisTest/TQ iMMUregionheaders4GiBapart. Sam32bitoffset nie wystarcza: dodano mfw_cdm19_heap_range zkontrola calego niezerowego zakresu wzgledem przekazanego queriedheapsize, overflow/underflow/alignment. Nowe boundaries+ASan/UBSan+ARM64compilePASS. Nie hardcodowac sourcebase zamiast wyniku HEAPDETAILS. BRN63553 wstartstop dotyczy MIPSdelay-slotprefetch/TLBmiss iremap5 do bootphysical; to firmwarestartup, NIE dowod obejsciaUSC ani bezpiecznegoactiveGPUkexec. Dokument gpu-errata-heap-startup-audit.json. Brak zmian telefonu/restartu/renderu.


## 2026-10-27 — USC register allocator: wskaznik do callerparams, nie HWtable
PVRUniFlexCompileToHw zapisuje plVar5[0x22a]=param4, czyli state+1150 to pointer do parametrow programu odcallera. Regallocator ELF39dd0 odczytuje stamtad +14/+20/+24; nie wolno na tej podstawie wywnioskowac fizycznych unifieddepth/slots. Nowe prywatne decomp ELFcc08 tylko alokuje/zeruje78Bstate+34a8; ELF7fc6c buduje zakresy rejestrow zcallerparams, nie uzyskano nowego zrodla physicalcapacity. Wniosek eliminuje kolejny bledny trop, NIE rozwiazuje profilu. Zero vendor runtime/phonechanges/restartu. Dowodgpu-usc-capacity-provenance.json; rawdecomp prywatnie. Nastepnie szukac niezaleznych targetlimits/quirks lub udowodnionej restricted compiler path, bez traktowania COMMON64/14 czy0xf8 jako hardware.


## 2026-10-27 — kodeki: port ioctl nie zapewni dekodowania
Przy nadal nieustalonym profiluUSC zbadano osobny blokmedia.7/7 referencyjnych plikowMT6739 ma zgodneSHA. Indeks20 ioctlcase/17bezcase (leksykalnie, nie compiledcoverage). Driver mapuje MMIO/memory,lock/waitISR/power; hal.c SET_CMD_QUEUE/POWER/ISR to stuby. Brak pelnego framelevel decoder wtychplikach. H264/HEVC_DEC rozpoznane wcontrolbranch, HEVC_ENCenum NIE dowodzi enkoderaMT6739. Dalszy nativeport wymaga parsera/registerprogramming+DMA/IOMMU/V4L2, nie tylko ioctl. Stary mmap nie sprawdza calosci zakresu przezstart+length; nie kopiowac unchecked. Bez power/MMIO/phonechanges. JSONvcodec-native-interface-audit i docs/NATIVE_RENDER_MEDIA.md.


## 2026-10-27 — VDEC IRQ protokol czesciowo zgodny zmainline
ReferenceMT6739:VDEC_MISC=base+5000,IRQreg+a4,donebit16,ackOR11/write potemAND~10/write. Linux7.3 mainline uzywa tego samegoack. Waski protocolmatch, NIE dowod zgodnosci pelnegoengine/firmware/DT. MainlinelegacyH264 backend wymaga vpu_dec_init/start/reset, czego stockcontrolioctl nie dostarcza. Stockpower potrzebuje tez SMI_LARB_VENCSYS przedSCP_SYS/CG; failurepath tylko loguje i idzie dalej, native musi unwind. Dowodvcodec-irq-power-comparison.json/docs. ZeroMMIO/power/phonechanges.


## 2026-10-27 — Huawei4.4 korelacja VDEC potwierdzona statycznie
Ghidra actualstock dec_isr ffffff80084b9bf4 potwierdza MISC+a4/donebit16/OR11/AND~10 zDSB po kazdymzapisiem. vdec_power_on ffffff80084b6758 potwierdza SMIid1 i2zegary kolejno;off ffffff80084b6834 odwrotnie+SMIdisable. Nazwyclockglobals wymagaja korelacji zprobe, nie zgadywac. Symbolscan prefixvpu_/scp_/vdec_vpu/venc_vpu znalazl tylko weakscp_dump_pc/lr, beztransportucodecVPU. Brak symboli NIE dowodzi brakusilicon/protokoluwinnymkomponencie; scpsys to powercontroller, nie dowodVPU. Native registerpath wart dalszego badania; nie dodawaccompatible doVPUbackend bezABI. Rawdecomp lokalnieD/out, publicznefacts vcodec-huawei-stock-correlation.json. Bezphonechanges/restartu.


## 2026-10-27 — exactHuawei nazwy clockglobals VDEC/VENC
Actual vcodec_probe plusboundedASCII kernelstrings potwierdzily globals9040=MT_CG_VDEC,9048=MT_CG_VENC,9050=MT_SCP_SYS_VDE,9058=MT_SCP_SYS_VEN (pelneadresyJSON). Zpoprzednim powerdecomp daje rzeczywistyHuaweiorder:SMIid1->SCP_SYS_VDE->CG_VDEC;off odwrotnie. To nazwyconsumera4.4, NIE sprawdzoneproviderIDs7.3 lub zgoda nauruchamianie przyhackclockach. Brakpower/MMIO/restartu. Odtwarzalny boundedstringreader archiwizowany, wynikfacts vcodec-huawei-stock-correlation.json i docs/NATIVE_RENDER_MEDIA.md.


## 2026-10-27 — LIVE native compute-context CREATE/DESTROY PASS
NaY5/Alpine/v62 uruchomiono nowy samodzielnyCtest exactABI1.9 bridge129 CREATE0/DESTROY1,input32B/output12B,framework16zero(disabled),resume0,rzeczywistyhPrivData zmemorycontext. compute_call odrzuca fn>1: brakKICKCDM/shadera/registerpayload. Nativecc-Wall-Wextra-Werror ihostcompilePASS. Telefon:COMPUTE_CREATE_PASS,COMPUTE_DESTROY_PASS,RC0. Przedipo Driver/FirmwareOK,HWR0,CRR0,APM0. Cleanupcompute->memctx->DISCONNECT,zboundedretry25. Bezrestartu. To dowod cykluz ycia kontekstu, NIE render3D ani USCexecution. Profil/120Bpacket/programycontext nadalpending. Kodarchive/pc_work/compute19_context_trial.c,dowodgpu-compute-context-lifecycle.json.


## 2026-10-27 — CDM caller initialization rozdzielone od FWpacket
PrivateGLES callerELFb7664 zeruje112B inputRGXKickCDM, potem ustawia m.in.input3c=1,54=20hex,44=conditionalflag,34=ctx745c(clamp>=12),5c=ctx8288. Resourceorigins08/10/48 zapisane wgpu-compute-caller-map.json bezzgadywania nazwrejestrow. Wazne: memsetINPUT wGLES NIE zeruje niewypisanego packet48..55 winnymstackframe libsrv. Common-size z34 to perprogram allocation,NIE fizycznacapacity. To pojedynczasciezkaGLES i statycznydowod, nie kompletABI. Brakzmiantelefonu/dispatch. Kolejne:resourceidentities i niezalezna mapa rejestrow; nie budowacpelnegopacketu zhipotez.


## 2026-10-27 — debugfs nie rozwiazuje brakujacego profiluUSC
Read-only inventory/version naY5: debugfs pvr ma version/status/debug_dump/buildvar/apphint itd.; version potwierdza22.49.21.16(HW),1.9@4893595 mt6739_microfw. Nie znaleziono dedykowanego pliku unifieddepth/uscslots. Source _DebugDumpDebugSeqShow wywoluje PVRSRVDebugRequest(MAX), a nie prosty cached-profile getter; nie uruchomiono pelnego dumpu tylko po to by ponawiac niepotwierdzony trop. Source rgxdebug.c odwoluje sie do knownfeatureconfig, ale nie ma nazwUSC_SLOTS/UNIFIED_STORE; to NIE dowod braku mozliwosci innego odczytu. Rzeczywiste unifieddepth/slots nadalUNKNOWN; nastepny shader potrzebuje niezaleznego dowodu targetlimits/quirks lub formalnie ograniczonej sciezki bez arbitralnychcapacity. Brak nowegobuild/testjob/restartu. Nie powtarzac poprzednich lifecycle/NULLtests bez nowejzmiany.


## 2026-10-27 — KOREKTA: invalid syntetyczna depth16 wstarym sweep
Source rogue_hw_utils: min_attr_lines=floor(depth*8/128)-2 unsigned. Dla16 wynik underflow, potemtempsclamp248 maskowalblad! Dla32 zerocapacity. Historyczne25/30success nie oznacza25poprawnychprob; cases16/32 oznaczonoINVALID woryginalnymJSON, pozostawiono surowe wyniki. Offlineprobe dostal guard floor(depth*8/128)>2 dla nonzero;16/32 exit2PASS. Nowy odczyt pco_get_precomp_data (nie pierwotnydata copy) dla synthetic64/4 daje compute128B,temps5,vtxins1,shareds0. To nadal NIEtargetlimits/ISAvalidation. Dla explicitWG1/no-barriers usc_slots wrogue_max_wg_temps sluzy assertion maxinstances; barrierarithmetics nieosiagalna wtejfunkcji. Nie dowodzi niezaleznosci calegocompilera. Kod/JSON/plan poprawione; nic nie wyslano naGPU.


## 2026-10-27 — zawężenie zapytan profilu wPCO, bez deklaracji pelnej reachability
Skan executableC/H PCO zodjeciemkomentarzy iuscgen znajduje4sites:RA rogue_get_temps/rogue_max_wg_temps, interpolation usc_itrsmp_enhanced, texture quirk74056. To indeksleksykalny, nie wholeprogramproof. Dla prostego normalcomputeWG1/globalstore/no-texture/no-interpolation trzeba udowodnic nieosiagalnosc odpowiednichhelperow zamiast wypelniac74unknownfalse. Nadal RA wymaga udowodnionegobudzetu iISC/errat, submission/contextprogramcontract unresolved. Indeks i ograniczenia gpu-pco-query-sites.json; odtwarzalny skryptarchiwizowany. Bezpowtarzaniatestjob/buildu i bezphonechanges.


## 2026-10-27 — restricted compute finalNIR whitelistPASS offline
Dodano kontrole finalNIR poPCOlowering:computeWG1, dokladnie1load_local_invocation_index i1store_global_2x32; odrzuca kazdytex iinnyintrinsic. Buildsynthetic64/4PASS,128B/temps5/vtxins1/shareds0. DumpNIR pokazuje index<1 iwarunkowystore0x12345678. Zatem wybranyfinalNIR nie podaje translatorowi texture/interpolation/barrierintrinsics; zawęża weryfikacje tychsciezek, nie dowodzi pelnejniezaleznosci/ISA/hardwareprofile. Nadalcapacity/errata/contextprograms/FWpacketpending. Kodarchiwizowany,JSONgpu-pco-query-sites rozszerzony. Brakuploadu/phonechanges/restartu.


## 2026-10-27 — finalgrouped computeIR: PDS musi dostarczyc vi0
Nowydump finalPCOIR offline synthetic64/4:12instructiongroups,temps5,vtxins1/shareds0. vi0 jest localinvocationindex,porownanysigned<1; compiler dodal conditional-execution prologue/epilogue z r4,cndst/cndend/branch. Store:writethrough32bit,DRC0 potemWDF. Nie jest to bezwarunkowysinglestore. Waznykontrakt: PDS musi ustawicvi0=0 dlajednejinvocation; nie wolno zostawic wejscianiezdefiniowanego. Listaoperacji i dispatchrequirements wgpu-restricted-compute-ir-audit.json. Nadalniepotwierdzone targetopcodes/errata,tempallocationunits,packet/contextprograms/cachecompletion. Kodprobe archiwizowany; brakbinaryupload/phonechanges/restartu.


## 2026-10-27 rano — publiczny generator computePDS zinputvi0 PASS offline
Telefonread-only:Firmware/DriverOK,HWR0,CRR0,APM0. Hosttest linkuje rzeczywisty pinnedMesa libpvr_pds.a, initunusedregs,localX->vi0,kickUSC,temprequest5,bezbarrier/fence/conditionalrender. Wynik3dworddata,20BAJTOWcode=5instrukcji,PDS3temps. Zlapano roznicejednostekcode_size(bytes)/data_size(dwords); pierwszywydruk mialnadmiarowe canarywords, poprawiono przedarchiwizacja i potwierdzono returnedpointer+5. Boundedcanaries iinstrumentowanyharnessASan/UBSanPASS (samastatycznalibrary nieprzebudowanasanitizerami). slc_mcu_cache_controls0/1 daje identycznywynikdlategopath. To publicPDS generator contract, nie targetISA/physicalCDMinputproof. Programu nie wyslano naGPU; adresUSC100hex to placeholder. Dowodgpu-compute-pds-input-candidate.json,odtwarzalnytest/buildscript warchive. Dalszehardgates:profil/ISAerrata,legacycontextprograms i120Bpacket48..55.


## 2026-10-27 rano — niezależny publiczny układ pakietu compute 120 B
Znaleziono historyczny rgx_fwif_client.h w android.googlesource kernel/mediatek, commit4f43e6b499c6d194030df8d2506485db9d5165bd (SHA iURL wgpu-public-legacy-compute-layout.json). Wycięte deklaracje z zachowaną MIT/GPLnotice: wariant bez opcjonalnych featurebranches daje56B runtime+56B context+flags/deadline=120B. Host sizeof/offsetofPASS iARM64compilePASS. Kandydaci runtime0/8/16/24/32/40/48:CDMstream,bordercolour,threadbarrier,PDSctrl,YUVcoeff,TPU(lubCEM),TPUarrays. Kolejność7context ioffsetyflags112/deadline116 zgadzają się z niezależnym stockanalysis. To pierwszy niezależny publiczny opis zgodnego kształtu, NIE dowód exactDDK1.9 ani braku opcjonalnych cech. Field48 nadal niezapisany wstockwrapper: publiczna nazwa nie dowodzi użycia/ignorowania przezFW. Next targetmasks PDSctrl44000000/TPU20 i konsumenci. Brakjob/phonechanges.


## 2026-10-27 — PRZEŁOM ABI: pełny DWARF we właściwym firmwareGPU
Z/tmp/mfw-gpu-runtime/firmware/rgx.fw.signed pobrano prywatną kopię naPC;1265664B/SHA8a843eb1...f9fa074c zgodne z działającymfirmware. ELF32little microMIPS32r2 ma4001symboli i pełnyDWARF. Nie uruchamiano naPC, nie commitowano blobu/decomp. Pyelftools potwierdził we wszystkich powtórzeniachCU identyczne typy:CDM_REGISTERS56B,CSWITCH56B,CMD_COMPUTE120B,flags112/deadline116. Wszystkie7nazw runtime i7context zgodne zpublicznymhistorycznymheaderem. To już exactfirmwarefacts, nie wariant hipotetyczny. COMPUTECTX_STATE8B zawiera bBufferBoffset0.
rgxfw_compute_kickcdm c0014828/1042B:DWARFlocations psCmdHeader=r16,psComputeCmd=r16+56. MicroMIPSdisasm koreluje pola zrejestrami (mapaJSON); PDS_CTRL jestRMW, YUVcoeff ustawiane warunkowo. Field48 toTPU_ARRAYS_CDM; brakbezpośredniegoloadr16+68/6c wtejfunkcji nie dowodzi braku użycia wszędzie. Firmwarewindowcf400000 NIE jest CPUphysicaladdress. Ekstraktor/metadane do repo, rawfirmware lokalnieprivate_gpu_reference. Brakjob/restartu, profil/ISA/contextUSC nadalpending.


## 2026-10-27 — KOREKTA i PASS: stock context PDS zawiera dwaDOUTW
Wcześniejszy wniosek, że stockcontext używa tylkoDOUTU i innego kontraktu niżMesa, był błędny: decompiler pominął argumenty helpera36230. Pełnydecomp tegohelpera ma3args, ARM64calls364c8/36524 zachowująx1buffer/w2mode. Helper emituje2pointerDOUTW (stateVA,stateVA+48), danehigh32/low32+controlwords2/80004002, a364a8 dopisuje taskcontrol+DOUTU. Całyprogram:16dworddata,6instrukcji z3instr.barrierprefix,nie8/1.
Publiczny rzeczywisty generatorMesa z2DOUTW/cc=true plusbarrierprefix porównany ze starymi expressions:wszystkie16data/6code zgodne dla storetemps8 i loadtemps20. ASan/UBSanharness4cases(store/load*SLCvariant) +returnpointer/canariesPASS; equality tylko gałąź kodowania zgodna zestock(variant0),variant1size/bounds. PublicUSCshaders/profil/errata nadalniepotwierdzone. Zaktualizowano błędne JSONassertions; pierwotne notatki zachowanehistorycznie. Dowodgpu-pds-context-pair-comparison.json,kodtestówarchive. BezGPUupload/restartu.


## 2026-10-27 — restricted RA bez syntetycznej pojemności profilu
Samodzielny wariantpco_ra zbudowany przezodtwarzalną transformację pinnedsource (MIT): softwarebudget8,guardscompute/WG1/no-barriers/no-shareds i końcowe no-spill/temps<=8. ProdukcyjnaMesa niezmieniona. Probe depth0 pozostawia unifieddepth/slotsunknown,override nie odczytuje ich. Offlinewynik128B,temps4/vtxins1/shareds0,SHA3c15b861...ba646. Nieidentyczny zpoprzednim synthetic64/4(temps5): innydobór rejestrow. Jawnysoftwarebudget NIE jest pomiarem hardware,ISA/errata/alokacjatargetu nadal wymagają testu. Brakphoneupload/dispatch. Skryptarchive/build_pco_budget8.py iJSONgpu-pco-budget8-experiment.


## 2026-10-27 — natywne USC context candidates i poprawka partialDMA
Public common.cl load_store_sr odejmował całyblocksize nawet gdy ostatni transfer mniejszy: unsignedunderflow dla16dw przyblock256/1024. CPUmodel odtworzył błąd i po odejmowaniu min(remaining,block) przeszedł1539legalnych16dwcases zASan/UBSan. To testharmonogramutransferów, nie GPUISA. Patchmesa-context-sr-last-burst.patch zapisany iSPIRV przebudowany.
Wybrane publiczne store/load skompilowano własnymstandalonePCO zsoftwareRA budget20,guardno-spill/temps<=20,unknownprofile bezcapacityvalues iPCO_DEBUG=no_pred_cf jakupstream. Wyniki store568B/load488B,oba10temps,hashJSONgpu-native-context-usc-candidates. BrakBionic/HAL/vendorCPUruntime. Budzet20 nie jest fizycznąpojemnością. ISA/context execution/memorysync jeszczehardwarepending; nic niewysłano. Zwykły generateduscgen wbuild-mfw jest po zmianieSPIRV stale, dopóki niezostanie przebudowany normalnytarget (nie mylić znowymikandydatamiout).


## 2026-10-27 — LIVE compute memory footprint:10pages/3heaps PASS
NowynativeC pyta exactlegacyHEAPDETAILS6/30(input20/out40) i używa rzeczywistych nazw/base/size. General65c0000000/80000000,PDSa800000000/40000000,USCba00000000/40000000. Mapowanie8sąsiednich stronGeneral (wtym5pagearena na16KiB+48Bstate),PDS1page,USC1page,łącznie10PMR4K. Każda strona zero-init+unikalnyCPUwrite/read1024wordsPASS, GPUVAmapPASS. PełnyreversecleanupRC0;Firmware/DriverOK,HWR0/CRR0/APM0. Native i host-WerrorcompilePASS. BrakzadaniaGPU/restartu:mapowanie nie dowodziGPUcachevisibility/ISA/renderu. Źródłoarchive/pc_work/compute19_memory_preflight.c,dowodgpu-compute-memory-preflight.json.


## 2026-10-27 — pierwszy rzeczywisty USCstore: FAIL/HWR1; brak MMUinitBRN65273
Zbudowano5programówPDS+nativeUSCimage+60BCD Mstream zbarierąkońcową, exactFWwire120B generowany zDWARFfactów. NativeStaging10pages/SHAPASS bezkick. Potem pojedynczyKICKCDM129/2 zaakceptowany:UFOzmieniłsię na87654321, aleoutputdeadbeef nietknięty,RC1. Driver/FirmwareOK,HWR0->1,CRR0; cleanupukończony/procesniepozostał. To NIEshaderPASS, HWRmoże ukończyćUFO.
DebugrecoveryCDM:MMU41,request4bf800000000,TA TERegionHeader zapisF800000000. To dokładnieMMU_INIA_BRN65273 heap. Nowystaticdecomp actualstockPVRSRVCreateDeviceMemContext18efc potwierdza inicjalizacjęMMU INIA/INIB przezhelper1904c:5B/alignment512/flags3+GPUmap. ARM64immediates sprawdzone. Naszklient pominąłte2bufory; silnaprzyczyna pierwszegofaultu, ale ISAshaderanadalniesprawdzona. Nieponawiać bezuzupełnienia obu mapowań i osobnegostagingu. BaselineHWR1. Źródła/pakietmetadataarchiwizowane, binariakandydatówtylkoD/out. Dowodgpu-first-store-trial-failure.json. Bezrestartu/bootwrite.


## 2026-10-27 — PRZEŁOM: pierwszy rzeczywisty nativePCO USCstore PASS
Po dodaniu własnych mapowań4K MMU INIA/INIB BRN65273 (F8/F9,actualqueriedheaps) osobnystaging12pages/5heapsRC0. PojedynczynowyCDMkick: USC_STORE_READBACK_PASS output12345678 zdeadbeef,UFO87654321 z12345678,1023pozostałesłowanienaruszone,trialRC0. Kompletnycleanupimportsync/computecontext/syncblock/12PMR/5heaps/memctx/DISCONNECT,procesniepozostał. Driver/FirmwareOK,HWRprzed1/po1(no new),CRR0;ponownystatusidleGPU0%. Bezrestartu.
CałyCPUstackAlpineC/exactlegacyABI; shader i contextUSC zpublicMesaPCO,bezBionic/HAL/vendorCPUexecution. UżytysoftwareRA budget8 nie jestcapacityprofile; wynik4temps/1vtxin/128B. Ten test potwierdza WYBRANYshaderISA/path,niepełnyprofil,niepełneerrata/contextswitchiNIE3Dtriangle/akcelerowanydesktop. Dowodgpu-native-usc-store-pass.json,actualsourcearchive/compute19_store_trial_mmu.c. Następnydowód:innyliteral+innyoutputoffset ioracle, potemTA/3Doffscreen.


## 2026-10-27 — niezależne wyzwanie nativecompute:innyliteral/adres PASS
Ponownakompilacja ograniczonegoPCO zmieniła literal naA1B2C3D4 i docelowyGPUVA na65c0000040(word16). Kod128B/temps4,nowyhash039a121c...ae770. NowyUSCimageSHA59e047bd...bd113 zweryfikowany natelefonie;osobnystaging12pagesRC0. Job:USC_STORE_CHALLENGE_PASS outputa1b2c3d4,UFO87654321,1023guardwordsPASS,RC0/pełnycleanup. HWR1->1,CRR0,Driver/FirmwareOK,idle0%. To potwierdza zależność rzeczywistegowyniku od nowegoshadera i adresu; nadal NIEtrójkąt/fullMesa. Właścicielowi wyjaśniono rozróżnienie.
Jedna próbalokalnegohashcheck przezPS$() została błędnie przechwycona przezPowerShell i wykonała nafonie pustytest; bezmutacji. Naprawiono przezsaved.sh i prawidłoweSHA przedstagingiem. Kod/JSONgpu-native-usc-challenge-pass zapisane. DalejTA/3D rendercontext/freelists/HWRT,VS/FS/PDS/commandstreams.


## 2026-10-27 — LIVE rendercontext i exactTA/3D firmwaretypes
Nativebridge130 CREATE12/DESTROY13,input32/output12 (VDMstackuint64offset4) zownedmapped4KGeneralcallstack i disabled16Bframework:CREATE/DESTROYPASS,RC0/pełnycleanup10pages. FW/DriverOK,HWR1unchanged/CRR0/id le0%,brakTA/3Djob. ExactloadedFW DWARF:CMDTA280B,CMD3D664B,TAREGS104B,3DREGS608B,TACSWITCH112B,HWRTDATA328B,FREELIST64B,offsetypólJSONgpu-exact-firmware-render-dwarf.
Kernelguardprzedfreelist:PMRWritePMPageList odrzucaCPU_READABLE/WRITEABLEtablePMR. Nie używać zwykłej CPUmappedtestpage jako backingfreelist; osobnyGPU-onlyPMR. Lifecycle rendercontext nie dowodzi renderowania/stackdepth. Kod/dowodgpu-render-context-lifecycle.json wrepo.


## 2026-10-27 — LIVE nativefreelist32pages CREATE/DESTROY PASS
Legacybridge130/8,9,input52/out12,globalNULL,max/init32,grow0. Osobnyowned4KtablePMR wGeneral bezCPUread/write permission, bezCPUmap. Kernelutworzyłpagelist/pool,freelistdestroy/rendercontextdestroy/pełnycleanup11pagesRC0;FW/DriverOK,HWR1unchanged/CRR0/id le0%. BrakTAjob.
WAŻNE: diagnosticznePMR nie ma jeszczePM_FW_PROTECT. PinnedmodernflagaDEVICEbit26 odpowiada oldPMMETA_PROTECTbit24; adaptercelowoodrzucadevicehints. Nieforwardowaćrawbitów ani nieuznawaćCREAT E za dowódgotowościTA. Następnykrok jawneprzetłumaczenietegojednegopola,testy iGPU-onlyprotectedtable; potemglobal/localpair/HWRT. Dowodgpu-freelist-lifecycle.json.


## 2026-10-27 — PMprotect translacja +livefreelistPASS
Jawna translacja nowoczesnegoPM_FW_PROTECTbit26->oldPMMETA_PROTECTbit24 wallocation/map. Pozostałedevicehints odrzucone; protectedPMR zCPUread/write permission odrzucane, kernel-onlymapping dozwolone dlaPMRpagelist. Explicitflags/negativeguardsASan/UBSanPASS,pełnyistniejącyadaptermockPASS. NowypatchapplycheckPASS poLFnormalization (pierwszycheckodrzucony przezCRLF);zastosowany doWSLMesa iDcanonicalheader.
NativeAlpineprotectedGPU-onlytable+freelist32pagesCREATE/DESTROY/rendercontextcleanupRC0,HWR1unchanged/CRR0/FWOK. Pełnyhostlibvulkan_powervr_mesa.so rebuildPASS;normalUSCgen został terazodświeżony po poprawcepartialDMA, wcześniejszywarningstale jużnieaktualny. Hostx86_64build NIEphoneICD aniTA/3Dproof. Patchmesa-ddk19-pm-protect.patch,dowodgpu-freelist-pm-protect-validation.json.


## 2026-10-27 — chroniona para global/local freelist PASS
Dwie osobne chronione tabele GPU-only, po32 strony global/local; lokalna lista wskazuje uchwyt globalnej. Native CREATE/DESTROY PASS, cleanup lokalna->globalna->rendercontext->12PMR, RC0. Driver/Firmware OK, HWR1 bez przyrostu, CRR0, GPU idle0%. Bez TA/3D submission: nadal brak trojkata/pikseli3D. Exact legacy enum potwierdza kolejnosc tablicy HWRT: LOCAL0, GLOBAL1, count2; nie zgadywac. RenderTarget bridge130/2 input8/output16, destroy3 input8/output4. Kod i dowod gpu-freelist-pair-validation.json zapisane.


## 2026-10-27 — RenderTarget lifecycle PASS
Exact bridge130 CREATE2 input8/output16, DESTROY3 input8/output4. Wlasna wyzerowana strona4K General jako adres VHeap; kernel zapisuje adres w strukturze FW bez wykonywania TA. CREATE/DESTROY PASS, cleanup target/listy/context/12PMR RC0; HWR1 bez przyrostu, CRR0, Driver/FirmwareOK, idle0%. To tylko lifecycle, NIE dowod poprawnego rozmiaru VHeap do renderowania ani pikseli3D. Przed TA ustalic rozmiary/backing HWRT i commandstream. Dowod gpu-render-target-lifecycle.json, kod archive/pc_work/render19_target_trial.c.


## 2026-10-27 — HWRT lifecycle PASS
Exact legacy bridge130 CREATE0 input98/output32, DESTROY1 input8/output4. Staticassert tailoffset40/maxRTsoffset96. LOCAL0/GLOBAL1, maxRTs1, wlasne wyzerowane4K adresy VHeap/MList/Tail, pola geometrii zero. CREATE zwrocil uchwyty cleanup/RTA/data i FWaddress. DESTROY i reversecleanup target/list/context/12PMR RC0, HWR1 bez przyrostu, CRR0, Driver/FirmwareOK, idle0%. Brak TA/3Dsubmission. To walidacja ABI i lifecycle, NIE konfiguracja gotowa do renderowania: rozmiary buforow, geometria, ochrona MList/VHeap i stream wymagaja ustalenia przed zadaniem. Dowod gpu-hwrt-lifecycle.json, kod archive/pc_work/render19_hwrt_trial.c.


## 2026-10-27 — kandydaci VS/FS bez syntetycznej capacity
Standalone PCO softwareRA budget20, rzeczywiste depth/slots nadal UNKNOWN. VS176B/6temps/1vtxin/0shared, FS440B/9temps/0vtxin/1shared; no-spill guard PASS. Budget8 VS PASS, FS trafil w nieobslugiwany spill setup guard; nie wlaczono spill. FS wymaga dokladnie fs.meta sharedDWORD0 (public metadata allocator), sampleID/coverage/ISPfeedback. Upstream FSmeta dla single sample/color0enable daje0x202 (samplemask1<<9,colorwrite1<<1,alpha flags0); konieczny poprawny PDS load. Nie traktowac tego jako hardwareproof. Kandydaci tylko offline, brak upload/renderu. Dowod gpu-graphics-bounded-candidates.json; build scripts archiwizowane, production compiler bez zmian.


## 2026-10-27 — exact stock v1 render geometry
Statyczna analiza RGXAddRenderTarget actual libsrv_um.so ELF220e4, niezaleznie ARM64:222c0..222dc ceil(width/16),ceil(height/16);22314..2231c region bytes=align64(tilesX*tilesY*5);223b8..223c0 tail=nextpow2(max(tileX,tileY))^2*8 dla sample1/layer1. Dla64x64:4x4tiles, region128B, tail128B. Dokladne przypisanie lokalnych registerfields do bridge jeszcze pending; generator oznacza grid_candidate. Nowa lukaMesa: pvr_arch_rt_mtile_info_init przy SIMPLE_IPF wymaga version2, target ma1; nie usuwac assert ani udawacv2. Publichelpers odrozniaja1x1macrotiles/v1 od4x4/v2. Private decomp/disasm poza repo; zapisane tylko fakty i wlasna arytmetyka CPU. BezGPUjob. Dowod gpu-v1-render-layout-correlation.json.


## 2026-10-27 — PDS grafiki i stock HWRT mapping
Public generators: fragmentkick data8/code1DWORD,9USCtemps; FSmeta shared0 value202 data2/code1 (LOWER DWORD/commonstore,last), required16byte alignment; vertexID->vi0 data3/code3/PDS temps2/2mapentries. HarnessASanUBSan+canariesPASS, linkedPDSlib existingbuild. Poczatkowo zly enum i data+1alignment poprawiono; paddingkick jawniezero. Nadal offline, phase_change i featurebranches wymagaja finalnego metadataguard.
Stock RGXAddRenderTarget->FUN_0010dbd8 potwierdza98BinputHWRT. Dla64x64/sample1/layer1/flags0:PPPscreen003f003f,PPPgrid0,multisample0,TPCstride0/TPCsize128,TEscreen3003,TEAA0,TEMTILE1/2=00100000,stride16. Wczesniejsze grid_candidate nie jest PPPgrid: to inne lokalne pole. Stock allocTPC+VHeap=align64(TPC)+0x600;VHeapoffsetalign64(TPC),RTC tylko arrays. Dokladne mergefloatconst odczytane zELF; callsiteARM64 i float32packing jeszcze sprawdzic przedsubmit. FaktyJSONgpu-graphics-pds-candidates/gpu-stock-hwrt-field-map. Decomp/blob tylko private.


## 2026-10-27 — exact renderFW wire header host/ARM64 PASS
Zapis ukladu bajtowego z exactloadedFW DWARF:TAregs104,3Dregs608,CMDTA280,CMD3D664. Kazde znanepole ma sizeof/offsetof staticassert; nieznana tablica ZSlice pozostaje opaque gap16B bez zgadywania count/type. Host i natywny AlpineARM64 compile/runPASS. Header nie ma API submit; zadnegoTA/3Djob. Plik phone/mesa/mfw_render19_fw_wire.h, generatorarchiwizowany, dowod gpu-render-wire-layout-validation.json. Dalej VDM/PPP/ISP/PBE/EOT i synchronizacja.


## 2026-10-27 — KICKTA3D bridge layout host/ARM64 PASS
Exactgeneratedlegacy IN352B/OUT16B, pola i offsety wlasny storageheader+staticassert. Niezalezny actualstockwrapper ELF e26c:bridge130/function14,input160hex/output10hex. CallerRGXKickTA przekazuje280BTA i664B3D, zgodniezexactFW DWARF. Host i nativeAlpineARM64compile/runPASS; brak submithelper/zadaniaGPU. Semantyka sync/PR/refcounts i wszystkieownedresources wymagane przed uzyciem. Pliki mfw_render19_bridge_wire.h i gpu-render-bridge-layout-validation.json.


## 2026-10-27 — stock CAM, VDM/PBE/EOT candidates
ExactGLES helperELFe3040 i caller4f1c8:output4+raster daje vector36<56 =>CAM31/maxInstances16; ARM64progi/pisanieoutputs sprawdzone. Nieustala calejcapacity. PublicpackersVDM8wordsPASS, ale segment bezPPP/syncNIEsubmitready. Poczatkowy recznyoracle mylil enumScratchFIVE z1;XMLpotwierdza0 i oraclepoprawiony.
ActualpublicPBE packer zASan/UBSan+canary:RGBA8linear64,stride64,target65c0010000,state70004000/00082019,regs0200007c68800000/0000003f0000003f/0. EOTnativePCOsoftwareRA20:40B/2temps/0shared/internaltrue,no-spill, IR2immediates+emitpix.freep.end; niewykonany naGPU. PDS EOTdata2/code1PASS. Bledny include pipe/p_defines poprawiony na aktualnyvk_format. Binariapoza repo. DalejPPP/ISP/contextswitchownedresources/fences. Dowodgpu-vdm-pbe-eot-candidates.json.


## 2026-10-27 — korekta VS5, FSdiscard, PPP iTAcontext
Finalne pco_shader_data ujawnilo implicit PSIZE:VSoutput5 (Position0..3,Pointsize4), nie4. Wczesny VDMcandidate4 NIEzostal wyslany; poprawiono na5, PPPoutputselect05040000. CAM31/16 nadal ten sambranch40<56. FSkontrakt phase0,z/w0,coeff0,meta0:1,discard1 przezcoverageprolog; publicpipeline wymaga PUNCH_THROUGH, nieopaque. Shaderymaja niezmienioneSHA zpoprzednichkandydatow.
PPP25words z viewport64/FSmeta/shared/PUNCH_THROUGH generatedpackersASanUBSan+canaryPASS; clipalignment16 nadalstockpending. TAcontextPDS:PT128Bstore8data/5code,resume8/6;SRpair16data/3code,ccfalse bezbarierCDM; harnessPASS. ActualstockRGXCreateRenderContext potwierdzaVDM92/TA484/PT128/SR4030hex perbank; DWARFasTAState bounds1 =>2elementy48B. Nie kopiowano kodu/blobdo repo. BrakTAjob. Dowodygpu-graphics-final-contracts/gpu-ta-context-array-dwarf.
Wlasciciel rozszerzyl cel na akcelerowanyWayland iWebGL2. Po offscreenpixelproof trzeba wdrozyc ARM64Mesa/winsys, prezentacjeDMA-BUF/KMS i przetestowac rzeczywisteGLES3/WebGL2wprzegladarce; pojedynczytrojkat nie zamyka tego celu. Nocnaautomatyzacja wstrzymana na prosbe, praca kontynuowana wtymczacie.


## 2026-10-27 — LIVE pelny footprint32PMR/5heap PASS
NativeAlpine mapuje32strony:General15(PBEoutput16KiB+VDM/PPP/TPC/VHeap/region/VDMstate/TAstate/MList/stack/dummyStencil/2protectedfreelisttables),PDS13(programs/PT/2x5pageSR),USC2,MMUINIA/B2. Zero-init+uniquepatternCPUread/write30pagesPASS,2GPU-onlyprotectedtablesbezCPUmap. Pełnycleanup32PMR/5heapsRC0,Driver/FWOK,HWR1unchanged/CRR0,idle0%. Pamiec zwolniona, zadnegoTA/3Djob. Dowodgpu-graphics-memory32-preflight.
TA112contextpacket z2bankami48B:publicpackers+actualstocktaskoraclePASS;SRstore22001004/resume20001004,state1data|5,PTstate420. Nativepublicvs_nop_common doVDMsync kompiluje8B/0temps przyunknownphysicalprofile/softwareRA20; brakGPUexecution. Stockhelper2e9ac sugeruje selectedoneoutput/maxTiles1 PIXEL_OUTPUT_CTRL1fc iISPctlpartial0/fixed80000; ASMcorrelationjeszczepending,nieustalacglobalnychcapacity. NadalPR/fence/refcountsemantics dozweryfikowania:oldbridge bezwarunkowowymagaPRFenceSyncPrimBlock, nawetgdybKickPRfalse; nieprzekazywacNULL. Dowodgpu-ta-context-sync-candidates.


## 2026-10-27 — graphics64 bundle gotowy; USB zniklo przed stagingiem
Zlozono8plikow kandidatow:2xUSC4KiB,2xPDS4KiB,VDM4KiB,PPP4KiB,TAcontext112,PBE32. MainPDS=VS/meta/FS/EOT/VDMsync,contextPDS2banki8programow; strictconstmap,bounds,ASanUBSanPASS. EOTprzekompilowany na outputVA65c0000000 zgodnyz32pageplan (poprzedniEOTtarget65c0010000 byl innycandidate). PBEstate70000000/82019. KontrolowanePPPterminate32pixelblocks nadalpubliccandidate, niehardwareproof. Nativegraphics19_stage_trial.c hostWerrorPASS:zaladujeobrazy i utworzy64x64HWRT zownbuffers; nie ma wywolaniaKICK.
Transfer ACK3plikow general-ppp/general-vdm/pbe-state. Przy pds-context listener setup WinError10054; potemPING2/2timeout i telnetconnecttimeout. RNDIS nieobecny wGet-NetAdapter i brakpasujacegoUSB/Pnpwfiltrze. CauseUNKNOWN, nieprzypisywacGPU bezdowodu. ZadnegoTA/3Djob/reboot/flash/kexecnieuruchomiono. Nativeclientniewgrany/niewykonany, resztapayloadow niewyslana/niezweryfikowana. OstatniGPUstatusprzedzerwaniempolaczenia FWOK/HWR1unchanged/CRR0/idle0%. Potrzebne zdjecieekranu wlasciciela zgodniezregulaPING.
Po odzyskaniuUSB:uptime/uname/status/logi, sprawdziclistener9500 przedponownymtransferem, SHAwszystkich8plikow, nativecompile/runstage; dopieropotempelneFWpacket+PR/fence/oracle i kontrolowanydraw. Nie uruchamiac run_graphics64_stage.sh przybrakujacychplikach;set-e/SHAcelowojeodrzuci. Autorniewlaczalautomatyzacji ponownie. Dowodgpu-graphics64-bundle-staging.json, kodarchivePC,binariatylkoD/out/graphics64_trial1.


## 2026-10-27 — dokumentacja pakietu renderu dla drugiego agenta
Na prosbe wlasciciela dodano docs/GPU_RENDER64_PACKAGE.md i GPU_RENDER64_MANIFEST.json:opis sciezki,8plikow/SHA,adresy32PMR,wynikifaktyczne i kandydacioffline,korektyVS5/FSdiscard/v1,procedurastagingu orazpozostalysubmission/Wayland/WebGL2. Beznowychtestowtelefonu; po komunikacie wlasciciela „jest” lacznosci nie zmierzono ponownie. Binaria zostajaD/out,bezrepo. NocnaautomatyzacjaPAUSED, jedenagentnaraztestujeGPU/restartuje.


# 2026-10-28 — przejęcie od Claude: replay i blokada po aktywnym GPU kexec

## Zmierzone przez Codexa
Telefon odpowiada na USB, Linux7.3 #48, uptime przy przejęciu29min. Brak pvr/status i render node; dmesg PVRSRVDeviceCreate Failed to get device config / INIT_FAILURE. Nie uruchamiano replay/firmware ani kolejnego kexec.

Read-only helper y5_gpu_power_read.c zbudowany natywnie -O2 -Wall -Wextra -Werror i wykonany: SPM_PWR_STATUS=b0005f5c, SPM_PWR_STATUS_2ND=b0005f5c, MFG_PWR_CON=0000000d, MFG_CORE0_PWR_CON=0000000d. MFGbit4=1 w obu. Helper mapuje tylko SPM10006000 PROT_READ, nie zapisuje MMIO i nie czyta rdzenia GPU.

W źródle aktualnego WSL mfw-ddk-power.c:mfw_ddk_power_create odrzuca !mfw_off z -EBUSY; SysDevInit zamienia ten błąd na INIT_FAILURE. mfw_off sprawdza mask80000010 w SPM180/184. Wynik odczytu spełnia warunek odmowy. Nie instrumentowano zwracanego errno, więc nie twierdzimy, że wszystkie wcześniejsze warunki zostały niezależnie wykluczone. Aktywny GPU kexec wraca do Linux/USB, ale NIE zapewnia poprawnego przekazania GPU. Nie traktować kexec jako bezwarunkowego resetu budżetu GPU.

Użytkownik poproszony o pełne wyłączenie/włączenie i Power na LK; potwierdził możliwość, oczekiwany komunikat po starcie. Nie wykonywać ręcznie raw powerdown ani unbind PMIC/VCORE. Brak powodów by teraz uruchamiać stock gltri: problem występuje przed firmware.

## Wyniki Claude — przekazane, niepowtórzone przez Codexa
Źródło D:/y5_linux_work/DOKUMENTACJA/PHASE2_BISECT_RESULTS.md. A0/A1/A3/A4b/A4c=1352px. A2 termination40001=507px. A4a output4->5 przyVS4=0px. A5 headerbit9 bez dodatkowego payload=TAHWR. Serie oceniane po kick3 były nieważne: kick3 ma inny VDM i clear. Patch/odczyt tylko kick2. Te wyniki zastępują wcześniejsze offline priorytety podejrzeń; A5 NIE dowodzi błędu własnego PPP25, który ma dodatkowe słowo.

## Hybryda offline
D:/y5_linux_work/scratchpad/phase2_codex/prepare_hybrid.py generuje nativeVS176+ownPDS w zasobach stock replay. Zarezerwowane zerowe okno USC: ba00035000+800, checked input bytes. PDSdataae000/codeae010, executionUSCoffset35800. VDMoutputs5 i PPPoutputselect05040000; stock PPP24, stock FS, termination i pozostałe parametry zostają. To jedna jednostka kontraktu VS/PDS, NIE pojedyncza zmiana bitu. Bez pełnego decode nadal możeHWR. Kandydat nie został wysłany ani wykonany. Nie oczekiwać1352px/stockkoloru od innej geometrii nativeVS. Replayer kopia graphics19_replay_hybrid.c podnosi limit patch32->512 i przerywa przedkick przy patchwritefailure. Canonical graphics19_replay.c niezmieniony.

Przed pierwszym testem: odzyskać normalną inicjalizację GPU, firmwarestage, kontrolaA0 na świeżym stanie. Po pierwszym nowymHWR zatrzymać serię. Nie uruchamiać drugiego agenta przy telefonie. Bionic/HAL nie jest docelowym stosem; native-only pozostaje celem.
