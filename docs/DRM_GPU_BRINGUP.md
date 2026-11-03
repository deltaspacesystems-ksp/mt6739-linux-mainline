# MT6739: właściwy scanout DRM i GPU — 2026-10-25

Nowy priorytet właściciela: rozpocząć rzeczywisty sterownik GPU i zero-copy display. Obecny simpledrm/pixman i y5disp NIE są końcowym torem. Potwierdzony fizyczny dotyk MStar opisano w MFW_V54_TOUCH_BRINGUP.md; obraz przy interakcji nadal migocze/rozrywa się i pozostają stare fragmenty.

## Nowe pomiary rzeczywistego toru

Read-only /dev/mem w y5_display_regs_read.c, natywnie zbudowany -Wall/-Wextra/-Werror. Dostęp wyłącznie do działających bloków wyświetlania. Bez zapisów, IRQack, FIFO czy rejestrów GPU. Definicje porównano z używanym Linux7.3 mtk_disp_rdma.c/mtk_disp_ovl.c/mtk_dsi.c, a adresowanie dodatkowo ze stock4.4.

| Blok | Pomiar |
|---|---|
| OVL0 baza | 0x1400a000 |
| OVL SRC_CON | 0x00000008: włączona warstwa3 |
| OVL L3 adres | offset0xfa0 = **0x775eb000** |
| OVL L3 pitch | offset0xa4 = **2880** |
| OVL L3 rozmiar | 0x05a002d0 =720x1440 |
| OVL L3 con | 0x000021ff,offset0 |
| OVL L0 | wyłączona, adres771e0000,pitch2944; nie używać jej jako obecnego framebuffer |
| RDMA0 | baza1400b000,GLOBAL_CON101,bitMODE_MEMORY1=0 |
| RDMA memory | MEM_CON0,pitch0,start_addr0 — **direct-link**, nie DMA z własnego bufora RAM |
| DSI0 | baza14012000,MODE_CTRL przy+14 ma1 (VIDEO sync-pulse),INTSTA800057b0 |
| DSI próbka1s |1000/1000 odczytów zbit31=BUSY,zero zmian statusu; to próbka w bieżącym stanie, nie uniwersalny czas transferu |
| MUTEX | rzeczywisty blok14001000; legacy bootstrap błędnie celuje14015000 |

RDMA INT_STATUSa6 nie ma bitu FIFO_UNDERFLOW4 według definicji mainline; nie nazywać go automatycznie underrunem. Część innych bitów/stickyflag wymaga stockowej interpretacji, bez kasowania podczas diagnostyki.

Stock ddp_ovl_get_cur_addr i ovl_get_address potwierdzają OVL_ADDR=f40+layer*20 i wybór aktywnej warstwy przez SRC_CON. Ważna korekta roboczej interpretacji: offset10 to CON_CTRL, a NIE MODE_CTRL; MODE_CTRL jest pod14 i ma1, czyli VIDEO sync-pulse. Stock dsi_wait_not_busy najpierw sprawdza MODE_CTRL&3 i dla video od razu wraca! Dopiero w commandmode sprawdza INTSTA bit31/czeka na zdarzenie. StałyBUSY przy ciągłymvideo nie dowodzi zawieszenia. Nie skopiowano procedury resetu do działającego systemu. Nie implementować commita wymagającego BUSY0 dla tego video. Następny test: czy panel odczytuje zmiany bufora RAM sam, bez ręcznego restartowania DSI; jeśli tak, y5disp_restart per commit jest podejrzany jako źródło artefaktów.

Tabela49modułów stock4.4 została wyeksportowana do display-modules.json: potwierdza OVL01400a000,RDMA01400b000,DSI014012000 oraz mutex14001000. IRQ w deskryptorach mają początkowo0 i są uzupełniane runtime; nie traktować ich jako SPI0. Rzeczywisty mutex14001000/EN20=1,RST28=0,MOD2c=41,SOF30=17ec0; legacy14015020=0. Wcześniejsza hipoteza o poprawnym mutexie pod14015000 była błędna. To dodatkowy argument za usunięciem manualnego bootstrapa w pracującymvideo, a nie przepisywaniem jego adresów do nowegoDRM. OVL/RDMA DT podają SPI201/202 level-low, ale numerację/polaryzację i obsługę sysirq należy zweryfikować przed request_irq.

## Znaleziony błąd adresu framebuffer i v55

V54 simplefb nadal opisuje775e8e38, ale włączona warstwa OVL3 wskazuje775eb000. Delta8648B=3wiersze+8B. Oznacza przesunięte źródło obrazu oraz niedopisywane końcowe8648B aktywnego scanoutu. To konkretny błąd, ale nie dowód wyłącznej przyczyny flickeru/starych okien.

v55 poprawia reg i unit-address węzła simple-framebuffer do775eb000, zachowując size3f4800,width720,height1440,stride2880 i format. Cały region775eb000..779df800 mieści się w zmierzonym reserved771c0000..77ffffff. NIE dodano ani nie zmieniono rezerwacji RAM/chronionych partycji.

Ten sam kernel#40 Image_mfw_v54, initramfs/cmdline/adresy boot identyczne. Verifier porównuje semantycznie wszystkie właściwości obuDT po normalizacji nazwy węzła i dopuszcza tylko jedną różnicę:regframebuffer. Image8802304B,SHA256f43c7b64b5492400e3bbd631910bf08fa2d9cf67a11c82fa10e90f33d3ee817c. Staging/backup PASS, właściciel zatwierdził zapis/restart. Wynik startu i obserwacji będzie dopisany po teście; nie nazywać kandydata potwierdzonym przed odczytem liveDT/OVL po starcie.

## Pierwszy komponent nowego sterownika — napisany i testowany

phone/kernel/display/mfw-ovl-scanout.h to wspólny komponent walidacji dla przyszłego atomic_check, NIE zarejestrowany sterownik KMS i NIE aktywne zero-copy. Oblicza rejestry adresu/pitch/rozmiaru odpowiedniej warstwy OVL, ogranicza pierwszą wersję do720x1440/XRGB8888/pitch2880, odrzuca za mały obiekt, nieprawidłową warstwę i przekroczenie32bitowego zakresu DMA. Wymaga page-aligned adresów z przypiętego, rzeczywiście ciągłego obiektu GEM DMA; nie udostępnia użytkownikowi ABI z dowolnym fizycznym adresem. Wyrównanie4KiB to ograniczenie pierwszego portu, nie wydedukowany limit sprzętu.

test_scanout.c pod UBSan PASS: zmierzony układ OVL3, warstwy0..3, koniec przestrzeniDMA32, za mały bufor, niewłaściwy pitch/format/rozmiar i nieprawidłowe wyrównanie. Nie programuje rejestrów. To początek kodu do wykorzystania w sterowniku, nie dowód działającego page-flip/GPU.

Kolejne elementy KMS: przydział i import ciągłych GEMDMA/DMA-BUF, przygotowanie fence, własność MMIO/IRQ, spójny latch nowego adresu OVL na ramkę i completion, poprawne vblank/pageflip events oraz bezpieczne zatrzymanie/utrzymanie referencji starego bufora do zakończenia DMA. Dopiero po tym usunąć kopię simpledrm->buforLK. Kontrola KMS i y5disp nie mogą działać równolegle nad tymi samymi rejestrami. Panel na początku może zachować inicjalizację LK, ale trzeba jawnie opisać ten etap jako handoff; pełna obsługa DSI/panelu/zasilania później.

Nawet bez GPU pierwszy etap może usunąć dodatkowy CPUblit do stałego buforaLK: pixman renderuje do bufora scanoutuGEM. Pełny tor GPU->scanout wymaga zgodnych DMA-BUF/formatów/modifierów/synchronizacji. wl_shm aplikacji i kompozycja okien nadal mogą wymagać kopiowania/renderowania; nie obiecywać zerowej liczby kopii dla każdej aplikacji.

## GPU — stan zasilania zmierzony, brak aktywacji

y5_gpu_power_read.c czyta tylko always-onSPM, nigdy rdzeńGPU. PWR_STATUS=PWR_STATUS_2ND=30005f4c. Bit4 MFG=0 w obu; MFG_PWR_CON338=72,CORE0_PWR_CON33c=72. Stock spm_mtcmos_ctrl_mfg potwierdza bit4 i338; core0 używa statusbit31 i33c. Obie domeny są wyłączone. NIE wolno teraz czytać13000000 jako „bezpiecznegoID”: dostęp do wyłączonego bloku może zawiesić AXI.

W4.4 zidentyfikowano sekwencje power/isolation/reset/SRAMack i busprotection0x600000, ale nie wykonywano żadnego zapisu GPU/SPM/PLL/regulatorów. Część handlerów jest stubem (MTKDisablePowerDomain); nie traktować pustego callbacku jako całej sekwencji. Następnie zweryfikować bazy/clocktree i zależności parent/core0, dopiero przygotować mainline genpd/CCF i kontrolowany probe przy właścicielu.

BVNC22.49.21.16 nadal tylko stockowy string/fallback; rzeczywisty ID nieodczytany. Stary moduł kernela4.4 nie załaduje się do7.3; wersja DDKrogueddk1.9@4893595 nie dowodzi zgodnego ABI firmware dla nowegoDRM/Mesa. Rzeczywisty blob i jego ABI nie zostały jeszcze porównane z wymaganiami mainline. [Oficjalna lista Mesa PowerVR](https://docs.mesa3d.org/drivers/powervr.html) nie wymienia tego BVNC; wymagana zgodność całego wariantu, cech/errata i firmware. Nie podstawiać ID innego GPU, nie nazywać rejestracji platformdevice akceleracją. Rendering aktualnie nadal software/pixman.

## V55 start i test RAM-only — potwierdzone

Po zgodzie właściciela zapis/readback/reboot v55 PASS, krótki Power naLK. Uname nadal#40 zgodnie z planem. LiveDT /framebuffer@775eb000/reg=0,775eb000,0,3f4800 i OVL3addr775eb000/pitch2880 — zgodność wreszcie potwierdzona po starcie. Guard przy24s:kicks4/errors0/profileok1/temp29.3C. Czas UTC przywrócony zPC.

y5_fb_video_probe.c mapuje rejestry OVL/DSI TYLKO do odczytu, weryfikuje warstwę3/adres/pitch/rozmiar/video1/hz0. Jedynym zapisem jest prostokąt180x60 w RAM bufora podx80/y100; zapisuje kopię fragmentu w pamięci procesu i odtwarza ją po czasie/SIGTERM. ŻADNEGO DRM ioctl, y5disp_trigger ani zapisu rejestru wyświetlania. Pierwszy30s test właściciel przegapił, log potwierdził restore. Drugi90s: właściciel „jest, taki bardziej magenta”. Restore potwierdzony logiem. To dowód, że istniejący tor video pobiera zmiany RAM bez ponownego wyzwalania DSI. Nie dowodzi braku tearingu przy ruchu lub gotowego zero-copyGPU.

## V56 — przygotowany test pomijania manualnego restartu video

Jedyna zmiana źródeł kernela względem poprzedniego patchsetu: y5disp.c. Przy MODE_CTRL&3!=0 nie planuje kicków po atomic_update, nie wyzwala starej sekwencji MUTEX/RDMA/DSI także zmanualtrigger/worker i nie tworzy cyklicznej pętli pracy dla video. Dodano readonly /sys/kernel/y5disp/mode. Legacy fallbackcommand zachowany, ale nie został tutaj przetestowany i zawiera historyczne adresy — przyszły właściwyDRM musi go wyłączyć/zastąpić zamiast współdzielićMMIO.

Build#41 SunOct25 17:24:18CET2026, Image12828680B. Config byte-identicalv54, DTv15/initramfs/cmdline/adresy boot jakv55. Pełny22plikowy mfw-kernel-7.3-v56.patch: dry-run na czystym archiwum7.3 PASS; stary pełny patchv53 z podmienionym wyłącznie nowym plikiemy5disp, nie nakładać ich jeden na drugi. Nowy komunikat continuous video scanout; manual kicks disabled znaleziony wImage. Verifierboot PASS. Obraz8802304B,SHA256cfdf62b447b1078a088fe608da51a4c4e4ab3c046cbfa6e11404252db688c8d2. Staging/backup PASS, na tym checkpointcie jeszcze bez zapisu; pytanie o zgodę wysłane do właściciela.

To test konkretnego błędu prezentacji obrazu, NIE aktywacja nowegoDRM aniGPU. Późniejszy test dotyku użyje czytnika z usuniętym sztucznymUP po5badframes, więc poprawę zachowania okien trzeba rozdzielać od poprawy samego scanoutu.

## Start v56 i rozdzielenie problemu wejścia

Po jawnej zgodzie właściciela zapis/readback/reboot v56 PASS, Power naLK. Live uname#41, /sys/kernel/y5disp/mode=1,hz0, dmesg:continuous video scanout; manual kicks disabled. UTC ponownie ustawiono zPC. Przy49s guard:kicks3/errors4/timer_faults_seen1/last_error0/profileok1/temp28.9C — początkowy timerfault należy zachować w dzienniku, nie ukrywać przez późniejszy rebind. Podczas kolejnego testu rebind zresetował liczniki; przy106s od niego kicks14/errors0/profileok1/temp29.0C. Display/kernel nie zmieniają kodu ładowarki.

Właściciel zamiast oceny flickeru zgłosił „chyba coś dotyk albo sam wayland nie współpracuje bo się zawiesił jakby”. USB działało, labwc czekał wdo_epoll_wait, klientmicrofw wppoll, brak dowodu zawieszenia kompozytora. Brak LAUNCHSettings wlogu; realne ramki dotyku nadal były odbierane. Displaylog ma debug Failed to get DMA-BUF/Failed to import buffer for scan-out — ważne ograniczenie obecnego simpledrm, ale takie próby mogą przechodzić do kompozycji software i same nie dowodzą crasha. Biblioteka wlroots zawiera WLR_SCENE_DISABLE_DIRECT_SCANOUT, jednak w tym teście tej opcji NIE włączano, żeby nie zmieniać dwóch rzeczy naraz.

Test180s skończył sięEXIT0/good412/bad14589 i poprawnym poweroff/cleanup. Wizualna skuteczność v56 nadal NIEPOTWIERDZONA — nie pisać „flicker naprawiony”. Podejrzenie osobnego problemu DOWN/UP: brak pewnego puszczenia palca uniemożliwia ACTION_TAP. Zmieniono przejściowy most na ograniczone odpytywanie danych co20ms niezależnie od próbkiGPIO6 i dodano CONTACT_COUNT,empty/high_good/low_good. To obejście diagnostyczne, nie finalny sterownik IRQ. MStarDT mstar_irq_config=3 jest enumem vendora: stockts_thread mapuje3->IRQF_TRIGGER_FALLING(2), NIE bezpośrednim LinuxIRQ_TYPE_EDGE_BOTH! Nie interpretować surowego3 bez callera.

Nativecompile/selftest czytnika po zmianie PASS. Nowy test desktopu i dotyku uruchomiony bez restartu, pytanie do właściciela dotyczy wyłącznie krótkiego tapnięciaSettings i puszczenia. Wynik następnego testu wymaga dopisania. Z launcheraGUI usunięto stare próby uruchamiania /tmp/y5_fb_snapshot, którego po reboot nie było; nie wpływało to na sam startlabwc (launcher nie miałset-e), ale log błędu nie był poprawnym pomiaremframebuffer.

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
