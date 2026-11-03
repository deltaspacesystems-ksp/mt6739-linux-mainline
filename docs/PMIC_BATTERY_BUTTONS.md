> Aktualizacja 2026-10-24: wd_v49.img przeszedł test na urządzeniu i jest bieżącym potwierdzonym obrazem. Szczegóły: docs/PMIC_V49_TEST.md. v48 pozostaje awaryjny.

# PMIC, bateria i przyciski — rozpoznanie lokalne 2026-10-24

Priorytet właściciela: PMIC, bateria, ładowanie i przyciski. SSH i dotyk odłożone.

## Źródła i fakty
- Stockowy DT: D:\y5_linux_work\out\stock_0.dts. PWRAP pod 0x1000d000, IRQ <0 175 4>; keypad pod 0x10010000, IRQ <0 176 2>. Nie zmieniać polaryzacji na podstawie innych urządzeń: te wartości pochodzą ze stockowego DT.
- DT zawiera gauge MT6357; identyfikator układu nie został jeszcze odczytany na 7.3.
- phone/hw_inventory_73.md dokumentuje aktywny FAN54005 na i2c-5 0x6a na 4.4. Węzeł RT9458 w DT nie dowodzi jego obecności.
- Kernel /home/y5/linux-7.3 ma obsługę MT6357 w drivers/mfd/mt6397-core.c, mt6358-irq.c, drivers/input/keyboard/mtk-pmic-keys.c i drivers/iio/adc/mt6359-auxadc.c.
- MTK_PMIC_WRAP oraz IIO są wyłączone w aktualnej .config. Host MT6739 nie występuje w tabeli dopasowań mtk-pmic-wrap.c; nie podstawiać innego SoC bez sprawdzenia rejestrów.

## Pierwszy wynik Ghidry
Projekt D:\y5_linux_work\ghidra_k44, k44/stock_Image, analiza readOnly/noanalysis. Wynik out/k44_pmic_codex.c, funkcja pwrap_wacs2_hal (stock 4.4).
- Baza wrappera jest pobierana ze zmiennej globalnej; DT wskazuje 0x1000d000.
- WACS2_CMD: offset 0xc20; WACS2_RDATA: 0xc24; WACS2_VLDCLR: 0xc28.
- Pole FSM: (RDATA >> 16) & 7; IDLE=0, WFVLDCLR=6; INIT_DONE bit 21.
- Komenda: write_bit << 31 | (pmic_address >> 1) << 16 | data16. Wynik odczytu: RDATA & 0xffff; potwierdzenie odbioru VLDCLR=1.
- To dekompilacja kodu stockowego, nie wynik nowego testu rejestrów na sprzęcie. Kod stockowy zawiera pętle bez limitu; ewentualna diagnostyka musi mieć timeout.

## Kolejność wdrożenia
1. Potwierdzić mapę hosta PWRAP i konfigurację pozostawioną przez bootloader, odczytać ID PMIC.
2. Przygotować obsługę transportu MT6739; na początku zachować ustawienia zasilania odziedziczone po LK.
3. Uruchomić MFD/IRQ i zdarzenia przycisków; ustalić osobno powiązanie fizycznych klawiszy z PMIC i keypad.
4. Uruchomić AUXADC: napięcie i temperatura baterii, z udokumentowaną skalą i przeliczeniem termistora. Sam ADC nie daje wiarygodnego procentu naładowania.
5. Osobno obsłużyć FAN54005, po potwierdzeniu magistrali i rejestrów. Nie dobierać prądów ani napięcia końcowego na podstawie domysłów.
6. Nowy obraz boot testuje właściciel przez BROM; dotąd nie przygotowano nowego obrazu ani nie zmieniono telefonu.
## Wyniki pomiarów i kandydat v49 (2026-10-24, Codex lokalnie)

### Zmierzone na działającym v48
- WACS2_RDATA początkowo 0x8070000e: INIT_DONE=1, FSM=0.
- HWCID[0x8] i SWCID[0xa] = 0x5721. PMIC MT6357 potwierdzony na urządzeniu.
- Świeża konwersja: ADC_RQST0[0x110e]=1, odczekanie 2 ms, odczyt gotowego ADC0[0x1088], ADC_RQST0=0. Przed startem wymagane RQST0=0. Zapisano wyłącznie żądanie konwersji, bez resetu ADC/PMIC i bez ustawień regulatorów/ładowania.
- Wynik: raw=23507, napięcie 3873834 uV. Skala raw * 3 * 1800000 / 32768, zgodna z opisem MT6357 w lokalnym mainline i skalowaniem w stock 4.4. Nie wyznaczano procentu baterii ani stanu ładowania.
- Właściciel potwierdził: jeden Power i dwa razy głośność +. Zapis: TOPSTATUS 0x000e -> 0x000c -> 0x000e, potem dwukrotnie 0x000e -> 0x0006 -> 0x000e. Power = aktywne zero bit1, VolumeUp = aktywne zero bit3.
- Dwa potwierdzone naciśnięcia VolumeDown: KPD_MEM = ffff ffff ffff ffff 00ff -> fdfe f7fb dfef 7fbf 00ff -> stan spoczynkowy, dwukrotnie. KPD_MEM0 pod 0x10010004, aktywne zero bit0. LK pozostawia powielenie tego samego sygnału co 9 bitów w macierzy; zgłaszać tylko jeden klawisz.

### Implementacja
- Nowy drivers/power/supply/y5-pmic-monitor.c, CONFIG_Y5_PMIC_MONITOR=y oraz INPUT_EVDEV=y.
- To tymczasowy sterownik płytki zachowujący inicjalizację LK, nie pełny sterownik hosta PWRAP ani MFD/IRQ. Nie włącza sterownika regulatorów i nie rekonfiguruje ładowarki.
- Mutex serializuje WACS2; timeout transakcji 20 ms, konwersji 100 ms; wejścia próbkowane co 30 ms, wymagane dwa zgodne odczyty. Brak procedur suspend/wakeup.
- /sys/class/power_supply/microfw-battery/voltage_now: świeży odczyt uV; status = Unknown. Celowo brak capacity/temp/current_now, bo nie są jeszcze zweryfikowane.
- Urządzenie input zgłasza KEY_POWER, KEY_VOLUMEUP, KEY_VOLUMEDOWN. Samo naciśnięcie Power nie wyłącza telefonu: potrzebny przyszły odbiorca zdarzeń w userlandzie.
- DT v12 zmienia compatible istniejącego PWRAP i dodaje drugi zasób MMIO keypad; stary węzeł kp jest wyłączony, aby dwa sterowniki nie przejęły tego samego bloku.
- Narzędzie diagnostyczne y5_pmic_probe odmawia bezpośredniego dostępu, gdy węzeł PWRAP ma już sterownik. Nie uruchamiać go po v49; używać sysfs/input.

### Obraz i odtwarzanie
- Kandydat do testu: D:\y5_linux_work\wd_v49.img. Nadal zalecany i potwierdzony: wd_v48.img, dopóki właściciel nie przetestuje v49.
- Kernel out/Image_wd23 = 12828680 B; DT out/v12.dtb; builder bm_wd49.py; obraz 8787968 B.
- SHA256 obrazu: 44a28e13ad9da185b745dc193657ceda79ca0290193579db479bb633d8c12c9e.
- Kernel kompiluje się; nm potwierdza y5_pmic_probe i evdev_init. verify_wd49.py potwierdza payload kernela/DT, SHA1 boot, rozmiar <24 MiB oraz identyczność initramfs/cmdline/adresów z v48. To nie jest jeszcze test rozruchu na urządzeniu.
- phone/kernel/y5-kernel-7.3-pmic.patch jest PEŁNYM patchem względem czystego linux-7.3.tar.xz (14 plików), alternatywą dla wcześniejszego patcha. Nie nakładać obu kolejno. Dry-run nowego patcha na czystych źródłach przeszedł.
- Config: phone/kernel/config-7.3-y5-pmic; zawsze make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- olddefconfig.
- DT: do proven out/v11.dts dopisać phone/kernel/y5-pmic-v12.dtsi i skompilować dtc do out/v12.dtb. bm_wd49.py ma domyślne wejścia Image_wd23/v12.dtb i limit partycji 24 MiB.
- Pliki PC wyeksportowano do archive/pc_work, dekompilacje do phone/ghidra/k44_{pwrap_wacs2,pmic_keys,auxadc}.c.

### Ważna pułapka tej sesji
Samo nohup uruchomione z krótkiego procesu wsl.exe nie utrzymało tu procesu budowania po zamknięciu sesji; stary build73f.log kończył się słowem done i mógł mylić. Start-Process z krótkiej sesji narzędzia też nie utrzymał procesu. Działa uruchomienie supervise_pmic_build.sh w długiej sesji exec: skrypt robi nohup bash bld.sh, ale proces nadrzędny czeka przez wait. Narzędzie zwraca session_id i pozwala kontynuować pracę. Sprawdzać mtime Image/logu i symbole nowej funkcji, nie samo done. Log nadal /home/y5/build73f.log.

### Test przez właściciela (jeszcze niewykonany)
```powershell
cd D:\mtkclient_work
python mtk.py w boot D:\y5_linux_work\wd_v49.img
```
Obserwować: konsola i baner jak v48, brak pętli restartu; po starcie RNDIS/ping/powłoka.
Po odzyskaniu powłoki odczytać:
```sh
uname -a
cat /sys/class/power_supply/microfw-battery/voltage_now
cat /sys/class/power_supply/microfw-battery/status
cat /proc/bus/input/devices
dmesg | grep -E 'y5-pmic|MT6357|keys:'
```
Następnie po jednym krótkim naciśnięciu każdego klawisza, sprawdzić zdarzenia evdev i log, a także zgodność kilku świeżych pomiarów napięcia. Gdy ping nie wraca, poprosić o zdjęcie ekranu. Awaryjnie właściciel przywraca boot wd_v48.img.

## Przygotowanie kolejnego etapu: temperatura/FAN54005 (tylko analiza 4.4)
- Nowa dekompilacja phone/ghidra/k44_battery_thermal.c. BattVoltToTemp używa stałych 0x4204 (16900 omów) i 0x708 (1800 mV) w przeliczeniu dzielnika; przy obecności BIF jest dodatkowe odejmowanie 16000 omów. To kod stockowego kernela, nie potwierdzenie aktywnego toru na tej sztuce. Nie dodawać temperatury do sysfs przed sprawdzeniem toru, tabeli termistora i skali ADC.
- BattThermistorConverTemp odwołuje się do tabeli w stock_Image pod 0xffffff80094b57f8. W następnym etapie wyeksportować wartości i sprawdzić zakres/monotoniczność, nie zastępować dowolną tabelą NTC z Internetu.
- fan54005_get_chip_status odczytuje rejestr0, maska3, shift4. driver_probe ustawia adres klienta 0x6a i wykonuje zapisy m.in. rejestru6; NIE kopiować całego probe jako testu odczytu. Najpierw uruchomić magistralę i odczytać identyfikator/status; wartości programowania ładowania wymagają oddzielnego sprawdzenia.
- W tej turze nie dodano temperatury ani sterowania ładowaniem, nie zmieniono obrazu v49 i nie wykonano zapisów PMIC/ładowarki poza dotychczasowymi konwersjami voltage_now.

Identyfikacja ładowarki skorygowana po rzeczywistym odczycie: ID0x41 stock mapuje do HL7005D. Nowe pomiary NTC i kandydat v50: docs/PMIC_V50_TEMPERATURE.md.
