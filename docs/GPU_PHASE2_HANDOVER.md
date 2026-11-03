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
