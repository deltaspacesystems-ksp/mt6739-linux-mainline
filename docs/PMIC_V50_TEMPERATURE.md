# v50: temperatura baterii; rozpoznanie ładowarki

Stan: noc 24/25 października 2026. v49 nadal jest obrazem potwierdzonym na telefonie; v50 gotowy do ręcznego testu przez właściciela.

## Temperatura: dane i pomiar
- Stockowy pmic_get_v_bat_temp wywołuje ADC descriptor nr2. Z tabeli 4.4: 12 bitów, skala x1, kanał3; request ID0x6ad = rejestr0x110e, bit3; ready ID0x63a = 0x108e bit15; output ID0x639 = 0x108e mask0xfff.
- BattVoltToTemp: pull-up 16900 omów, reference 1800 mV. BIF weak implementation w tej binarce kończy się mov w0,#0 / ret; dodatkowego opornika BIF 16000 omów tutaj nie odejmować.
- Wyeksportowano 21 par temperatur/resystancji z 0xffffff80094b57f8: -40..60°C, 10k przy25°C. Dane i instrukcje BIF: phone/battery_tables_44.txt; skrypt phone/ghidra/Y5BatteryTables.java; dekompilacja phone/ghidra/k44_thermal_detail.c.
- Na działającym v49 chwilowo odpięto sterownik y5-pmic-monitor (brak równoczesnego dostępu do WACS2), wykonano dwie świeże konwersje i ponownie podpięto sterownik. Oba odczyty raw=1432, napięcie629296uV. R=raw*16900/(4096-raw) ≈9084 omów -> interpolacja27.7°C.
- Ponowne podpięcie potwierdzone: voltage_now3842523uV, input z trzema klawiszami, event0. Nie pozostawiono sterownika odpiętego.
- UWAGA proceduralna: phone_shell.py --file podaje linie interaktywnej powłoce, nie wykonuje pliku przez osobne sh. Pierwszy trap EXIT nie wykonał od razu restore; sterownik podpięto jawnie i sprawdzono. Poprawiony test_thermal_unbound.sh zawiera cały test w podpowłoce (...), aby EXIT uruchomił cleanup. Nie polegać na samym trap EXIT w skrypcie bez podpowłoki.

## Implementacja i testy v50
- y5-pmic-monitor.c obsługuje request1 dla VBAT i request8 dla BATTEMP, z dotychczasowym mutexem i timeoutami; brak resetów ADC/PMIC i zmian regulatorów.
- /sys/class/power_supply/microfw-battery/temp: dziesiąte części stopniaC. voltage_now i trzy przyciski zachowane. Status pozostaje Unknown.
- y5-ntc.h: interpolacja tabeli stockowej, odrzucanie zwarcia/otwarcia oraz wartości spoza zweryfikowanego zakresu. Żadnego zastępowania błędu pozornie poprawną temperaturą.
- Przyjęto skalę z kodu stockowego, ale nie zaimplementowano kompensacji spadku napięcia zależnej od prądu baterii. Wynik jest wstępnym pomiarem diagnostycznym, bez kalibracji z zewnętrznym termometrem; nie uruchomiono na jego podstawie sterowania ładowaniem.
- test_y5_ntc.c wykonany natywnie w WSL z UBSan: wartości znane (1432->277, 1523->250), skrajne/nieprawidłowe odczyty, pełny zakres0..4096, monotoniczność,3151 poprawnych kodów. PASS. Kernel zbudowany; tekst nowej wersji sterownika znaleziony w vmlinux.

## Ładowarka: ważna korekta inwentarza
- Stockowy sterownik nazywa się fan54005, ale obsługuje kilka układów. Nie utożsamiać nazwy sterownika z zamontowanym krzemem.
- Zmierzone pod v49: I2C5 0x11016000, INFRA_STA2=0 (bramki I2C5/I2C5A/I2C5_IMM otwarte), adres klienta w rejestrze0xd4 czyli7-bit0x6a; timing0x0216, divider0x0004, control0x0028. Inicjalizacja z LK działa; bez zmiany zegarów/pinów.
- Odczyt PIO przez FIFO: jeden bajt indeksu rejestru, repeated START, jeden bajt odczytu; control0x3a, transfer_len1, aux_len1, transac_len2. Bez DMA, zapisów danych konfiguracyjnych do IC ani resetu kontrolera. Zachowywane/odtwarzane zmieniane rejestry kontrolera; stare flagi completion są kasowane W1C. Nie jest to pełny sterownik I2C.
- Dwa odczyty ID rejestru3 =0x41. Reg0 kolejno0xf6,0xd0; reg0x10=0xc6. Nie przypisywać jeszcze znaczenia stanom z dokumentacji innego modelu.
- Dekompilacja fan54005_hw_component_detect w stock_Image rozpoznaje ID0x41 i przypisuje nazwę HL7005D (string0xffffff80094b6558). Inne obsługiwane nazwy: BQ24157 i SY6923D1. phone/ghidra/k44_charger_id.c oraz phone/battery_tables_44.txt.
- Wniosek: sztuka identyfikowana przez swój stockowy sterownik jako HL7005D. Inwentarz phone/hw_inventory_73.md poprawiono. Następny etap to sprawdzenie mapy bitów HL7005D/stockowej logiki jego obsługi, następnie kernelowy odczyt statusu i dopiero kontrolowane zarządzanie ładowaniem.
- Porównanie z producentem FAN54005 było pomocnicze: [onsemi FAN54005/D](https://www.onsemi.com/download/data-sheet/pdf/fan54005-d.pdf), tabela27, IC_INFO. Oczekiwany identyfikator tego modelu różni się od0x41. To nie jest dokumentacja HL7005D i nie należy stosować jej parametrów ładowania do tej sztuki bez sprawdzenia.
- Diagnostyka PC: y5_i2c5_snapshot.c i y5_fan_read.c (historyczna nazwa rodziny protokołu). Nie uruchamiać tych narzędzi po przyszłym podpięciu kernelowego sterownika I2C5.

## Obraz i odtwarzanie
- D:\y5_linux_work\wd_v50.img; kernel out/Image_wd24=12828680B; DT out/v12.dtb bez zmian; builder bm_wd50.py; obraz8787968B.
- SHA256:89be48726adf9f5edf8e74630fe629dba4fe279f32134ebf9ca87fbd5b314878.
- verify_wd50.py: PASS payloadów Image/DT, SHA1 nagłówka boot, limitu24MiB, identyczności ramdisk/cmdline/adresów z v49. To nie zastępuje testu rozruchu na urządzeniu.
- Pełny patch phone/kernel/y5-kernel-7.3-v50.patch (15 plików) nakładać na czyste7.3 ZAMIAST poprzednich patchy. Dry-run na czystej bazie PASS. Config jak v49: config-7.3-y5-pmic. Zawsze make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- olddefconfig.
- Kod i skrypty: archive/pc_work/y5-pmic-monitor.c, y5-ntc.h, test_y5_ntc.c, prepare_thermal_kernel.sh, collect_thermal_kernel.sh, export_thermal_patch.sh, bm_wd50.py, verify_wd50.py. Budowanie przez nadzorowaną sesję WSL z nohup bld.sh jak przy v49.

## Test przez właściciela — oczekujący
```powershell
cd D:\mtkclient_work
python mtk.py w boot D:\y5_linux_work\wd_v50.img
```
Obserwować konsolę/baner jak poprzednio, brak restartów; potem ping/powłoka, temp i voltage_now, oraz krótki test regresji przycisków.
```sh
cat /sys/class/power_supply/microfw-battery/temp
cat /sys/class/power_supply/microfw-battery/voltage_now
```
Wartość277 oznacza27.7°C. Przy braku ping poprosić właściciela o zdjęcie ekranu. Awaryjnie przywrócić boot wd_v49.img.
