# V54, desktop bez migotania i pierwsze testy dotyku

2026-10-25. Właściciel wrócił do telefonu i potwierdził „Desktop widoczny, bez migotania” podczas testu nowej binarki UI przy y5disp/hz=0. Wcześniej zmieniono jednocześnie repaint UI i tryb odświeżania; nie przypisywać poprawy wyłącznie jednej zmianie. Brak oceny animacji pod obciążeniem, GPU i fizycznego dotyku.

## Zapis i start v54 — potwierdzone

Właściciel jawnie zatwierdził zapis v54 i restart. mfw_update_boot.py wykonał ponownie prywatny backup24MiB, walidację celu boot/mmcblk0p36, zapis8802304B, pełny odczyt zwrotny/SHA256 PASS i dopiero potem reboot. Nie zapisano innych partycji poza autoryzowanym rootfs/p36. Backup pozostaje prywatnie na PC, nie w repo.

SHA256 obrazu: 146815f0426e2f980762fb287ed1131d66ba1d66262e9c3d835c48226876ab00. Kernel Linux7.3.0 #40, build Sun Oct25 14:40:05 CET2026. /dev/uinput jest obecne. Nowe symbole INPUT_MISC/UINPUT/I2C_CHARDEV nie tworzą automatycznie adaptera I2C ani sterownika dotyku.

Pierwsze16s: kicks0/errors3/last_error-34/profile_ok0 — faza startowa. Przy73s: kicks7/errors3/last_error0/profile_ok1, Charging,30.7C. To poprawne przejście do działania guardu po fazie startowej, nie utrzymująca się awaria ładowania. Reboot zresetował wall clock do1970; mfw_sync_time.py --set ustawił UTC z PC, odczyt zwrotny różnica-2s. /dev/shm zamontowane automatycznie z rc.sh. Desktop ponownie uruchomiony na v54, trzy warstwy FRAME_DONE; to dodatkowy dowód transportu, potwierdzenie wizualne bez flickeru pochodzi z testu przed restartem.

## VLDO28 — ograniczony test enable/restore PASS

Native C y5_vtouch_pulse.c: wyłączność PWRAP przez krótki unbind monitoringu z trap/rebind, sprawdzone ID PMIC0x5721, selector1c24&300=100 (2.8V), enable1b32=0, reset GPIO119 już outputLOW i muxGPIO. Bez zapisów selectorów napięcia. Jedyny PMIC data write:1b32=1,150ms,odczyt,1b32=0,odczyt.

Wynik: before0000,enabled0001,restored0000,result0,restore0. To dowód sterowania i odczytu bitu enable; nie pomiar napięcia miernikiem, nie dowód pracy kontrolera dotyku. Pierwsza kompilacja strictC11 nie miała featuremacro POSIX; dodano _POSIX_C_SOURCE200809L i kompilacja -Wall/-Wextra/-Werror PASS. Błąd kompilacji nie wykonał żadnej operacji sprzętowej.

## Pierwszy odczyt ID pod0x5d — bez odpowiedzi

y5_touch_id_probe.c: sprawdzenie wyłączności zasobów, niegated I2C0 i pusty FIFO/idle, VLDO28 2.8V selector bez zmiany. Krótka sekwencja resetu oparta na stock Goodix: INT6 low, RST119 high po opóźnieniach, następnie INTinput; wysłano wyłącznie indeks8140 i żądanie6B odczytu. Brak zapisu danych konfiguracji/firmware/watchdogRAM kontrolera.

Preflight INT6 mux0/dir0/out0; RST119LOW. I2C0 zwrócił status0002,rx_count1,start1; odczytu nie zaakceptowano. data000000000000 to wyzerowany bufor błędu, NIE identyfikator czujnika! Wynik-1. Cleanup przywrócił resetLOW/INT i wyłączył VLDO28; restore0,enable_after0. Wrapper set-e zakończył sięrc4 i trap przywrócił monitor PMIC.

Odczyt po teście: GPIO6input/high,out0;119output/low. I2C0 pierwotny slave007c/control0028/txlen2/transac1/timing0216/extconf8001/aux0/div4 wrócił; FIFOcount0. Status później0001, więc nie utożsamiać wcześniejszego start1 z trwałym zawieszeniem magistrali. Kontroler nie ma sterownika i nie był resetowany. Charging po40s od rebind: kicks6/errors0/last_error0/profile_ok1/temp30.8C; później188s: kicks24/errors0/faultseen0/profileok1/Charging/30.9C.

Unbind/rebind resetuje liczniki/nową sesję guardu i numerację evdev PMIC. Nie mylić tego z reboot ani porównywać liczników sprzed/po jako ciągłej serii.

## Korekta hipotezy: identyfikacja modelu nadal otwarta

Live DT touch/ts_kit zawiera goodix_ts@5d, focaltech@38 i mstar_ts@26. Wcześniejszy zapis aktywnego klienta hw-ts pod0x5d opisuje framework Huawei; bez logu detect nie dowodzi producenta fizycznego kontrolera. Brak ACK próby Goodix nie rozstrzyga między błędem zasilania/resetu/transportu a innym kontrolerem. Nie wykonywano pełnego skanowania I2C, nie wysyłano komend innych producentów i nie zmieniano firmware.

Na prośbę właściciela wracamy do dokładnej ścieżki wykrywania w4.4: tpd_probe,ts_thread,huawei_ts_chip_register,init poszczególnych dostawców oraz rzeczywisty format ts_i2c_read. Następny krok to porównanie ich z DT i ewentualnymi zapisanymi dowodami detect z Androida; bez przełączania telefonu na4.4.

## Analiza4.4 i przełom MStar

Nowe dekompilacje k44_tskit_detect.c,k44_touch_vendors.c,k44_mstar_detect.c,k44_mstar_transport.c,k44_mstar_reports.c,k44_mstar_irq.c. huawei_ts_chip_register przyjmuje dostawcę po powodzeniu callbacku detect i blokuje następne rejestracje flagą. Focal i Goodix zmieniają client->addr, a mstar_iic_write_data ustawia go przy każdym wywołaniu. Nazwa pierwotnego klienta frameworku0-005d nie jest zatem pomiarem adresu wybranego dostawcy. Nie wyciągać kolejności initcalli wyłącznie z adresów funkcji.

Stock tpd_probe przypisuje:98000f8=state_rst_output1,9800108=state_rst_output0,9800120=state_eint_as_int,9800110=state_eint_output1,9800118=state_eint_output0; supply=vtouch. To potwierdza wcześniejsze znaczenie helpera tpd_gpio_output.

Live DT warianty: gtp/GT917D,addr5d,chip_id87; fts,addr38,chip_id54,ic_type3; mstp/MSG2836A,addr26,chip_idbf. Każdy720x1440. Nadal nie są to odczyty rzeczywistego chip ID.

Dokładny Focal detect w4.4:2.8V,100ms, resetHIGH10/LOW20/HIGH240ms (240 z bajtu2 tablicy8c39ca8), read1B rega3 pod38. Natywny test tej ścieżki: I2Cstatus0002,rxcount0,result-1,restore0,enable_after0. Nie otrzymano ID. Nie wykonywano bootloader/flash/update.

Test kontrolny magistrali BEZ resetów/zasilania: pojedynczy odczyt NT50358A pod3e reg0, status0001,count1,value0f,valid1. Przywrócono rejestry hosta. To potwierdza małe transakcje PIO/repeatedSTART na I2C0 i izoluje wcześniejsze NACK od całkowicie niedziałającej magistrali. Nie zmieniono bias ekranu.

MStar4.4 detect: power2.8V,50ms, resetHIGH10/LOW10/HIGH25ms, jeden bajt37 pod adresem62. Adres62 pochodzi ze stałej32bit pod94ae4e4, a helper zapisuje go bez przesuwania do client->addr (7bit). To komenda wykrywania/reshape, nie flashowanie. Natywny test dokładnie tego przebiegu: RST119dir1/out1/in1,status0001,FIFOcount0,start0,result0,restore0,enable_after0. Jest ACK zgodny ze stockową ścieżką MStar; nie jest to pomiar modelu układu.

Nie uruchamiano mstar_get_chip_type ani pełnego init_chip: pierwsza funkcja zatrzymuje MCU i wykonuje wiele zapisów sterujących interfejsem flash, druga czyta dane firmware/flash. Co więcej get_chip_type ma gałęzie podstawiającebf dla innych/nieznanych wartości, więc samo raportowanebf nie musi być surowym ID. Zostawiamy je poza pierwszym bring-up.

Normalny raport stocku: adres26 ze stałej94ae4e0, pojedynczyREAD bez prefiksu, demo43B (2b), nagłówek5a,10slotów po4B, checksum modulo256 całego pakietu=0. x=((p0&f0)<<4)|p1,y=((p0&0f)<<8)|p2,pressure=p3,slotp0/p1/p2=ff oznacza brak kontaktu; stock skaluje do wymiarów przez mnożenie i >>11. Mainline/generic Goodix lub Focal parser nie pasuje.

Pierwsza próba normalnego odczytu: adres26 odpowiada, PIO opróżniany w trakcie transferu otrzymał43B,status0001,header00,sum0. Pakiet ODRZUCONY, bez generowania zdarzeń; sama zgodność checksum z zerem nie wystarcza. Nie było pełnego dumpu danych tej pierwszej próby, więc nie twierdzić, że wszystkie43B były zerowe. INT6 przed odczytemlow. Regulator i GPIO przywrócone. Przygotowano wersję z5ograniczonymi próbami i wydrukiem liczby niezerowych bajtów/prefiksu; oczekuje na ruch palca właściciela. Nie ma jeszcze działającego dotyku w Waylandzie.

## Fizyczny dotyk w Waylandzie — POTWIERDZONY, ekran pod obciążeniem niepoprawny

Właściciel przesuwał palcem. Poprawny pakiet:status0001,43B,header5a,sum0,niezerowych43B,prefiks5a24be0206ffffffffffffff. Slot0 raw702/1026,pressure6. Parser akceptuje tylko pełne43B,5a,poprawnychecksum i współrzędne do2047; skaluje720x1440. Odrzuca skrócone, błędne, niewłaściwy nagłówek i zakres. Native selftest obejmuje przechwyconą próbkę, pustą ramkę i slot9 na końcu zakresu; PASS.

Natywny C mfw_mstar_touch.c tworzy uinput z INPUT_PROP_DIRECT,10slotówMT,pozycjami,naciskiem iBTN_TOUCH. Rootfs nie ma udevd/udevadm, więc wrapper tworzy wyłącznie tymczasowy runtimeDB/tagseat dla NOWEGO naszego input; niczego istniejącego nie nadpisuje, usuwa własne wpisy przy zakończeniu. labwc trzeba uruchomić po utworzeniu urządzenia, bo nie ma demona dostarczającego hotplugudev. Log kompozytora potwierdził MICROFW MStar Touchscreen event1, a właściciel: „Reaguje i trafia poprawnie”. To pierwszy potwierdzony dotyk fizyczny w nowym DE.

Power helper y5_mstar_touch_power.c osobno --on/--off, wyłącznie przy unbound PWRAP. Po weryfikacji dokładnego stanu początkowego tworzy root-only lease /run/mfw-touch.lock/active, umożliwiający cleanup także po przerwaniu startu. Monitor PMIC jest REBOUND przed uruchomieniem readera i pracuje przez cały180s test. Czytnik nie mapuje PWRAP. Na końcu zwalnia wszystkie kontakty, niszczy uinput, wrapper usuwa tylko własny DB/tag, wyłącza zasilanie/resetujeGPIO przez krótki unbind i natychmiast rebind. Brak autostartu i kolejnych zapisów boot.

Test180s zakończył sięEXIT0,good306,bad14691,TOUCH_POWER_OFF/stan początkowy przywrócony. Ładowanie57s później:kicks8/errors0/faultseen0/last_error0/profileok1,temp29.7C. Dużo odrzuconych pakietów to OTWARTY problem odpytywania/IRQ/gotowości danych, nie „stabilny sterownik”. Pierwszy reader wymuszał finger-up po5błędnych pakietach; może to powodować ponowne tapnięcia przy nadal przyłożonym palcu. Usunięto to: błędny pakiet nie tworzy zdarzeń; po2s trwałego błędu przy aktywnym kontakcie reader kończy się jeden raz i sprząta. Poprawka kompiluje się/selftest PASS, ale nie była jeszcze powtórzona fizycznie.

Właściciel zgłosił częściowy flicker i dziwny obraz podczas interakcji, doprecyzował: rozdarcia podczas ruchu ORAZ pozostające stare fragmenty. To nie unieważnia poprawnego trafiania dotyku, ale UI NIE jest wizualnie gotowe. Wcześniejszy „bez migotania” dotyczył statycznego desktopu. 25Hz jest nadal WYŁĄCZONE. Brak błędów transportu frame callbacks nie dowodzi spójności obrazu na panelu. Podejrzenie: kopiowanie simpledrm do aktywnego bufora LK i niezależny bootstrapDSI (min40ms) bez synchronizacji, przy nominalnym trybie kompozytora60Hz. Hipoteza, nie pomiar przyczyny. W logu były też brakujące ikonki foot/labwc; nie dowodzi to awariiGPU.

Najnowsza dyspozycja właściciela: zacząć właściwy sterownikGPU i zero-copy display. Priorytet przechodzi na DRM/RDMA/DSI, wspólnebufory i synchronizację, z oddzielnym sprawdzeniem zasilania/ID/firmware PowerVR. Nie nazywać obecnego mostu dotyku ani simpledrm finalnym sterownikiem. Kolejny kernel wymaga osobnego testu i zgody na zapis według zasady9.
