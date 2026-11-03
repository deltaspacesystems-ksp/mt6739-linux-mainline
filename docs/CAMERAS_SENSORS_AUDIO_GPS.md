# Kamery, ALS/proximity, mikrofony i GPS — 2026-10-25

Właściciel rozszerzył zakres o kamery, czujniki światła/zbliżenia, mikrofony i GPS. Zezwolił na sprawdzenie kamer zdjęciem oraz czujników/mikrofonów próbkami, bez jego obserwacji ekranu. Nadal obowiązuje brak restartu podczas jego nieobecności. Pliki zdjęć/nagrań pozostają lokalnie poza Git; nie publikować ich automatycznie. W tej sesji NIE wykonano zdjęcia ani nagrania, bo brak interfejsów sprzętowych.

## Potwierdzony stan bieżącego systemu

Live v53: brak /dev/video*, /dev/media*, /dev/snd, /dev/gnss*, /dev/i2c-* oraz urządzeń IIO. /sys/bus/i2c/devices jest puste. Kernel: MEDIA_SUPPORT, SOUND, IIO i MODULES wyłączone. Samo doinstalowanie aplikacji kamery, arecord, gpsd czy i2c-tools nie uruchomi sprzętu. Przy MODULES=n nie doładujemy sterowników do tego uruchomienia. Użytkowy dostęp przez /dev/mem nie zastępuje jeszcze nieistniejącego toru przechwytywania obrazu/audio i nie został użyty do nowych prób zasilania.

Skrypty inspekcji: archive/pc_work/inspect_capture_sensors.sh, inspect_capture_mainline.sh, inspect_capture_dependencies.sh, inspect_i2c_capture.sh. Live DT wyeksportowano tylko z 9 wskazanych węzłów i whitelisty właściwości do phone/hardware/capture-live-dt.json; pominięto chosen, identyfikatory i kalibracje.

## Światło i zbliżenie — LTR577

Inwentarz z działającego stock 4.4 wskazuje rzeczywiście podpięty ltr577_alsps, I2C-1/0x53. Live DT potwierdza alsps@53/status=okay oraz cust_alsps@0/compatible=mediatek,ltr577, i2c_num=1. ALS pracował w polling, PS z przerwaniem. Równoległy wpis STK3x1x w DT jest alternatywą płytową, nie dowodem drugiego zamontowanego czujnika. SX9320 jest osobnym czujnikiem SAR anteny; nie mylić go z czujnikiem przy uchu.

Caller stockowy w phone/ghidra/k44_camera_als.c potwierdza:

| Rejestr | Odczyt/znaczenie potwierdzone w stock |
|---|---|
| 0x00 | Sterowanie, ALS bit1; włączanie ALS przez OR2, wyłączanie AND~2 |
| 0x06 | Part ID; stock ma szczególną gałąź dla 0xb1, nie wymusza go jako jedynej możliwej rewizji |
| 0x08..0x09 | PS raw little-endian, 11 bitów: low OR ((high AND7)<<8) |
| 0x0a..0x0c | Drugi kanał optyczny używany do korekcji, 24 bity little-endian; bez przypisywania mu skalibrowanej jednostki |
| 0x0d..0x0f | ALS raw, 24 bity little-endian |
| 0x04,0x05 | Stock zapisuje 0x22 i3 do konfiguracji ALS |

Probe zapisuje też rejestry PS/LED: 0x02=0x10,0x01=0x36,0x03=0x5c oraz konfigurację IRQ. Te wartości NIE zostały zastosowane do sprzętu 7.3. Nie włączać emisji IR przez kopiowanie niezweryfikowanych parametrów LED. Domyślne progi DT PS to900/700; stock zmienia je również na podstawie kalibracji. Nie traktować ich jako potwierdzonego progu dla obecnego położenia/szkła telefonu.

Ważna korekta dawnego inwentarza: mainline ltr501.c obsługuje LTR501/559/301/303 z inną mapą (np. ID0x86), nie LTR577 (ID0x06). ltrf216a.c ma część podobnych adresów, lecz obsługuje LTR308/F216A, bez gotowej obsługi PS LTR577. [Źródło mainline LTRF216A](https://linux.googlesource.com/linux/kernel/git/torvalds/linux/+/aaed66fadba2d2de8fe0daa0aa3eac827d2076b9/drivers/iio/light/ltrf216a.c) potwierdza oddzielne warianty układu; sama wspólna magistrala0x53 nie oznacza zgodności.

Przygotowano native C mfw_ltr577_snapshot.c, bez zapisów konfiguracji i skanowania magistrali. Wymaga jawnego --read-only, sprawdza właściciela sterownika, używa I2C_SLAVE bez FORCE i łączonej transakcji I2C_RDWR. Odczytuje tylko ID, sterowanie/konfigurację i surowe kanały; pomija status/IRQ mogące kasować zdarzenia. Akceptuje do interpretacji wyłącznie znaną gałąź ID0xb1; inne rewizje wymagają analizy. Wyjście nie udaje luksów, świeżości próbki ani near/far. Kompilacja NATYWNA Alpine/aarch64 -Wall -Wextra -Werror PASS. Próba wykonania poprawnie kończy się brakiem /dev/i2c-1; brak jakiegokolwiek odczytu z sensora i brak zapisu PMIC/GPIO. Binarka /var/cache/mfw-ui/mfw-ltr577-snapshot.

Kolejność dalsza: właściwy adapter I2C-1 (0x11008000) i jego zegary/pinctrl/zasilanie, potem ID i odczyt stanu, następnie ograniczony start ALS i porównanie kolejnych próbek. PS osobno po sprawdzeniu ustawień emitera, kalibracji i IRQ. Docelowy interfejs IIO z raw/scale lub poprawnym processed lux; automatyczna jasność dopiero po rozróżnieniu pomiaru od kalibracji zależnej od panelu. I2C-1 jest wspólne również dla akcelerometru, SAR i flasha.

## Kamery

Live DT ma ogólne klienty camera_main@43, camera_sub@41 i alternatywne camera_main_w@42/camera_sub_w@40 na I2C-2 (0x11009000). Te nazwy/adresy to vendorowe klienty frameworku; NIE identyfikują sensorów i NIE wolno ich przenosić jako adresów konkretnych sensorów V4L2 bez sprawdzenia. AF0x18 i EEPROM0x50 z wcześniejszego inwentarza nie wystarczają do określenia modelu matrycy. Nie czytano EEPROM/OTP.

SENINF i kd_camera_hw1 wskazują 0x15008000/0x1000. Stock posiada zegary CAMTG/CAMTG2/SENINF/SCAM i stany reset/powerdown/MCLK obu kamer. Pola clock-names i pinctrl-names zachowano w selektywnym eksporcie DT. Nie zmieniano żadnego z nich.

Odtworzono 14 statycznych sekwencji zasilania z tablicy 0xffffff800947ca70, stride248, wpis kroku20B; eksport phone/hardware/camera-power-sequences.json. Są to kompilacyjne WARIANTY, nie lista zamontowanych kamer: HI1333, S5K3L6/MAM, OV13855, HI846, OV8856, GC8034/A, HI556, GC5025A/H/AW, OV5675, S5K5E9YX. Numery pinów i poziomów enum pozostają nierozkodowane: nie zakładamy, że np. enum9 znaczy konkretne napięcie.

Platformowa tablica pod0xffffff800969bb98 leży poza file-backed stock_Image, więc zależy od inicjalizacji runtime; ekstraktor nie udaje, że zawiera statyczne dane. Pierwsza taka próba ekstrakcji została zatrzymana sprawdzeniem granic. Caller power-on idzie naprzód, power-off wstecz. Dekompilacja sugeruje, że ścieżka off korzysta z pola delay+8, a nie ostatniego pola+16; przed zapisami trzeba sprawdzić to w instrukcjach ARM64. W callerze jest też granica wyszukiwania +0xf8 elementów long, podczas gdy cała tablica ma14 rekordów — nie uznawać samej obecności dalszego rekordu za dowód, że dany wariant był osiągalny/wybrany.

W używanym 7.3 istnieją sensory hi556.c,hi846.c,ov8856.c; nie znaleziono nazw pozostałych wariantów w plikach drivers/media/i2c. Brak dopasowania nazwy nie wyklucza przyszłej adaptacji. W drivers/media/platform/mediatek nie znaleziono SENINF/CAMSV ani dokładnego mt6739. Sterownik samej matrycy nie wystarczy do zdjęcia: potrzebny odbiornik CSI-2/SENINF, tor DMA/RAW, clock/power/reset i media graph. Zaczynać od RAW/test pattern po potwierdzeniu ID i poprawnych napięć, ISP/debayer/3A później. Brak sprawnej kamery, zdjęcia lub potwierdzonego modelu w tej sesji.

## Mikrofony / audio

Live DT audio@11220000 ma0x1000B, audio_sram@11221000 ma0x9000B. Mainline ma rzeczywisty kodek sound/soc/codecs/mt6357.c z capture1–2kanały, obsługą ADC, bias i różnych trybów mikrofonu. mt6397-core.c rejestruje komórkę mt6357-sound. To elementy do ponownego użycia, a nie gotowe audio tej płyty.

Potrzebne są: prawidłowy regmap/MFD PMIC zamiast wyłącznego bootstrapu PWRAP, sterownik AFE/ADDA/DMA MT6739, zegary i pinctrl, machine driver/routing oraz określenie typu i połączenia fizycznych mikrofonów. Istniejący machine driver MT8365+MT6357 nie czyni AFE MT6739 zgodnym; nie podmieniamy compatible. Liczba kanałów kodeka nie dowodzi liczby zamontowanych mikrofonów. Nie wykonywano zapisów MICBIAS i nie nagrywano otoczenia.

Pierwszy test po uruchomieniu PCM: jawnie wybrany capturehw, krótki WAV, kontrola liczby rzeczywistych próbek, RMS/DC/clipping i zmiany sygnału, bez automatycznego odsłuchu/publicznego eksportu. Udane otwarcie ALSA lub sam nagłówek WAV nie są dowodem poprawnego mikrofonu.

## GPS

Live DT: ogólny mediatek,gps, bez własnego zasobu GNSS. Stock wmt_drv.ko zawiera mtk_wcn_stpgps_drv_init, do_gps_drv_init, stany gps_lna i również tekst no impl. — symbole/logi nie dowodzą aktywnego transportu. Potrzebna identyfikacja modułu transportu STP/GPS, wspólnego zasilania CONNSYS, firmware i protokołu danych; potem ewentualna integracja GNSS/gpsd. Nie zakładamy, że odbiornik od razu wystawia standardowe NMEA ani portUART.

Historia phone/hw_inventory_73.md mówi o odłączonych antenach; nie zweryfikowano fizycznego stanu dzisiaj. Oddzielne kryteria: wykryty transport/ID, odbierane poprawne komunikaty, widoczne satelity, fix. Brak fixa przy braku anteny/niekorzystnym położeniu nie dowodzi błędnego sterownika. GPS nie aktywowano.

## Przygotowane testy i najbliższy wspólny etap

mfw_capture_test.sh jest na telefonie w /var/cache/mfw-ui oraz w archive/pc_work. Domyślnie tylko jawne --check; --photo wymaga wskazanego istniejącego V4L2, a --microphone jawnego hw:N,M. Pojedyncza klatka jest ograniczona timeout12s, nagranie2s timeout7s. Prywatny katalog rootfs /var/cache/mfw-hardware-private ma0700, pliki umask077. Zrzut RAW wymaga późniejszej interpretacji według formatu; skrypt nie ogłasza sprawności kamery po samych bajtach. Nie jest autostartem. sh -n i --check na telefonie PASS, brak węzłów i brak narzędzi przechwytywania; nie uruchomiono żadnego nagrywania.

Najwięcej odblokuje prawdziwy provider clock/pinctrl/I2C + właściwa warstwa PMIC/regulatorów. V54 dodaje tylko uinput/I2C_CHARDEV; samo jego wgranie nie doda IIO, audio ani toru kamery. Nie wgrywano go podczas nieobecności właściciela. Następny mierzalny cel sprzętowy: adapter I2C-1 i identyfikacja LTR577/MC3XXX, z ograniczonymi testami po powrocie właściciela; równolegle statyczna analiza reset/MCLK i AFE.

## Rozszerzenie: g?o?niki i pe?ne audio
W?a?ciciel doda? g?o?niki i ca?y tor audio: playback g?o?nika multimedialnego, s?uchawki rozm?w, wyj?cia s?uchawkowego, mikrofony i routing/poziomy. Model/liczba fizycznych przetwornik?w, obecno?? wzmacniacza zewn?trznego oraz pin jack-detect pozostaj? do potwierdzenia; nie wyci?ga? ich z samej listy skompilowanych funkcji.
Stock ma Speaker_Amp_Change (ffffff8008977958), Speaker_Amp_Set (ffffff8008977bf4), Ext_Speaker_Amp_Set (ffffff80089767c4), speaker_amp_i2c_probe (ffffff800897fea8) oraz selekcj? speaker_amp_type. To cele kolejnej analizy offline. Nie dowodzi to podpi?tego zewn?trznego wzmacniacza. Odtworzy? bias/DAC/wyj?cia kodeka i kolejno?? mute/power/unmute, dopiero potem kr?tki test przy niskim poziomie; nie uruchamiano d?wi?ku w tej sesji.
GPU nadal nie uruchomiono; stockowy BVNC22.49.21.16 to kandydat kompilacyjny, firmwareABI oraz power/clock/reset niegotowe. Desktop pozostaje labwc/wlroots/pixman+simpledrm, brak akceleracji i zero-copy. Dalsze dowody/plan docs/RADIOS_CLOCKS_BRINGUP.md.


## 2026-10-26 — I2C1 and native light IIO without reboot
Built/loaded two native GPL modules on v62/#48: restricted PIO I2C1 adapter plus stock-c2 raw-light IIO driver. Sensor responds stably at0x53 with ID0xc2, not assumed0xb1. Stock ltr577 non-b1 ALS/PS callers use the same raw register map; exact part remains unknown. InitialCTRL0/rate0x22/gain1. Explicit2-second ALS-only trial CTRL2 returned ALS16/channel0a56, restoredCTRL0; PS/IR never enabled. IIO device mfw-stock-c2-light exposes in_illuminance_raw16 and in_intensity0_channel0a_raw54. Values are RAW, not lux; freshness and controlled response not established. Owner away, cover/lamp test deferred.
PIO transport reserves controller resource, refuses other owner/gated/busy controller, uses20ms bounded poll, preserves timings/config and masks/restores IRQ config. Only allowlisted reads; optional ALS control permits only reg0 values0/2. Any transaction fault latches and pins module until reboot. No PMIC/GPIO/clock/power manipulation. Initial I2C_RDWR attempt rejected before MMIO due i2c-dev-added DMA_SAFE; confirmed kernel source, accepted annotation without enabling DMA, rebuilt/reloaded. Default old snapshot rejectedIDc2 correctly; explicit c2 raw mode added based on stock non-b1 callers.
ARM64W=1/-WerrorPASS; ASan/UBSan exact allowlist1280read+65536write cases and malformed messages; exact IIO callback restoration/failure/mutex casesPASS. Own module O taint expected. Modules installed extra/,depmodPASS, explicit /usr/local/bin/mfw-light-start tested; no autostart. GPU firmware and PMR_CPU_MAP_PASS repeated successfully after kexec, HWR/CRR0. Source phone/kernel/sensors, evidence phone/hardware/light-iio-v62.json. No further reboot/kexec performed.


## 2026-10-26 — optical physical tests PASS; Mesa ABI stage2 saved
Owner present. Initial45s shortIIO150ms samples stayedALS19, unclearresponse. ContinuousALS45s nativecapture180samples reacted tocover0 andlamp29897, CTRL0restored. Thus hardwarelightresponsePASS; shortreadfreshness was notproven. IIOwaitincreased500ms andinstalled, separatecontrolledIIOreteststillpending.
IR: newoptin kernelcallback savesreg0..3, usesexactstock36/10/5c PS config, enablesPS50samples100ms, restores/readbackchecksall4regs. Far14..23median18; ownerhand1-3cm182..268median235.5; bothresult0/restored1. Kernelcallbackrunsrestorationevenifwriterprocessends; faultlatchallowsoneboundedinternalPSoffattempt butnofurtherblindconfigretry. ARM64W1/Werror+ASan/UBSan67faultpositions/busy/optin/offtestsPASS, read/writeallowlisttestsPASS. NoIRQthreshold/NVRAMwrites. ReloadedmodulewithIRgateN, restoredIIOlight. mfw-hw-statusI2Ccountfixedtobusdevicespath. Evidencephone/hardware/optical-physical-tests-v62.json; no reboot.
Mesa stage2: contextcreate/destroy, heapcount30->28/ordertranslation, heapcreate/destroy. Sevenopstotal. GeneratederrorcodesRETRY25/BRIDGE37confirmed; fullhostlibrary+hostASan/UBSan+nativephoneactualDRMsmokePASS17heaps/cacheline64,FWHWR/CRR0. StillnoRendering. Patchphone/mesa/mesa-ddk19-stage2-on-stage1.patch exactreplayPASS.
