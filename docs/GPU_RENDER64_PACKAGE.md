# Pakiet pierwszego renderu 3D 64×64 — przekazanie pracy

Stan opisany 2026-10-27, na podstawie pracy do commitu `77f76ee`.

**Pierwszy trójkąt NIE został jeszcze uruchomiony ani wyrenderowany.** Działa natywny shader compute z niezależnym odczytem wyniku; pakiet grafiki jest kandydatem do pierwszego testu TA/3D. Wayland z GPU i WebGL2 są dalszym celem, nie osiągniętym wynikiem.

## Gdzie są pliki

- Gotowe binaria na PC: `D:\y5_linux_work\out\graphics64_trial1\`.
- Manifest rozmiarów, SHA256 i rozmieszczenia: [GPU_RENDER64_MANIFEST.json](GPU_RENDER64_MANIFEST.json).
- Kod i generatory w repo: `archive/pc_work/`; oryginalne skrypty robocze: `D:\y5_linux_work\`.
- Docelowe pliki testowe na telefonie: `/tmp/mfw-graphics64/`.
- Pełny zapis ostatniej próby: [gpu-graphics64-bundle-staging.json](../phone/hardware/gpu-graphics64-bundle-staging.json).

Binaria pozostają poza repo. Dokumentacja i kod pozwalają je sprawdzić lub odtworzyć. Nie kopiować firmware, bibliotek stockowych, obrazów partycji ani sekretów do repo.

## Co ma zrobić pierwsza klatka

Ścieżka: VDM pobiera trzy identyfikatory wierzchołków → PDS przekazuje VertexID do `vi0` → natywny shader VS emituje pozycje trójkąta → PPP/TA przygotowuje rasteryzację → shader FS emituje stały kolor → EOT i PBE zapisują kafelki do własnego bufora RGBA8 64×64. CPU ma odczytać piksele po zakończeniu GPU.

To render poza ekranem. Pakiet nie przejmuje panelu telefonu. Nie ma jeszcze podłączenia do kompozytora, EGL/GLES ani przeglądarki.

| Plik | Rozmiar | Zastosowanie |
|---|---:|---|
| `general-vdm.bin` | 4096 B | Referencja PPP, stan VS, draw 3 wierzchołków, strumień zakończenia |
| `general-ppp.bin` | 4096 B | PPP 25 DWORD, viewport, PDS, ISP, stan zakończenia |
| `pds-main.bin` | 4096 B | Uruchomienie VS, metadane FS, FS, EOT i synchronizacja VDM |
| `pds-context.bin` | 4096 B | Dwa banki programów zapisu/odtworzenia PT i SR kontekstu |
| `usc-main.bin` | 4096 B | Natywne VS 176 B, FS 440 B, EOT 40 B, sync NOP 8 B |
| `usc-context.bin` | 4096 B | Natywne programy SR store 568 B i load 488 B |
| `ta-context.bin` | 112 B | Adresy VDM/TA i dwa banki rejestrów przełączania kontekstu |
| `pbe-state.bin` | 32 B | Dwa DWORD stanu PBE i trzy rejestry 64-bitowe |

## Adresy i pamięć

Sterty zostały odczytane z rzeczywistego drivera Y5, nie przepisane z GE8300:

| Sterta | Baza GPU VA |
|---|---|
| General | `0x65c0000000` |
| PDS | `0xa800000000` |
| USC | `0xba00000000` |
| MMU INIA BRN65273 | `0xf800000000` |
| MMU INIB BRN65273 | `0xf900000000` |

Offsety poniżej są względem odpowiedniej sterty, nie adresami fizycznymi:

| Obszar | Offset / zakres |
|---|---|
| General: wyjściowy RGBA8, 16 KiB | `0x0000..0x3fff` |
| General: VDM / PPP | `0x4000` / `0x5000` |
| General: TPC + VHeap | `0x6000`; VHeap od `+128 B` |
| General: region headers | `0x7000` |
| General: VDM state / TA state | `0x8000` / `0x9000` |
| General: MList / VDM callstack / dummy stencil | `0xa000` / `0xb000` / `0xc000` |
| General: chronione tabele freelist global / local | `0xd000` / `0xe000` |
| PDS: główne programy / programy kontekstu | `0x10000` / `0x12000` |
| PDS: oba bufory PT | `0x20000` i `0x20080` |
| PDS: SR bank 0 / bank 1, po 5 stron | `0x30000..0x34fff` / `0x35000..0x39fff` |
| USC: główne shadery / shadery kontekstu | `0x10000` / `0x11000` |

Łącznie 32 PMR po 4 KiB: General 15, PDS 13, USC 2, MMU init 2. Dwie tabele freelist są GPU-only z jawną translacją PM protection bit26 → legacy bit24; nie wolno mapować ich do CPU. Pozostałe 30 stron ma dostęp CPU do przygotowania/odczytu.

**Aktualny EOT jest skompilowany dla wyjścia `0x65c0000000`.** Wcześniejszy pojedynczy kandydat PBE/EOT wskazywał `0x65c0010000`; nie mieszać tych zestawów.

## Co zostało sprawdzone

| Element | Wynik i granica dowodu |
|---|---|
| Compute USC | Dwa rzeczywiste testy na Y5: różny literal i adres, wynik/UFO/guardy/cleanup PASS. To nie trójkąt. |
| Render context, freelist, RenderTarget, HWRT | Osobne cykle CREATE/DESTROY na telefonie PASS; wcześniejszy HWRT był testem lifecycle, bez rysowania. |
| Pamięć pełnego renderera | 32 PMR / 5 stert, 30 CPU zero/pattern/readback + 2 protected tables PASS; pełne zwolnienie RC0. |
| Layout firmware | Z dokładnego załadowanego FW/DWARF: TA regs 104 B, 3D regs 608 B, CMDTA 280 B, CMD3D 664 B; host i ARM64 staticassert PASS. |
| KICKTA3D ABI | Bridge `130/14`: wejście 352 B, wyjście 16 B. Generated header, stock wrapper i native ARM64 zgodne. Nie wysłano KICKTA3D. |
| VS/FS/EOT/PDS/VDM/PPP/PBE | Kandydaci offline; kompilacja, kontrola rozmiarów/map/overlap oraz opisane harnessy ASan/UBSan PASS. Wykonanie grafiki na GPU niepotwierdzone. |
| `graphics19_stage_trial.c` | Host C11/Werror PASS. Nie został jeszcze wgrany ani wykonany na telefonie podczas ostatniej próby. |

Ostatni zmierzony stan GPU po teście pamięci: Driver/Firmware OK, HWR **1 bez przyrostu**, CRR 0, GPU idle 0%. HWR 1 pochodzi z wcześniejszego testu compute bez mapowań MMU INIA/INIB. Samo zakończenie UFO nie wystarcza: HWR też może zakończyć UFO.

## Ważne ustalenia i poprawione tropy

- Target to zmierzony BVNC `22.49.21.16`, natywne Alpine, bez Bionic/HAL/libhybris.
- Fizyczne limity USC depth/slots nadal UNKNOWN. Budżet RA20 jest wyłącznie ograniczeniem osobnego kompilatora testowego; nie staje się profilem sprzętu. Produkcyjnego kompilatora tym nie zastąpiono.
- VS ma **5 wyjść**, bo lowering dodaje PointSize do czterech składowych Position. Poprawiono VDM na 5 i PPP OUTPUT_SEL na `0x05040000`; wcześniejszego wariantu 4 nie wysłano.
- FS: 9 temps, shared meta `0:1`, brak phase change i coeffs, ale `discard=1` przez coverage prologue. Zgodnie z publiczną ścieżką wymaga `PUNCH_THROUGH`. Metadane FS `0x202` są ustawiane przez PDS do shared0.
- Stockowy helper CAM, sprawdzony też ARM64: dla wybranego VS5/raster otrzymujemy CAM31 i maxInstances16. Nie jest to pełny profil pojemności.
- Y5 używa simple parameter format **v1**. Publiczne `pvr_arch_rt_mtile_info_init` w tej wersji Mesy wymaga v2 w danej gałęzi. Nie usuwać asercji i nie udawać v2.
- Ze stocku odtworzono tile16×16, region `align64(tileX*tileY*5)`, tail `nextpow2(max(tileX,tileY))²*8`. Dla64×64: 4×4 tiles, region128 B, tail128 B, VHeap1536 B.
- `PPPGridOffset=0` dla wybranych flags0; wcześniejsze pole `grid_candidate` oznaczało inny lokalny rejestr.
- Dokładny FW potwierdza TA context112 = dwa adresy 8 B + **2 banki po48 B**. Stock: VDM state92 B, TA state484 B, PT128 B i SR `0x4030` B na bank.
- Stary bridge bezwarunkowo wymaga poprawnego `PRFenceSyncPrimBlock`, nawet przy `bKickPR=false`. Nie podawać NULL.
- Dopasowanie clippingu PPP/terminate do starej ścieżki oraz pełne flags/PR/fence/refcount semantics nadal wymagają weryfikacji przed kontrolowanym draw. Złożenie plików nie zamyka tych punktów.

## Odtwarzanie i pierwszy staging

Główne skrypty na PC:

- `build_graphics_main_pds.py`, `build_graphics_context_pds.py` — generatory publicznego PDS.
- `build_pco_eot_output0.py`, `build_pco_syncnop20.py` — wybrane natywne programy USC; pozostałe shadery zgodnie z manifestem i wcześniejszymi generatorami.
- `build_generate_graphics_ppp.py`, `build_generate_graphics_vdm_segment.py`, `build_graphics_termination.py`, `build_pbe_output0_probe.py` — stan i strumienie.
- `package_graphics64_trial.py` — pakuje osiem plików, sprawdza znane SHA shaderów, rozmiary i brak nakładania danych.
- `prepare_graphics64_stage.py` — wytwarza klienta stagingu z dotychczasowego preflight/lifecycle kodu.
- `push_graphics64_stage.py` — przesyła osiem plików oraz źródło C, kolejno na porcie9500.
- `prepare_run_graphics64_stage.py` — generuje skrypt z SHA wszystkich ośmiu plików.

Generatory `build_*.py` dla Mesy uruchamiać w WSL `archlinux`, np.:

```powershell
wsl -d archlinux -- python /mnt/d/y5_linux_work/build_graphics_main_pds.py
python D:\y5_linux_work\package_graphics64_trial.py
```

Nie trzeba ponownie budować gotowych, zgodnych SHA plików. Skrypty używają `/home/y5/mesa-native` i przypiętego lokalnego builda Mesy; nie pobierają zdalnie źródeł.

Po odzyskaniu i sprawdzeniu łączności, przy jednym właścicielu testu telefonu:

```powershell
python D:\y5_linux_work\phone_shell.py "uname -a; uptime; cat /sys/kernel/debug/pvr/status"
python D:\y5_linux_work\phone_shell.py "mkdir -p /tmp/mfw-graphics64"
python D:\y5_linux_work\push_graphics64_stage.py
python D:\y5_linux_work\prepare_run_graphics64_stage.py
python D:\y5_linux_work\phone_shell.py --file D:\y5_linux_work\run_graphics64_stage.sh --timeout 30
```

Przed wznowieniem transferu sprawdzić pozostawiony listener9500; przerwana próba mogła zostawić listener lub niepełny plik. Skrypt uruchomienia ma `set -eu` i `sha256sum -c`; brak lub zły SHA musi zatrzymać staging.

Klient stagingu: mapuje 32 PMR, zeruje stan, wypełnia output `DEADBEEF`, ładuje sześć obrazów stron i metadane TA/PBE, sprawdza adresy i kluczowe słowa, tworzy render context → global/local freelist → RenderTarget → HWRT64×64, następnie zwalnia wszystko. **Nie zawiera wywołania KICKTA3D.** PASS potwierdzi przygotowanie zasobów, nie render.

## Ostatni transfer i łączność

Ostatnia próba potwierdziła przesłanie tylko `general-ppp.bin`, `general-vdm.bin`, `pbe-state.bin`. Na `pds-context.bin` nastąpił WinError10054, potem ping i telnet nie odpowiadały, a Windows nie widział adaptera RNDIS. Nie uruchomiono renderu, restartu, kexec ani flashowania. Przyczyna nieustalona.

Właściciel następnie napisał, że telefon jest; po tym komunikacie nie wykonywano nowego pomiaru łączności. Nie zakładać ani dalszej awarii, ani kompletnego transferu. Stan obecny sprawdzi agent przejmujący telefon.

## Co zostało do rysowania i do celu końcowego

1. Odzyskać/potwierdzić USB, sprawdzić uptime, kernel, status i diagnostykę; zweryfikować wszystkie osiem SHA.
2. Wykonać natywny staging opisany powyżej, z pełnym cleanup i bez przyrostu HWR.
3. Dokończyć natywnego klienta submission: wypełnić CMDTA280/CMD3D664 oraz KICKTA3D352 z rzeczywistymi uchwytami HWRT/sync, poprawnymi PR/fences/flags/refcounts i należącymi do testu adresami. Obecny klient tego nie robi.
4. Jeden kontrolowany draw; sprawdzić wynik pikseli, zakończenie, brak nowych HWR, guardy i cleanup. Timeout → zachować używane zasoby, zbadać stan; nie wykonywać ślepych ponowień ani niezweryfikowanego GPU kexec.
5. Dopiero potwierdzony render rozszerzać na pełny ARM64 Mesa/winsys, prezentację DRM/KMS/DMA-BUF i akcelerowany Wayland.
6. Następnie natywne GLES3 i WebGL2 w przeglądarce, z dowodem użycia sprzętowego renderera. Sam compute, host build biblioteki albo pojedynczy trójkąt nie potwierdza tych etapów.

Nocna automatyzacja jest PAUSED na prośbę właściciela. Właściciel upoważnił dalsze etapy bez pytań „czy robimy”, ale testy GPU i restarty wykonuje jeden agent naraz; drugi może równolegle opracowywać kod i zapisywać ustalenia w docs. Zachować zasady boot/rootfs-only, bez NVRAM i sekretów.
