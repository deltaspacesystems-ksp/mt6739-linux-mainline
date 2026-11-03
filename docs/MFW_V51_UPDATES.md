# MICROFW v51: stan ładowarki, zegar CPU i aktualizator USB

2026-10-25. Właściciel potwierdził rozruch v50. Pomiar: Linux 7.3.0 #34, temp=277 (27,7°C), voltage_now=3843841 uV, USB i powłoka działają. **v50 pozostaje obrazem potwierdzonym; mfw_v51.img oczekuje na pierwszy test.**

## Nowe nazewnictwo
Na życzenie właściciela nowe obrazy nazywać `mfw_vNN.img`; pierwszy to `D:\y5_linux_work\mfw_v51.img`. Nie zmieniać nazw wcześniejszych obrazów, aby zachować istniejące procedury odtworzenia. Builder: bm_mfw_v51.py, kernel out/Image_mfw_v51, DT out/v13.dtb.

## Ładowarka HL7005D
- Powtórzony pomiar I2C5: ID=0x41, CHG_STATUS reg0=0xf6, control reg1=0x70, special reg5=0x24. ID odpowiada HL7005D w stockowej procedurze wykrywania.
- Stan z reg0[5:4], CE z reg1[2], HZ_MODE z reg1[1], stan pinu CDIS z reg5[3]. Sam STAT=1 oznacza zezwolenie PWM; nie zgłaszać Charging, jeżeli ładowanie jest wyłączone przez CE/HZ/CDIS.
- Detekcja VBUS: stock upmu_get_rgs_chrdet czyta pole0x4c1. Z tabeli pól: MT6357 rejestr0xa88, mask1, shift4. Zmierzono 0x001b, bit4=1 przy podłączonym USB. Odczyt wymagał krótkiego unbind/bind z podpowłoką i trap; po bind temperatura wróciła (279).
- v51 dodaje microfw-usb/{online,status,model_name}, aktualny status baterii w microfw-battery/status oraz diagnostyczny /sys/bus/platform/devices/1000d000.pwrap/charger_registers. Zapisów parametrów ładowarki ani karmienia jej watchdoga jeszcze nie dodano.
- STAT=0 -> Not charging przy obecnym VBUS; 1 -> Charging tylko przy włączonym torze; 2 -> Full (zakończenie według ustawień układu, nie pomiar procentu); 3 -> Not charging. Brak VBUS lub aktywny boost -> Discharging. Błąd odczytu nie jest zastępowany fałszywym Charging.
- Aktualny kod błędu6 odpowiada timer fault w tabeli HL7005D. W v51 ma być widoczny jako brak ładowania i fault=6 w surowej diagnostyce. To etap odczytu statusu, nie uruchomienie pełnego zarządzania ładowaniem.
- I2C5 nadal działa jako wąski bootstrap PIO wewnątrz sterownika płytki, pod wspólnym mutexem z WACS2. Stary węzeł I2C5 wyłączony; nowy zasób MMIO należy do monitora PMIC. Zegary/piny/timing z LK, timeout20 ms, brak DMA i resetów. Odtwarzane zmienione rejestry kontrolera. Wysyłany tylko bajt indeksu i repeated START + odczyt, bez bajtów danych konfiguracyjnych do ładowarki.
- Brak okresowych ueventów ładowarki; użytkownik/aplikacja odczytuje świeże sysfs. Brak procentu, prądu baterii, konfiguracji ładowania oraz pełnego PMIC/IRQ/suspend.

Źródła: lokalne dekompilacje phone/ghidra/k44_v51_{clocks_status,more}.c, phone/v51_tables_44.txt; [strona producenta HL7005D](https://www.halomicro.cn/pro_detail/989816552942030848.html) i producentowska tabela bitów HL7005D V1.0 July2016, str.22 ([publiczny PDF producenta na mirrorze](https://datasheet4u.com/pdf-down/H/L/7/HL7005D-HALO.pdf), dostępny w indeksie wyszukiwarki; bezpośrednie pobranie zwracało403). Nie użyto tabel FAN54005 jako zamiennika HL7005D.

## CPU: rzeczywisty odczyt zegara
- W v50 CONFIG_CPU_FREQ oraz CPU_IDLE są wyłączone. Nie ma sterownika MT6739 ani polityk cpufreq.
- Stock mt_cpufreq_dts_map: mediatek,apmixed -> 0x1000c000; mediatek,topckgen_ao -> 0x1001b000. prepare_pll_addr: ARMPLL_CON1 offset0x204, CLKDIV offset0x2c. Mux z TOPCKGEN_AO offset0, bity11:8.
- Zmierzone: ARMPLL_CON1=0x811244ec, CLKDIV=0, MUX=0x100. Stockowy kod traktuje CLKDIV=0 jako brak dzielenia. PCW22bit,14 bitów ułamkowych, postdiv z bitów26:24. Nominalnie 949999755 Hz, czyli około950 MHz. To odczyt ustawień PLL względem nominalnego26 MHz, nie pomiar częstotliwości miernikiem.
- v51: clk-mt6739-boot-cpu.c i mfw-cpu-rate.h, read-only provider CCF `mfw_cpu`; sysfs /sys/bus/platform/devices/mfw-cpu-clock/cpu_rate_hz; debugfs clk/mfw_cpu/clk_rate. Brak set_rate, zmian PLL/mux/divider i napięć. Zachowana częstotliwość z LK.
- Wszystkie cztery CPU wskazują provider. Usunięto stare, niespójne clock-frequency z DT (2 GHz/1,638 GHz), zamiast przedstawiać je jako pomiar. Nieobsługiwane lub zmieniające się konfiguracje zegara zwracają brak danych.
- CONFIG_MFD_SYSCON=y oraz COMMON_CLK_MT6739_BOOT_CPU=y. CPU_FREQ pozostaje wyłączony; nie jest to DVFS. Do regulacji częstotliwości trzeba jeszcze potwierdzić VPROC/VSRAM i OPP.

## Testy i obraz
- Testy C z UBSan: obliczenia PLL (w tym950 MHz i ułamkowe dzielniki), nieobsługiwane mux/dzielniki; wszystkie kombinacje statusu/control/special pod kątem błędnego Charging przy odłączonym lub wyłączonym torze. PASS.
- Kernel i dwa nowe obiekty kompilują się. Image=12828680 B; obraz=8794112 B. Nowe teksty obu sterowników znalezione w vmlinux.
- SHA256 mfw_v51.img: 8cb66e3f9a997219f1127828860629f7d5e4a86237d076e26c6bd332fb108fe8.
- verify_mfw_v51.py: PASS nowego Image/DTB, SHA1 boot, limitu24MiB, identycznego initramfs/cmdline/adresów względem v50. Pełny alternatywny patch20 plików phone/kernel/mfw-kernel-7.3-v51.patch; dry-run na czystym7.3 PASS. Config config-7.3-mfw-v51. Nie nakładać go po wcześniejszych pełnych patchach.
- DT: do out/v12.dts dopisać phone/kernel/mfw-v51.dtsi, skompilować build_dtbv13.sh. Budowanie: build_v51.sh, zawsze ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu-, nohup bld.sh w żywej nadzorowanej sesji WSL.

## Aktualizacja bez BROM: przygotowana, zapis jeszcze nieautoryzowany
Właściciel zapytał o aktualizacje bez BROM. Userland już można zmieniać przez USB; kernel wymaga zmiany obrazu boot i restartu. Kexec nie jest obecnie włączony. Przygotowano tools/mfw_update_boot.py (kopia robocza w D:\y5_linux_work, kopia archiwalna w archive/pc_work).

1. Domyślne --check: tylko odczyt. Waliduje format/rozmiar/układ adresów boot, SHA1 payloadów, gzip kernela i initramfs, DTB; sprawdza rootfs p46 i cel p36 z PARTNAME=boot, rozmiarem24MiB i stanem writable.
2. --stage: pobiera CAŁĄ partycję boot do prywatnego D:\y5_linux_work\boot_update_backups, porównuje SHA256 z urządzeniem przed/po pobraniu, wgrywa nowy obraz do RAM /tmp/mfw-update.img i sprawdza sumę. Kopii boot ani manifestu NIE eksportować do repo ani czatu.
3. --write: oddzielny, jawny zapis TYLKO /dev/mmcblk0p36 po ponownych kontrolach. dd conv=fsync, sync, odczyt całego zapisanego obrazu i SHA256. Blokada /run/mfw-boot-update.lock. --reboot działa wyłącznie z --write i po poprawnym readback. Nie ma automatycznego przywracania po przerwanym zapisie ani slotu A/B; BROM pozostaje odzyskiwaniem.

**W tej sesji wykonano --check oraz --stage dla v51, bez --write i bez restartu.** Zweryfikowano prywatną kopię24MiB i obraz w RAM. Manifest jest prywatnie w boot_update_backups/latest.json; gotowy skrypt zapisu do przeglądu: D:\y5_linux_work\out\mfw-write-boot.sh. sh -n PASS (bez wykonania). Pięć testów walidatora/bezpiecznego trybu domyślnego/odmowy przy błędnym preflight PASS. Samego zapisu aktualizatorem jeszcze nie testowano.

Zgodnie z pierwotnymi zasadami właściciela (BROM ręcznie i zgoda przed zapisem eMMC) trzeba uzyskać jego wyraźną zgodę na pierwszy zapis boot z działającego MICROFW i restart. Pytanie ma dotyczyć gotowego v51. Do tej zgody NIE uruchamiać --write. Po zgodzie:
```powershell
python D:\y5_linux_work\mfw_update_boot.py D:\y5_linux_work\mfw_v51.img --write --reboot
```
Dotychczasowy wariant ręczny, gdy właściciel wybierze BROM:
```powershell
cd D:\mtkclient_work
python mtk.py w boot D:\y5_linux_work\mfw_v51.img
```
Po starcie sprawdzić temperaturę/napięcie, status/online/model_name ładowarki, charger_registers, mfw_cpu/cpu_rate_hz i trzy przyciski. Gdy nie ma ping, poprosić o zdjęcie ekranu. Przywrócenie: właściciel BROM do wd_v50.img; nowa prywatna kopia boot również zachowana na PC.

## Pierwszy zapis aktualizatorem — wykonany 2026-10-25
Właściciel zatwierdził konkretny zapis i restart ('zapisujmy!'). Uruchomiono --write --reboot. Ponowny backup całego boot 24 MiB, wgranie obrazu do RAM, zapis p36 i pełny readback 8794112 B/SHA256 przeszły poprawnie. Następnie wysłano reboot. To potwierdza działanie zapisu bez BROM, ale jeszcze nie rozruch nowego kernela.
Po restarcie telefon nie odpowiadał na kilka krótkich prób ping. Windows nie pokazywał wtedy karty RNDIS ani adresu192.168.7.x. Nie ustalono przyczyny i nie wykonywano ponownego zapisu ani rollbacku. Poprosić właściciela o zdjęcie ekranu; może to być ekran LK lub etap startu wymagający diagnozy. v50 pozostaje ostatnim potwierdzonym rozruchem.

Aktualizacja diagnozy: właściciel widzi 'requesting system reboot'. Najprawdopodobniej zawisł restart v50, a boot v51 jeszcze się nie odbył. Wymuszenie restartu Power >=10s; nie wgrywać ponownie tego samego obrazu tylko z powodu tego ekranu. Źródła mają PSCI priority129 i watchdog128; możliwe utknięcie w PSCI to hipoteza do sprawdzenia po odzyskaniu. [Instrukcja Huawei dotycząca Power](https://consumer.huawei.com/en/support/content/en-us15846652/).

## Test rozruchu v51 — POTWIERDZONY 2026-10-25
Po wymuszonym restarcie telefon wstał. uname: Linux7.3.0 #35, build00:51:26 (v50=#34). Nowe logi i sysfs potwierdzają v51. Pierwszy odczyt przy uptime30s: temp288, napięcie3876800uV, battery/USB statusCharging, modelHL7005D, online1, fault0; rejestry0xd0/0x78/0x04. CPU949999755Hz. To krótki test bieżących odczytów, nie długotrwałego ładowania. Nie wykonywano ponownego pełnego testu przycisków.
**mfw_v51.img jest teraz bieżącym potwierdzonym obrazem; v50 pozostaje awaryjny.** Aktualizator zapisał obraz bez BROM poprawnie; restart programowy z v50 zawisł, wymagając ręcznego Power. Restart trzeba naprawić osobno; PSCI129 vs watchdog128 pozostaje hipotezą do sprawdzenia, nie potwierdzoną przyczyną. Ten problem nie oznacza awarii nowego boot.

Dalszy pomiar przy uptime2:20: status Not charging, następny raw0xd0/0x70/0x24/fault0. Odczyty nie były jednoczesne. Względem pierwszego odczytu zmieniły się control/special bez zapisów konfiguracji przez nasz sterownik. Sprawdzić 32-sekundowy watchdog/autonomiczny reset konfiguracji HL7005D; na razie hipoteza, nie potwierdzona przyczyna. Priorytet dalszego rozwoju: restart, ładowanie, właściwe PWRAP/MT6357, zegary i DVFS, I2C/GPIO oraz urządzenia wejściowe/czujniki.
