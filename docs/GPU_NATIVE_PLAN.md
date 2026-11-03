# MICROFW: plan dojscia do natywnego 3D i akcelerowanego desktopu

Aktualizacja sesji 2026-10-26. Target: DURA-L21/MT6739, zmierzony BVNC22.49.21.16, Linux7.3/v62, Alpine. Runtime C/C++, bez Bionic/HAL/libhybris. To plan prac i bramki weryfikacji, nie obietnica terminu.

> 2026-10-27: FIRST actual nativePCO USCstorePASS on Y5, after MMU_INIA/INIB mappings. Output+UFO+1023guards+cleanup; HWRbaseline1unchanged. This clears selected compute execution/readback gate, not3D/fullprofile. See gpu-native-usc-store-pass.json.

## Co rzeczywiscie dziala

- Native DDK1.9/FW, CONNECT, kontekst pamieci i sterty, ograniczony PMR/CPU pattern/GPU VA mapping i cleanup.
- Sync block SET i niezalezny odczyt po poprawnym lancuchu lokalnego importu.
- Transfer-context CREATE/DESTROY, bez transfer payload.
- Native compute-context CREATE/DESTROY exact1.9 naY5 PASS (bridge129/0,1,disabled framework16B), cleanupRC0/FirmwareOK/HWR0/CRR0; bezKICKCDM/shadera. Dowod gpu-compute-context-lifecycle.json.
- **Wykonanie komendy kolejki firmware**: RGXKickSync138/2, NULL command z jednym UFO update; telefon obserwuje12345678->87654321, bez CPU SET drugiej wartosci, Driver/FirmwareOK,HWR0,CRR0. Nie jest to zadanie USC ani render3D.
- Pelna biblioteka Mesa buduje sie na x86_64. Telefon wykonuje natywny standalone z dokladnym adapterem, nie cala biblioteke Mesa.
- Offline PCO normal shaders (internal=false) + dokladne allocatory metadanych z pinnedMesa: compute128B, VS176B, FS440B przy syntetycznych wartosciach capacity.Historyczny sweep25/30success/stage zawieral INVALIDdepth16(unsignedunderflow); depth16/32 wykluczone wpozniejszym audycie. Pozostale wyniki nadal syntetyczne. **NIE profil sprzetowy, NIE walidacja ISA na telefonie; nie wysylac tego kodu na GPU.**

Dowody: phone/hardware/gpu-firmware-null-command.json, mesa-*-validation.json, pco-normal-shader-sensitivity.json. Patche/kolejnosc: phone/mesa/README.md. Ostatnia wdrozona konfiguracja kernela nie wymaga nowego boot tylko do prac userspace.

## Blokady potwierdzone, a nie hipotezy

| Obszar | Potwierdzony brak / roznica | Warunek przejscia |
|---|---|---|
| Profil |88pol wMesa structfeatures,14referencyjnieznanych,74UNKNOWN; nie wszystkie wymagane przez kazdy shader | Macierz pochodzenia dla wszystkich pol uzytych przez wybrana sciezke, bez traktowania UNKNOWN jako false |
| Rejestry USC | Realny PCO bez uzupelnienia capacity zatrzymuje sie na unified_store_depth, potem usc_slots | Odtworzone rzeczywiste limity i ograniczenia work-around, albo niezaleznie udowodniona zgodna sciezka ograniczonego testu |
| Interpolacja | PCO odczytuje usc_itrsmp_enhanced; normal FS moze dodawac obsluge pozycji/helper invocation | Zweryfikowac wybrane instrukcje i ta ceche, nie kopiowac GE8300 |
| Stare errata | DDK22.49.21.16 ma60084/61450/63027/65273/63553 iERN36400/42606/57596 | Rozpisac skutki dla pamieci, shaderow i commandstream; numery nie sa zamienne z nowymi flagami Mesy |
| Heap | OldHEAPDETAILS nie ma reserved_size; Mesa wykorzystuje carveout_size dla staticdata | Jawna polityka wlasnej rezerwacji VA klienta, zgodna z rozmiarem/offsetami staticprograms, test kolizji; nie udawac odczytu0 zkernela |
| Submission | ModernTQsubmit3 jest woldABIsetpriority3; oldsubmit2, inny uklad/timeline | Translacja pol i semantyki z exact generated1.9, nie memcpy ani sam numer |
| Native library | Brak kompletnej biblioteki/ICD AlpineARM64 i pelnego winsys | Uruchomienie rzeczywistej Mesy na telefonie; hostlink/smoke nie wystarczy |
| Display | simpledrm + test physicalflip nie daje wymaganego KMS/GEM/fences | Wlasciwy sterownik prezentacji z przewidywalnym przejmowaniem/przywracaniem pipeline |

Dokumentacja upstreamPowerVR nie wymienia naszego BVNC; nie jest gotowym wspieranym targetem. https://docs.mesa3d.org/drivers/powervr.html (sprawdzone wsesji). Profil GE8300 nie jest zamiennikiem.

## Kolejnosc najblizszej implementacji

### 1. Ustalic profil i znaczenie danych kompilatora

- Rozszerzyc gpu-profile-gap-audit o transitive helpers i derivedfeatures; obecny audyt nie dowodzi pelnej osiagalnosci.
- Publiczne generatedKMconfig/core: oddzielic cechy/braki tabel KM od pelnej listy UM. Nieobecnosc define wKM nie dowodzi braku funkcjiUM.
- Prywatna statyczna analiza stock USC/GLES/services dozwolona jako odniesienie; nie uruchamiac bibliotek, nie publikowac binary/decomp.
- Dotychczas: stockGLES ustawiaUNIFLEXparameter word0x16=0xf8, USC kopiuje go dostate+0x30 iogranicza<=0x100. To budzet temporaries perprogram; **nie dowodzi unified_store_depth ani usc_slots**. TileBufferSize constant12288B nie dowodzi commonstore.
- Znormalizowac dane z dowodem: pole, wartosc/jednostka, adres/symbol lub publicsourcecommit, pewnosc, konsumenci. Pozostawic explicit unknown.
- Weryfikowac zwykle shadery, nie mylic helper/internal z entrypointem zadania. Zapisywac NIR/PCO/ISA i liczby temp/shared/coeff/vtxin. Nie usuwac asercji, by uzyskac pozornyPASS.

### 2. Pierwszy rzeczywisty shader: minimalny compute store

To droga do sprawdzenia USC bez calego TA/PBE, nie substytut3D.

- Exact1.9 compute-context i kickCDM ABI plus framework; porownac firmware structs z nowym commandstream/PDS.
- PMR4K: sentinel, jawnie wybrana VA, WRITE_COMBINE dlaCPU i prawidlowe cache GPU, granice/ochrona; osobne bufory koduUSC/PDS i danych.
- PDS parametry z rzeczywistego shaderdata, poprawny CDM stream i jawne zakonczenie zadania. Bez niezweryfikowanego blobu z innego BVNC.
- Jeden workgroup/jeden invocation, zapis znanej32bit wartosci, UFO update po zadaniu. Timeout i zatrzymanie kolejnych prob przy HWR/CRR lub bledzie.
- PASS wymaga zmienionego bufora przez shader i sygnalu zakonczenia. Sam sukces ioctl lub NULLcommand nie zalicza shader execution.

### 3. Pierwszy trojkat poza ekranem

- Przetlumaczyc RenderContext, free-listy local/global i ich backingPMR, HWRT dataset, wymagane bufory PM/TA/ISP. Nie powielac nowoczesnych rozmiarow struktur.
- Zbudowac zgodne VS/FS, PDS/VDM/PPP i TA/3D firmwarepacket; dodac EOT/PBE z prawidlowym formatem i geometriatile.
- Maly linearRGBA target, beztekstur/MSAA/depth na start, jesli exactDDK dopuszcza takieustawienia. Najpierw wyczyscic sentinel.
- Odczyt po zakonczeniu: kolor tla, kolor wewnatrz trojkata i spodziewany obszar pokrycia. Zapisac obraz i CPUoracle/tolerancje krawedzi. Powtorzyc z innymi wierzcholkami/kolorem.
- Potem tekstura, depth, blend i krotka sekwencja obracajacego sie szescianu; kontrola wyciekow/cleanup, bez udawanej konformancji.

### 4. Pelna natywna Mesa/Alpine

- Dokonczyc wymagane przez winsys heap/staticcarveout, cacheops, fences/timelines, import/export i job ABI. DiagnosticCREATE zNULLextraPMR nie oznacza pelnej zgodnosci semantyki1.17.
- Zbudowac aarch64/musl ICD/library i narzedzia. Osobne locale/font/Wayland/EGL/Vulkan deps, bez vendorCPUruntime.
- `vulkaninfo` ma pokazywac fizyczneGPU, nie lavapipe; potem aplikacjaoffscreen z porownaniem pikseli i wybrane testy Vulkan. Nie wlaczac unsupported extensions by ominac walidacje.
- Mierzyc powtarzane klatki i poprawna synchronizacje; dopiero wtedy benchmark, energia i czestotliwosci.

### 5. Ekran i zero-copy

- Wlasciwy DRM/KMS z GEM, mode/plane, vblank/EOF, pageflip i fences na zmierzonym OVL3/RDMA/continuousDSI. Nie wracac do cyklicznego DSIkick25Hz.
- Candidate: displayGEM exports DMA-BUF ->GPUimport. Stary GPUexport attach obecnieENOSYS; nie usuwac guardu bez pelnej implementacji.
- Sprawdzic format/pitch/alignment/modifiers, SG/DMA/IOMMU, cache i czas zycia obu stron. Dowodwspolnegobufferu, bez CPUmemcpy na prezentacje.
- Testy aktualizacji przyVSYNC, brakstarychfragmentow/tearing, recoverydoznanegoframebufferu; ownerphysicalfeedback oddzielnie odlogPASS.

### 6. Akcelerowany desktop i aplikacje

- Wybrac renderer kompozytora po sprawdzeniu wymaganych Vulkanfeatures; alternatywnie GLES przezZink po porownaniu z profilem wymagan. Zink nie gwarantuje GLES tylko dlatego, ze pierwsza komendaVulkan dziala. https://docs.mesa3d.org/drivers/zink.html
- Obecny Cairo UI rasteruje naCPU. GPUkompozytor przyspieszy skladanie, ale nie zamieni automatycznie kazdego Cairodraw wGPU. Przeniesc animacje, zaokraglenia, blur/refraction na renderer C/C++GPU; cache tekstu/ikon jako tekstury.
- Rzeczywisty touch/gesty wkompozytorze, transformy obcychokien, damage i framepacing; nieograniczoneklatki60fpsbezpotrzeby niedozwolone.
- PMIC/VCORE i dotyk: dotychczasowy powertrial odpina provider uzywany przezGPU. Wspolny zserializowany interfejs lub kontrolowane rozwiazanie zaleznosci przed dalszym powertrial. Nie odpinac wciemno.

### 7. Kodeki jako osobna sciezka

- VDEC/VENC/JPEG to osobne silniki. ExactMT6739 stock ioctl/register/clock/reset/firmware responsibilities; zmapowac na natywne V4L2M2M/odpowiednieAPI dopiero po wykazaniu zgodnosci.
- H264 iHEVC testowac osobno: znany prosty8-bit clip, porownaniezdekodowanychklatek, pozniej inneprofile/rozdzielczosci. Nie zakladac10bit/HEVCMain10 ani enkoderaHEVC.
- Dopiero pozniej wspolneDMABUF/decode->GPU/display. Sukces renderowania nie dowodzi dekodowania.

## Zasady pracy nocnej i checkpoint

Heartbeat microfw-nocna-praca-gpu jest ACTIVE:20min/24wznowienia, skonfigurowany na zadanie nocne. Bez duplikatow. Postep zapisywac po przełomach wNOTES i oddzielac measured/offline/reference/hypothesis. Powiadamiac tylko istotnie. Stala zgoda update/restart istnieje; nadal tylko boot/rootfs, bezsecrets/remotes/NVRAM/protectedpartitions. AktywnyGPUkexec niezweryfikowany. Przy braku danychGPU kontynuowac audytkodekow lub bezpieczny GPS/hardware, nie powtarzac bezcelowo tych samych smoke-testow.


## 2026-10-27: profil — sprawdzone slepe tropy
Live debugfs version ponownie identyfikuje22.49.21.16, lecz inventory nie oferuje oddzielnego unifieddepth/uscslots getter. debug_dump uruchamia DEBUG_REQUEST_VERBOSITY_MAX; nie traktowac go jako prostego cached-profile API ani powtarzac bez konkretnego celu. CallerparamsUSC, tilebuffer12KiB, COMMON64/14 i per-task TEMPS nie stanowia physicalcapacity. Kolejny shader pozostaje zaleznym od udowodnienia wybranej sciezki ISA/errat oraz rzeczywistych limitow; NULL/lifecyclePASS tego nie zastepuja.

## Restricted compute profile audit
Lexical PCO source scan (comments/uscgen excluded) identifies four query sites: two RA helpers, interpolation usc_itrsmp_enhanced and texture quirk74056. See gpu-pco-query-sites.json. This narrows the review; it is not whole-program reachability or hardware validation. Prove which helper paths are unreachable for the selected no-texture/no-interpolation WG1 shader, then validate its instruction/errata and allocation contract without pretending unknown features arefalse.

## First compute PDS input contract
Offline final grouped IR for the normal store shader reads vi0 as local invocation index, emits conditional-execution control inr4, and requires5temps/1vtxin. Single invocation must receive vi0=0 through validated PDS setup; an undefined input can suppress or misexecute the store. Store uses writethrough+DRC0/WDF, which still requires target cache/errata/completion validation. See gpu-restricted-compute-ir-audit.json.

## Compute PDS candidate generated
Pinned public generator produced localX->vi0+DOUTU candidate (3dataDWORDS/20codeBYTES=5instructions), with consistent metadata, return pointer and canaries. Harness sanitizerPASS, SLCfeature0/1 sameoutput. See gpu-compute-pds-input-candidate.json. This supplies an offline native source path, not physical CDM/ISA validation; no program uploaded. Keep exact legacy context/packet and allocation gates before dispatch.

## Historical public firmware layout correlation
Pinned android.googlesource reference independently produces the observed120B shape and seven context words. Candidate runtime names are recorded in gpu-public-legacy-compute-layout.json with sourceURL/hash and extracted licensed C layout test. This is not exact1.9 proof: validate feature branches, register masks and unresolved TPU_ARRAYS candidate atpacket48 before a builder/kick.

## Exact firmware debug types available
The actual loaded rgx.fw.signed contains fullDWARF and4001symbols. Exactcompute types now confirm120B with56B runtime/56B context/flags/deadline. The historical public C declarations match these actual types; optional branch ambiguity for this packet shape is resolved. See gpu-exact-firmware-compute-dwarf.json and updated packetmap. Consumer microMIPS maps registers; PDS_CTRL is RMW. Offset48 is TPU_ARRAYS_CDM but its use must be distinguished from the observed unwritten host bytes. Continue exact consumer/context-program audit; profile/ISA and safe memory completion remain unproven. Privateblob stays outside repo.

## Context PDS contract correction and comparison
The stock context helper does emit twoDOUTW pointers beforeDOUTU; prior different-contract conclusion came from missing decompiler call arguments. Complete public generator output16dataDWORDS/6codeDWORDS matches old expressions for store/load at corresponding encoding. See gpu-pds-context-pair-comparison.json. This removes the PDS pointer-contract discrepancy, while USC context program ISA/errata and target capacity are still separate gates.


### 2026-10-27: zasoby TA/3D
Rendercontext i chroniona para global/local freelist (32 strony kazda) CREATE/DESTROY na Y5 PASS, bez nowego HWR. Nastepne: RenderTarget i HWRT lifecycle, potem VS/FS/PDS/VDM oraz offscreen pixel readback. Dotychczasowy sukces to shader compute, nie trojkat. Dokladna kolejnosc list HWRT LOCAL0/GLOBAL1, count2.

HWRT lifecycle na Y5 PASS (130/0,1;98B/32B), ale konfiguracja diagnostyczna ma zerowe rejestry geometrii i tylko wlasne strony4K. Przed TA wyliczyc rzeczywiste VHeap/MList/TPC oraz register geometry; nie traktowac lifecycle jako gotowego render targetu.


### Cel wlasciciela: Wayland i WebGL2
Kontynuowac natywny render3D do odczytu pikseli; dalej rzeczywisty ARM64Mesa/winsys, akcelerowanykompozytor z prezentacjaDMA-BUF/KMS, natywneGLES3 i WebGL2 wprzegladarce. Kazda bramka wymaga osobnego dowodu wykonania naGPU. FinalVSma5outputs przezPSIZE; FSdiscard wymagaPUNCH_THROUGH; nie uzywac wczesniejszego VDM4. Nocnaautomatyzacja wstrzymana na prosbe wlasciciela, bez wznawiania samodzielnie.

### Biezaca zaleznosc: lacznosc USB
Podczas transferu graphics64 bundle telefon zniknal zRNDIS; brakping/telnet, zadnegoGPUdrawnieuruchomiono. Wymagane zdjecieekranu i odzyskanieUSB; nie klasyfikowac jakoawariaGPU bezdowodu. Wznowienie:uptime/uname/GPU/logi/listener9500, ponownytransfer/8SHA, nativeStage, potemjednokontrolowaneKICKTA3D zpelnymiFWpacket/fences. Bundle/tableadresow i walidacje wgpu-graphics64-bundle-staging.json.
