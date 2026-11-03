# MT6739: radio i zegary — stan 2026-10-25

Właściciel poza domem: bez restartu i aktywowania nowych bloków sprzętu. Poniższe wyniki pochodzą z lokalnego kernela 7.3, stockowego kodu 4.4 i DT, a nie z udanej inicjalizacji radia. Bieżący system pozostaje v53. Nie uruchomiono nvram_daemon, nie odczytywano ani nie zapisano kalibracji.

## Co mamy i czego brakuje

| Blok | Dowód z tej platformy | Brakująca warstwa |
|---|---|---|
| CONNSYS | DT: mediatek,mt6739-consys, baza 0x18070000; zależności TOPRGU/TOPCKGEN/SPM | Zasilanie, zegary, reset, wydzielona pamięć, firmware oraz transport WMT/STP |
| Wi-Fi | DT: HIF 0x180f0000, zegar wifi-dma; stock stos WMT | Sterownik tego HIF, DMA/IRQ i integracja cfg80211; samo wpa_supplicant nie utworzy urządzenia |
| Bluetooth | DT: BTIF 0x1100c000; stock mtk_btif_probe wywołuje hal_btif_clk_get_and_prepare | Natywny BTIF, STP/HCI i inicjalizacja wspólnego CONNSYS; BlueZ dopiero nad działającym HCI |
| Modem | DT: CLDMA 0x10014000/0x1021b000, CCIF AP 0x10209000 i MD 0x1020a000 | Port CCCI/CLDMA/CCIF, pamięć i handshake firmware, obsługa kanałów FS/RPC, dopiero potem porty AT i transmisja |
| CPU | Działający odczyt około 950 MHz przez mfw_cpu; ARMPLL_CON1=0x811244ec w wcześniejszym pomiarze | Pełna ścieżka zmiany ARMPLL, napięcia VPROC/VSRAM, właściwe OPP i binning |

Lokalne przeszukanie drivers/bluetooth, drivers/net/wireless/mediatek, drivers/net/wwan i drivers/remoteproc w używanym 7.3 nie znalazło dokładnego mt6739. Brak dopasowania tekstowego nie dowodzi braku jakiejkolwiek możliwości adaptacji; dowodzi, że nie znaleźliśmy gotowego dopasowania tej platformy.

btmtkuart.c używa serdev i protokołu STP, lecz jego kompatybilne urządzenia to m.in. MT7622/MT7663/MT7668. Lokalny BTIF nie jest zwykłym UART serdev; nie podmieniamy compatible dla wymuszenia probe. Kod może być materiałem do wydzielenia wspólnych operacji protokołu.

Nowy dowód z k44_cpu_bt.c: hal_btif_clk_get_and_prepare pobiera zegary o nazwach btifc i apdmac i wykonuje clk_prepare obu. Nazwy odczytano z dwóch dokładnych adresów stałych kernela i wyeksportowano do stock-clock-tree.json. Nie jest to jeszcze pełna sekwencja włączenia BT; samo prepare nie dowodzi aktywnego enable ani załadowania firmware.

T7xx jest sterownikiem PCIe T700 5G, więc nie jest zamiennikiem zintegrowanego modemu MT6739 z CCCI. Zgodność nazwy producenta i obecność CLDMA nie wystarczają. Zobacz [oficjalną dokumentację T7xx](https://www.kernel.org/doc/html/v6.12/networking/device_drivers/wwan/t7xx.html).

Stock wmt_drv.ko ma vermagic 4.4.95+ SMP/preempt/modversions/aarch64. Nie można załadować go do 7.3. Znalezione w module nazwy WMT_SOC.cfg i mt66xx_patch_hdr.bin są kandydatami ścieżek firmware, a nie potwierdzeniem właściwego pliku dla tej płyty.

## Kolejność uruchamiania radia

1. Odtworzyć zależności zasilania, resetów i zegarów CONNSYS ze stockowych callerów oraz DT. Oddzielić wspólne zasilanie Wi-Fi/BT od transportu konkretnej funkcji.
2. Porównać stock reserved-memory/EMI, ATAG i live DT z pamięcią faktycznie wyłączoną z alokatora 7.3. DMA modemu/CONNSYS nie może trafić w RAM kernela ani użytkownika. Sam węzeł chosen z adresem nie dowodzi rezerwacji.
3. Ustalić dokładną wersję firmware i ABI; do repo trafiają opis i publiczne źródła, nie prywatna kalibracja ani zrzuty partycji. Udokumentować pochodzenie i warunki dystrybucji przed kopiowaniem firmware.
4. Przy właścicielu: ograniczona inicjalizacja wspólnego bloku z możliwością odzyskania telefonu; najpierw odczyt ID i kontrola timeoutów, bez automatycznego startu transmisji.
5. Bluetooth: BTIF/STP i pierwszy poprawny HCI reset/wersja. Wi-Fi: HIF/IRQ/DMA i rejestracja wiphy, następnie skan. Obie funkcje wymagają niezależnych dowodów, że poprawnie wyłączają sprzęt po błędzie.
6. Modem: osobny etap po audycie pamięci, firmware i zweryfikowanej prywatnej kopii kalibracji. Historyczne HS2_FAIL oraz fałszywe ServiceNVRAMReady są tropem, nie potwierdzoną diagnozą. Bez nvram_daemon i zapisów NVRAM przed spełnieniem warunku właściciela.

## Drzewo zegarów — nowy wynik offline

Źródła dowodu: phone/ghidra/k44_clock_tree.c, k44_clock_detail.c oraz phone/hardware/stock-clock-tree.json. Powtarzalny ekstraktor: archive/pc_work/extract_stock_clock_tree.py; wejściem jest D:\y5_linux_work\out\stock_Image, nigdy kopia kalibracji.

mtk_apmixed_init tworzy 7 slotów i rejestruje 6 PLL. Zweryfikowany stride deskryptora wynosi 112 bajtów. Z callera mtk_clk_register_plls potwierdzono offsety rejestrów; pozostałe słowa pozostawiono jawnie jako raw_words.

| ID | PLL | CON0 | PCW |
|---|---|---|---|
| 1 | mainpll | 0x220 | 0x224 |
| 2 | mfgpll | 0x240 | 0x244 |
| 3 | mmpll | 0x270 | 0x274 |
| 4 | univpll | 0x230 | 0x234 |
| 5 | msdcpll | 0x250 | 0x254 |
| 6 | apll1 | 0x2a0 | 0x2a4 |

To offsety wewnątrz mapowania apmixed, nie absolutne adresy i nie zalecenia zapisów. ARMPLL nie występuje w tej tablicy; potrzebna osobna analiza CPU/FHCTL. Nie oznacza to, że ARMPLL nie istnieje.

TOP ma 67 slotów, fixed clock f_f26m_ck=26 MHz z rodzicem clk26m, 40 fixed-factor PDN (stride40) oraz 24 mux (stride48). INFRA: 80 slotów/79 bramek. Ekstraktor sprawdza unikalność ID/nazw PLL, ID dzielników, granice adresów, poprawność nazw i niezerowe współczynniki; eksportuje wszystkie 40 relacji rodzic/mult/div. Dwa pola PDN pozostały raw do sprawdzenia ich helpera. Nie przenosimy inicjalizacyjnych masowych zapisów stockowych do bootstrapu: mogłyby odciąć działające USB/eMMC/ekran.

Następny krok implementacji: odtworzyć pełne parent arrays muxów i semantykę SET/CLR/UPDATE, powiązać z istniejącymi mainline helperami CCF oraz ID z DT. Potem provider utrzymujący dotychczasowe częstotliwości i dopiero kontrolowana zmiana pojedynczej bramki przy właścicielu. CPU DVFS później: napięcia i dozwolone OPP muszą być zweryfikowane przed pierwszą zmianą szybkości. Obecny read-only provider nie nadaje się do bezpiecznego podłączenia cpufreq dokonującego zmian napięcia.

Uzupełnienie tego etapu: pełne parent arrays wszystkich 24 muxów już odtworzone. Caller mtk_clk_register_mux_clr_set_upd potwierdza wskaźnik tablicy rodziców pod +0x10 i liczbę rodziców pod +0x2c; stride48 pochodzi z funkcji rejestrującej całą tablicę. Eksport zachowuje cztery słowa rejestrów i cztery bajty pól jako raw, ponieważ semantyka SET/CLR/UPDATE wymaga jeszcze analizy operacji muxa. _mt_cpufreq_init wybiera tablice na podstawie _mt_cpufreq_get_cpu_level; nie wolno zakładać uniwersalnej tabeli OPP. mt_dfs_armpll przekazuje sterowanie przez tabelę operacji, więc sama ta funkcja nie ujawnia kompletnej sekwencji PLL/FHCTL.

## GPU i wyświetlanie

Stock wskazuje kompilacyjny/fallback BVNC 22.49.21.16, nie potwierdzony odczytem GPU. [Oficjalna lista Mesa PowerVR](https://docs.mesa3d.org/drivers/powervr.html) nie wymienia tego BVNC; podkreśla konieczność zgodności całego BVNC. Nie udajemy zgodności z GE8300 ani innym wariantem. Potrzebne własne dane urządzenia, firmware zgodnego ABI i ścieżka zasilania/zegarów/resetu. Nie włączono jeszcze GPU.

Obecny desktop to C/Wayland, labwc/wlroots/pixman i simpledrm. Kopiowanie do stałego bufora LK pozostaje. Zero-copy wymaga właściwego DRM/DSI/RDMA, współdzielonych DMA-BUF/GEM, zgodnego format/stride/modifier i synchronizacji. Overlay status/home wymaga płaszczyzn lub kompozycji GPU. Migotanie zgłoszone przez właściciela jest osobnym nierozwiązanym problemem; wyłączenie 25 Hz czeka na ocenę po jego powrocie.


## 2026-10-26 — live v61 radio memory audit, no restart
Read-only live DT, early reserved-memory log and /proc/iomem agree: CONNSYS has a dynamically allocated 2 MiB no-map reservation at 0xbfe00000; CCCI reservations are 0x6c000000/1 MiB (no-map), 0x5c000000/256 MiB (mapped but reserved), and 0x8e000000/12 MiB (no-map). All four are excluded from the allocator. The CONNSYS DT node has size/alignment 2 MiB and no fixed reg: do not assume this dynamically selected address matches stock EMI/remapping expectations. Reserved memory alone does not establish safe DMA or firmware compatibility. Current netdevs: lo/sit0/usb0; no Bluetooth or remoteproc class. No radio activation, NVRAM access or reboot. Evidence: phone/hardware/radio-memory-v61.json; repeatable read-only script archive/pc_work/radio_reserved_audit.sh. Next: verify stock CONNSYS EMI base/remap and CCCI region usage before implementing power/transport.


## 2026-10-26 — exact stock4.4 CONNSYS/WMT driver analysis
User asked to search4.4driver. Decompiled exact local GPL wmt_drv.ko read-only (not loaded into7.3), relevant26functions incl power/reset/PMIC/EMI/STP-BTIF. Callback returns0x6739. consys_read_reg_from_dts maps4DTresources in order:18070000/200,10007000/100,10000000/2000,10006000/1000. Clocknameconn; DTclockphandle1a/index3; IRQspecifiersSPI233flags8 and235flags2 (not LinuxIRQs).
EMI remap offset1380 inresource2 ->physical10001380. Objdump confirms ubfx #21,#13 then OR old and2000, store+DSB. Native bounded O_RDONLY/PROT_READ AO-register probe measured0, remappingdisabled. ReservedRAMbasebfe00000 from earlieraudit would encode25ff ifold0; calculationonly, NEVER written. Stock MPU caller protectsbase+80000 throughbase+size-1 region16/rawpermission4b6da28; permission semantics unresolved.
Stockpower paths requirevcn18/vcn28/vcn33_bt/vcn33_wifi; commonvcn18 routine also touchesvcn33_bt. pmic_set_register_value selectors are ENUMS, notPMICaddresses. Exactkernel tableVAffffff8008bd8390 stride12 decoded index/addr/mask/shift withbounds+IDchecks: c2f->1a58bit0,c31->1a5abit0,d03->1b1ebit1,d08->1b24bit1,d3b->1b8abit1,d41->1b90bit1. Matched localmainlineMT6357 names VCN18CON0/OP_EN,VCN33OP_EN/OP_CFG,VCN28OP_EN/OP_CFG. No PMICwrites or radioactivation. Ghidrasignatures/indirectcallbacks need instructionverification beforeport. Source/evidence phone/ghidra/wmt_consys.c,k44_pmic_fields.c,phone/hardware/consys-stock-v62.json.
Publicpinned WikoGPLtree contains onlyconnectivityadapters (4files), notWMTimplementation; sameforPower535tree. NewerRedmireferencehas6765/6768platforms, notexact6739; do not copyregistersequences. Independently fetched7MT6739vcodecGPLreferencefiles, allGitblobverified, manifestinphone/hardware/vcodec-mt6739-reference.json. Theyconfirm oldLOCKHW/WAITISR/mmap/controlABI research direction, notLinuxdecode. No protectedpartition/NVRAMsecrets accessed.
