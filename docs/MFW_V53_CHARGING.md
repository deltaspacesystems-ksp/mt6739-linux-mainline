# MICROFW v53: obsługa HL7005D — kandydat do testu

Stan 2026-10-25: telefon potwierdzony na v52 (#36). Restart z v52 przechodzi sam do LK; krótkie Power na czerwonym ekranie pozostaje konieczne. V53 przygotowany, jeszcze NIE zapisany do boot i NIE przetestowany jako kernel na telefonie.

## Pomiary przed zmianą

Ładowarka I2C5/0x6a, ID 0x41: HL7005D, zgodnie z rozpoznawaniem modelu w stockowym sterowniku fan54005. Odczyt profilu: reg0=d0, reg1=70, reg2=8e, reg3=41, reg4=81, reg5=24, reg6=7b. Reg2 oznacza 4.20 V; reg1 zachowuje limit wejścia USB 500 mA. Nie zmierzono prądu baterii ani wartości bocznika.

Eksperyment na v52 z tymczasowo odpiętym sterownikiem bootstrap i przywróceniem go po teście: 10 impulsów TMR_RST (reg0=c0) co 8 s przez 80 s. Po każdym impulsie fault=0, pozostały profil bez zmian, TEST_DONE result=0. Po zakończeniu impulsów późniejszy odczyt status=f6 wykazał TimerFault=6. Surowy, odfiltrowany wynik: phone/hl_watchdog_result.txt. Osobny test reg1: 78 (TE), 7c (CE wyłączone), przywrócenie70, odczyty zgodne. To potwierdza zapis/odczyt bitów, nie rzeczywisty prąd.

Stock Ghidra charging_reset_watch_dog_timer ustawia reg6=7b dwukrotnie i następnie TMR_RST. W v53 nie kopiujemy zapisów reg6. Wyciąg: phone/ghidra/k44_hl_watchdog.c. Stock DT ma normalny przedział JEITA 10–45°C i max_charging_time=43200 s; nie wdrażamy wyższych napięć stocka ani pełnej tabeli JEITA.

Źródła producenta: [HL7005D](https://hmisemi.com/hl7005d/), [product brief](https://hmisemi.com/app/uploads/2023/11/HL7005D_Product-Brief_R1.7_0919.pdf), [pełna karta producenta V1.0, kopia](https://datasheet4u.com/pdf-down/H/L/7/HL7005D-HALO.pdf). Pełny PDF był dostępny przez indeks wyszukiwarki, bezpośredni fetch zwracał403. Rozróżniać 32-sekundowy watchdog trybu HOST od zachowania AUTO bez obsługi hosta.

## Zakres sterownika v53

Worker co 8 s, pierwszy po 500 ms. Świeży NTC oraz snapshot ładowarki, następnie kontrola wąskiego profilu. Dopuszczone reg1 po odjęciu CE/TE:30 lub70 (100/500 mA), reg2=8e, reg4&7f=1, reg5&e7=24, reg6=40 lub7b. 40 jest wartością domyślną z karty, 7b profilem zmierzonym; inne profile powodują pauzę/-ERANGE, bez przepisywania limitów.

Jedynie zapisy reg0 TMR_RST z zachowaniem EN_STAT oraz reg1 CE/TE. Reg2/4/5/6 bez zapisów; nie podnosimy napięcia ani limitów prądu. TE włączone; CE włączone tylko dla 10 <= T < 45°C, poprawnego NTC i sesji krótszej niż 43200 s. Brak/poprawny poza zakresem NTC wyłącza CE. Odnotowane odłączenie USB zeruje czas sesji; zegar sesji nie jest zachowany po restarcie. Tryb boost/OTG pomijany. Zatrzymanie workerów przy shutdown.

To etap bootstrap: brak pomiaru prądu, fuel gauge/%, pełnej JEITA, obsługi zewnętrznego CDIS i kompleksowego sterownika charger. Awaria magistrali/profil nieobsługiwany oznacza pauzę managera, nie gwarancję wymuszonego wyłączenia ładowarki. Nie uznawać urządzenia za gotowe do codziennego używania na podstawie tego etapu.

Diagnostyka tylko do odczytu:
- /sys/bus/platform/devices/1000d000.pwrap/charger_service: kicks, errors, timer_faults_seen, last_error, profile_ok, blocks (1 temperatura, 2 czas), temp w 0.1°C, elapsed_s.
- charger_registers w tym samym katalogu: wszystkie kontrolowane pola profilu oraz fault/online. Licznik timer_faults_seen liczy zaobserwowane błędy, inne odczyty mogą skonsumować stan zatrzaśnięty.

## Artefakty i weryfikacja

Obraz D:\y5_linux_work\mfw_v53.img, 8796160 B. SHA256:
09d14a4e62b8d953ab56fc0ab819e5586d16cae961ecd0a9b9edc5617a258fd5

Image out/Image_mfw_v53 ma 12828680 B. DTB out/v14.dtb, config jak v51, initramfs/cmdline/adresy identyczne z v52. Zachowana poprawka restartu v52. Pełny patch phone/kernel/mfw-kernel-7.3-v53.patch (22 pliki) stosować ZAMIAST wcześniejszych pełnych patchy, nie na nie.

Build arm64/CROSS_COMPILE i porównanie config PASS. verify_mfw_v53.py: Image/DTB/SHA1 boot/rozmiary/zgodność z v52 PASS. Dry-run pełnego patcha na czystej bazie7.3 PASS. UBSan test_hl_policy.sh: wszystkie256 wariantów reg1 x4 maski blokad, dozwolone maski zapisu, profil i granice99/100/449/450 oraz43199/43200 PASS. Źródła testów, budowania i eksperymentów w archive/pc_work. Obrazy i prywatne backupy pozostają poza repo.

## Aktualizacja i test akceptacyjny

Wykonano tylko --stage (backup boot24MiB, upload do RAM, sprawdzenie SHA256). Nie wykonano --write. Przed zapisem wymagana zgoda właściciela zgodnie z jego zasadą9 dotyczącą eMMC. Po zgodzie:

```powershell
python D:\y5_linux_work\mfw_update_boot.py D:\y5_linux_work\mfw_v53.img --write --reboot
```

Krótkie Power na LK, następnie potwierdzić nowy kernel (inny niż #36) i komunikat watchdog service 8s. Obserwować co najmniej 2 minuty: kicks rośnie, errors nie rośnie, last_error=0, profile_ok=1, blocks=0 przy obecnej temperaturze ~29°C, fault=0; timer_faults_seen nie rośnie po ewentualnym początkowym błędzie. Profil zachowany, control zwykle78. Sprawdzić napięcie/NTC, klawisze i restart. Granic temperatury nie testować przez fizyczne grzanie ani omijanie czujnika. Następny krok: obserwacja odłączenia/podłączenia USB z właścicielem, potem dłuższa ocena trendu napięcia. Nie deklarować zakończonego ładowania na podstawie samego bitu STAT.

## Wynik pierwszego uruchomienia v53 — PASS z opóźnionym przejęciem
Właściciel zatwierdził zapis i jeden test, gdy był jeszcze przy PC. --write bez --reboot zakończył pełny backup/zapis/readback SHA256 PASS. Następnie oddzielny sync; reboot (ostatni dozwolony tej nocy), krótkie Power na LK wykonane przez właściciela. Uruchomiony Linux7.3 #39, build01:54:21 CEST. Od tego momentu właściciela nie ma przy telefonie: żadnych kolejnych restartów/poweroff/suspend/resetów ani testów wymagających USB/Power.

Istotna obserwacja: LK po restarcie zostawił control78/special04/oregb6/ibatf1/safety7b. W 19. sekundzie kicks0/errors3/-ERANGE, zgodnie z guardem nieznanego profilu; manager niczego nie przepisywał. Około pierwszych32s timer sprzętu powrócił do profilu domyślnego8e/81/24. Próbka47s: kicks2, errors4, timer_faults_seen1, last_error0, profile_ok1, blocks0, control78, fault0. To NIE była awaria rozruchu ani trwała pauza sterownika.

Dalsze 12 próbek do uptime236s: kicks25, errors nadal4, timer_faults_seen nadal1, fault0, NTC290, VBAT3882073uV. Odczyt przy uptime414s: kicks47, errors4, timer_faults_seen1, last_error0, profile_ok1, blocks0, NTC291, VBAT3886029uV, statusCharging. CPU949999755Hz, input event0 nadal z trzema klawiszami. To potwierdza kilka minut obsługi, nie pełny cykl ładowania ani pomiar prądu. Wczesny TimerFault przy przejściu do domyślnego profilu jest znanym ograniczeniem bootstrap; przyszła inicjalizacja wymaga osobnego, zweryfikowanego projektu. Nie rozluźniać guardów do wyższego napięcia b6.

Dowody bez identyfikatorów: phone/v53-first-boot.txt. Monitor PC mfw_night_monitor.py (archive/pc_work) działa co60s przez10h; zbiera tylko sysfs do D:\y5_linux_work\out\mfw-night-telemetry.jsonl i mfw-night-latest.json. Nie wykonuje żadnych zapisów na telefonie. Zatrzymanie: utworzyć lokalny plik out/mfw-night-monitor.stop. PID w out/mfw-night-monitor.pid. PC musi pozostać uruchomiony; nie zmieniano ustawień zasilania Windows.

## Wynik nocny i zegar
Wyniki10h i ograniczenia ADC/CV zapisano w docs/NIGHT_WORK_2026-10-25.md oraz phone/mfw-night-summary-10h.json. 546 poprawnych próbek,33 błędy odczytu, liczniki błędów sterownika bez wzrostu. Nie zaobserwowanoFull. Oczekujemy jeszcze potwierdzenia granicy43200s. Ustawienie czasuUTC/strefyWarsaw wykonano bez restartu; limit ładowania opiera się na jiffies i nie został wyzerowany.

## 2026-10-25: RZECZYWISTY limit sesji12h PASS, bez restartu
Próbka elapsed43188: blocks0/control78/Charging/kicks5269. Próbka43204: blocks2/control7c/Not charging/kicks5271. Kolejne próbki do43294: blocks2, CE nadal wyłączone7c, kicks rośnie do5282, errors nadal4, timer_faults_seen nadal1, last_error0, fault0. Obserwacja po wyłączeniu przekroczyła32s watchdoga. To pomiar na telefonie, nie tylko unit test. VBAT spadł do4.169641V, NTC32.3C; nie interpretować tego jako pomiaru prądu lub dowodu kalibracjiADC. Dowód phone/charge-limit-12h.json i kod obserwacji archive/pc_work/observe_charge_limit_12h.py.
Ładowanie pozostawione zatrzymane zgodnie z limitem. Właściciel dostał prośbę o odłączenieUSB na20s i ponowne podłączenie bez wyłączania, aby sprawdzić CHRDET/reset sesji/wznowienie. Wynik tego osobnego testu JESZCZE NIEZNANY. Nie symulować odłączenia resetem kontrolera. Telefon nadal v53, bez kolejnego reboot.

## 2026-10-25: fizyczne odłączenie/podłączenie USB po limicie
Właściciel odłączył kabel na około20s i podłączył ponownie bez wyłączania telefonu. Świeży odczyt: uptime43448s (bez reboot), elapsed_s16, blocks0, profile_ok1, control78, online1, statusCharging, kicks5297, errors4, timer_faults_seen1/fault0. Sesja została wyzerowana i ładowanie wznowione. Nie odczytaliśmy online0 podczas braku połączenia: dowodem odłączenia jest czynność właściciela oraz nowy licznik sesji przy ciągłym uptime. Dłuższy odczyt powtórny trwa, wynik zostanie dopisany. Nie wykonywano restartu kontrolera ani kernela.

Retest po poprawce klienta PASS: cztery próbki nowej sesji253->303s, kicks5326->5332, errors4 i timer_faults_seen1 bez zmian, control78/online1/blocks0/Charging/fault0 w każdej próbce, uptime>12h. Dowód phone/charge-reconnect.json. Poprawiony loggerPC uruchomiony na dalsze2h (bez żadnych automatycznych działań na telefonie).
