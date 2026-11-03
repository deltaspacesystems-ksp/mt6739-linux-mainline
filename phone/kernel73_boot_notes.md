# Kernel 7.3 na Y5 (MT6739): dlaczego "booting now" -> restart po ~20 s

Objaw: po potwierdzeniu bootu LK wypisuje "booting now", po ~20 s telefon sie restartuje.
Najbardziej prawdopodobna przyczyna: **sprzetowy watchdog (TOPRGU)** uzbrojony przez LK, ktorego kernel 7.3 nie przejmuje,
wiec SoC resetuje sie po jednym okresie. Kernel 4.4 ma wlasny watek `wdtk`, ktory karmi watchdog co ~20 s.

Drugi podejrzany: kernel uzywa pamieci zarezerwowanej dla ATF/TEE/modemu (patrz nizej), co potrafi powiesic/zresetowac SoC.

## Dane odczytane z dzialajacego telefonu (kernel 4.4, /proc/device-tree, /proc/config.gz, FBIOGET_*)
- Watchdog: `toprgu@10007000`, reg = <0x0 0x10007000 0x0 0x1000>, compatible w vendorze "mediatek,toprgu".
- UART: `apuart0@11002000` (console=ttyMT0,921600n1, ale LK dodaje `printk.disable_uart=1`), `apuart1@11003000`.
- Framebuffer LK: fizyczny 0x771e0000, rezerwacja 0x00e20000, tryb 720x1440, wirtualny 736x4320, 32 bpp, stride 2944 B (=736*4).
- RAM 2 GB od 0x40000000 do 0xc0000000.
- cmdline z LK: `bootopt=64S3,32N2,64N2 ... androidboot.verifiedbootstate=red ... console=tty0 console=ttyMT0,921600n1 root=/dev/ram`.
- pstore/ram_console zarezerwowane w DT (0x44400000 / 0x44410000), ale zawartosc `console-ramoops` po resecie jest
  nieczytelna (DRAM nie przezywa tego resetu) - nie licz na log po awarii bez ekranu/UART.

## 1) Watchdog (najwazniejsze)
Konfiguracja kernela (wbudowane, nie modul):
    CONFIG_WATCHDOG=y
    CONFIG_WATCHDOG_HANDLE_BOOT_ENABLED=y
    CONFIG_MEDIATEK_WATCHDOG=y
DTS (wezel na poziomie soc):
    watchdog: watchdog@10007000 {
        compatible = "mediatek,mt6589-wdt";        /* sprawdz, czy w drzewie 7.3 jest tez "mediatek,mt6735-wdt" i uzyj go jako pierwszego */
        reg = <0 0x10007000 0 0x1000>;
        timeout-sec = <30>;
        #reset-cells = <1>;
    };
Jesli watchdog jest juz uzbrojony przez LK, a sterownik go znajdzie, jadro karmi go samo az do otwarcia /dev/watchdog w userspace
(dzieki HANDLE_BOOT_ENABLED). Dopisz tez w init (ramdisk) proces/skrypt otwierajacy /dev/watchdog i karmiacy go, jesli bedzie dzialal dlugo.

## 2) Zarezerwowana pamiec (nie dotykac) - fragment DTS
    reserved-memory {
        #address-cells = <2>;
        #size-cells = <2>;
        ranges;

        atf@44600000      { reg = <0 0x44600000 0 0x00040000>; no-map; };   /* mblock-3-atf-reserved */
        ccci_a@5c000000   { reg = <0 0x5c000000 0 0x10000000>; no-map; };   /* mblock-10-ccci */
        ccci_b@6c000000   { reg = <0 0x6c000000 0 0x00100000>; no-map; };   /* mblock-9-ccci  */
        ccci_c@8e000000   { reg = <0 0x8e000000 0 0x00c00000>; no-map; };   /* mblock-11-ccci */
        teei@70000000     { reg = <0 0x70000000 0 0x03c40000>; no-map; };   /* mblock-4-teei-reserved */
        mcupm@771c0000    { reg = <0 0x771c0000 0 0x00010000>; no-map; };   /* mblock-8 */
        spm@771d0000      { reg = <0 0x771d0000 0 0x00010000>; no-map; };   /* mblock-7 */
        fb@771e0000       { reg = <0 0x771e0000 0 0x00e20000>; no-map; };   /* mblock-6-framebuffer (LK) */
        logstore@7ff7f000 { reg = <0 0x7ff7f000 0 0x00080000>; no-map; };   /* mblock-2-log_store */
        emi@7ffff000      { reg = <0 0x7ffff000 0 0x00001000>; no-map; };   /* mblock-1-emi-reserved */
        hwboot@80fff000   { reg = <0 0x80fff000 0 0x00001000>; no-map; };   /* mblock-5-hw_boot */
        ramcons@44400000  { reg = <0 0x44400000 0 0x00010000>; no-map; };
        pstore@44410000   { reg = <0 0x44410000 0 0x000e0000>; no-map; };
        minirdump@444f0000 { reg = <0 0x444f0000 0 0x00010000>; no-map; };
    };
(consys-reserve-memory ma wlasne, w 4.4 puste reg - zostaw na razie.) Pamiec do uzycia przez kernel: 0x40000000-0xc0000000 poza powyzszymi.

## 3) Widocznosc bez UART: log na ekranie przez simplefb + fbcon
    chosen { stdout-path = "serial0:921600n8"; bootargs = "console=tty0 ignore_loglevel earlycon=simplefb ..."; };   // bootargs zwykle dodaje LK
    framebuffer@771e0000 {
        compatible = "simple-framebuffer";
        reg = <0 0x771e0000 0 0x00c21000>;
        width = <720>; height = <1440>; stride = <2944>;
        format = "a8r8g8b8";                    /* jesli kolory sa odwrocone, sprobuj "a8b8g8r8" lub "x8r8g8b8" */
        status = "okay";
    };
Konfiguracja: CONFIG_FB_SIMPLE=y (albo DRM_SIMPLEDRM=y), CONFIG_FRAMEBUFFER_CONSOLE=y, CONFIG_VT=y, CONFIG_VT_CONSOLE=y, CONFIG_LOGO=n.
Nagraj film z ekranu (telefonem) podczas bootu: widac wtedy, czy kernel w ogole rusza, i gdzie sie zatrzymuje.

## 4) Jak odroznic "kernel nie wystartowal" od "kernel dziala, ale watchdog resetuje" (bez zadnych zmian w DTS)
- USB: masz w DTS MUSB gadget/RNDIS. Podczas tych 20 s patrz w Menedzer urzadzen na PC albo `dmesg -w` w WSL
  (usbipd) - pojawienie sie urzadzenia = kernel dziala (wtedy to prawie na pewno watchdog).
- Brak USB i brak obrazu = LK nie przekazuje sterowania (zly format obrazu boot/naglowek MTK/offsety kernel-ramdisk-dtb) -
  sprawdz tekst LK po "booting now" i naglowek obrazu (MTK wymaga wlasnego naglowka 512 B przed kernelem i dtb w poprawnym miejscu).

## 5) Kolejnosc proponowana
1. Dodaj watchdog (pkt 1) + rezerwacje pamieci (pkt 2) + simplefb (pkt 3), zbuduj nowy mainline_test.img.
2. Przed flashem: zrobiony backup (backup_phone_live.py) oraz pelny backup mtkclient w BROM.
3. Flash tylko partycji boot (jak `flashpart.py ... boot`); odzysk: BROM + mtkclient z `D:\huawei_y5_backup`.

## USB z zywego 4.4.95 (2026-10-23)
- DT `usb0@11200000`, compatible `mediatek,mt6739-usb20`; reg: MAC 0x11200000 (64K) + PHY 0x11cc0000 (64K); IRQ SPI 73 (0x49), flaga 8 (level low).
- clock-names: `usb0`, `usb0_clk_top_sel`, `usb0_clk_univpll3_d4`; clocks = <&infracfg 6> (infra_icusb), <&topckgen 0x3c> (usb_top_sel), <&topckgen 0x1e> (univpll3_d4).
- clk_summary: usb_top_sel 62.4 MHz (enable 1), infra_icusb 136.5 MHz (enable 1).
- MUSB regdump: HWVers e503, ConfigData 1f, EPInfo 88, RAMInfo 8b, DevCtl 99, Power 71 (tryb urzadzenia, UDC `musb-hdrc`).
- PHY debugfs (usb20_phy): HSTX_SRCTRL 100, TERM_VREF_SEL 101, VRT_VREF_SEL 101, PHY_REV6 11 (bin), driving cap 10.
- Wniosek: 4.4 uzywa wlasnego sterownika (nie generic-tphy); dla 7.3 trzeba PHY przez rejestry 0x11cc0000 + wlaczenie infra_icusb i usb_top_sel.
