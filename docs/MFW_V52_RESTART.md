# MICROFW v52 — test restartu przez TOPRGU

2026-10-25. Właściciel polecił: najpierw restart; ładowanie dopiero po jego potwierdzeniu. mfw_v51.img pozostaje obrazem sprawdzonym na urządzeniu. mfw_v52.img jest gotowy do testu, ale jeszcze niewgrany.

## Hipoteza i zmiana
Przy wcześniejszym reboot z v50 ekran zatrzymał się na 'requesting system reboot'. Zapis USB/readback był poprawny, a po długim Power wystartował v51. W źródłach PSCI ma restart-priority129, watchdog MTK128. Zawieszenie w firmware SYSTEM_RESET jest możliwym wyjaśnieniem, nie zostało jeszcze zmierzone bezpośrednio.

V52 zmienia TYLKO wybór ścieżki restartu:
- DT root compatible: huawei,dura-l21 oraz dotychczasowy mediatek,mt6739.
- mtk_wdt.c: dla huawei,dura-l21 restart-priority192, przed PSCI129; pozostałe platformy zachowują128. W logu startu: DURA-L21: TOPRGU restart priority 192 (before PSCI).
- Sam mtk_wdt_restart, rejestry/modele resetu, inicjalizacja i timeout watchdoga są bez zmian. Dotychczasowy callback wyłącza tryb IRQ i pisze klucz SWRST. V52 sprawdza hipotezę priorytetu; nie gwarantuje jeszcze poprawnego restartu.
- Nie zmieniono ładowarki, PMIC, zegarów CPU, USB, rootfs ani initramfs. Ładowanie pozostaje następnym etapem po sukcesie restartu.

## Artefakty i weryfikacja
- D:\y5_linux_work\mfw_v52.img, rozmiar8794112B.
- SHA256:10f1bd8e0c23491ce13b3cbdf062fd4698be13ebc50bad412baae5953d535d90.
- Kernel out/Image_mfw_v52 (12828680B), DT out/v14.dtb, builder bm_mfw_v52.py.
- CONFIG identyczny z config-7.3-mfw-v51 (cmp PASS). Nie ma nowego zestawu opcji.
- Build mtk_wdt.o i Image PASS; nowy komunikat znaleziony w vmlinux. fdtget potwierdza dwa compatible w v14.
- verify_mfw_v52.py: nowe Image/DT, boot SHA1, rozmiar oraz identyczność initramfs/cmdline/adresów z v51 PASS.
- Pełny alternatywny patch21 plików: phone/kernel/mfw-kernel-7.3-v52.patch, dry-run na czystej bazie7.3 PASS. Nakładać zamiast wcześniejszych pełnych patchy; config jak v51.
- DT: out/v13.dts + phone/kernel/mfw-v52.dtsi -> build_dtbv14.sh -> v14.dtb.
- Skrypty odtwarzające i builder w archive/pc_work: build_v52_restart.sh, build_dtbv14.sh, collect_v52.sh, bm_mfw_v52.py, verify_mfw_v52.py, export_v52_patch.sh.

## Stan przygotowania aktualizacji
Wykonano tylko mfw_update_boot.py --stage dla v52. Preflight, prywatny backup całego boot24MiB i staging obrazu do RAM z SHA256 PASS. Backup pozostaje D:\y5_linux_work\boot_update_backups, bez eksportu do repo/czatu. Nie wykonano --write, reboot ani innego resetu telefonu w tej turze.

Pierwotna zasada właściciela wymaga pytania przed zapisem eMMC. Bieżące polecenie autoryzuje pracę nad restartem; poprzednia zgoda dotyczyła konkretnego zapisu v51. Przed pierwszym zapisem gotowego v52 uzyskać zgodę na zapis boot i test restartu. Po zgodzie:
```powershell
python D:\y5_linux_work\mfw_update_boot.py D:\y5_linux_work\mfw_v52.img --write --reboot
```

## Test ma dwa osobne etapy
1. Przejście v51 -> v52. Reboot w aktualizatorze wykonuje JESZCZE stary v51, więc może ponownie utknąć; wtedy właściciel wymusza restart Power >=10s. Nie oceniać poprawki v52 na podstawie tego przejścia.
2. Po uruchomieniu v52 sprawdzić uname, zgodność root compatible i powyższy komunikat restart-priority192. Następnie wykonać zwykłe sync; reboot z NOWEGO kernela. Obserwować faktyczny reset, ponowny start i powrót USB/powłoki z wyzerowanym uptime. Krótkie Power na czerwonym ekranie LK (jeżeli potrzebne) odróżnić od długiego Power wymuszającego reset.

Sukces poprawki: restart z v52 opuszcza etap Requesting system reboot i uruchamia telefon bez długiego Power. Jeśli dalej stoi, utrwalić ekran/obserwację i odróżnić problem przed wywołaniem callbacku od samego resetu TOPRGU. Dopiero potem kolejna zmiana. Przy braku ping poprosić o zdjęcie, nie zgadywać. Nie rozpoczynać sterowania ładowaniem przed zakończeniem tego testu.

## Zapis v52 — wykonany, start/test oczekujący
Po zgodzie właściciela aktualizator ponownie pobrał i zweryfikował prywatną kopię boot24MiB, wgrał mfw_v52.img do RAM, zapisał p36 i potwierdził SHA256 całych8794112B odczytem z urządzenia. Wysłano reboot ze starego v51. Pierwszy ping po restarcie0/2. Właściciel dostał instrukcję długiego Power, jeżeli stoi na Requesting system reboot. Nie uznawać tego pierwszego przejścia za test nowego priorytetu. Czekamy na uruchomienie52, jego komunikat TOPRGUpriority192 i dopiero potem zwykły reboot z52.

## Wynik testu: PASS
V52 uruchomiony (#36), root compatible i TOPRGUpriority192 potwierdzone. Zwykły sync; reboot z52 sam przeszedł do LK; właściciel użył tylko krótkiego Power na czerwonym ekranie. System wrócił: #36, uptime27s, komunikat192. Długi Power nie był potrzebny. Restart SoC działa, ręczne potwierdzenie LK pozostaje; nie zmieniać bootloadera. mfw_v52.img jest bieżącym potwierdzonym obrazem. Kolejny etap zgodnie z właścicielem: ładowanie HL7005D.
