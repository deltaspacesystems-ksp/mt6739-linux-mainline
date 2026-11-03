# v49 — wynik testu na urządzeniu, 2026-10-24

Właściciel wgrał wd_v49.img i potwierdził rozruch oraz widoczne reakcje wszystkich przycisków na konsoli. Ping USB: 2/2 odpowiedzi, <1 ms. Powłoka działa.

- uname: Linux microfw 7.3.0 #33 SMP PREEMPT Sat Oct 24 23:13:02 CEST 2026 aarch64.
- Sterownik y5-pmic-monitor: MT6357 ID=0x5721, rejestracja przy starcie ~0.4 s.
- /sys/class/power_supply/microfw-battery/voltage_now: 3849114, 3845819, 3845324 uV (kolejne świeże konwersje).
- status: Unknown, zgodnie z implementacją; ładowarka i gauge nie są jeszcze obsługiwane.
- /proc/bus/input/devices: microfw MT6357 power and volume keys; Handlers=kbd event0; /dev/input/event0 istnieje.
- RAM: 1647 MiB, użyte 28 MiB, dostępne 1602 MiB; swap 0.

## Kontrolowany test evdev
Uruchomiono statyczny y5_evtest z /tmp, tylko odczyt event0, bez EVIOCGRAB i bez wstrzykiwania zdarzeń. Właściciel nacisnął kolejno Power, głośność + i głośność − oraz potwierdził wykonanie testu.

| Klawisz | EV_KEY code | Naciśnięcie | Puszczenie | SYN_REPORT po obu |
|---|---:|---:|---:|---|
| Power | 116 | value=1, t=70884 ms | value=0, t=71164 ms | tak |
| Głośność + | 115 | value=1, t=73368 ms | value=0, t=73572 ms | tak |
| Głośność − | 114 | value=1, t=75096 ms | value=0, t=75256 ms | tak |

To potwierdza przepływ sprzęt -> sterownik -> input core -> evdev -> aplikacja. Krótkie testy nie weryfikują suspend/wakeup, długich przytrzymań, jednoczesnych kombinacji ani długotrwałego działania na baterii.

## Status obrazów
wd_v49.img zastępuje v48 jako bieżący obraz potwierdzony dla konsoli, rootfs/USB, napięcia baterii i trzech przycisków. v48 zachować jako awaryjny. SHA256 v49: 44a28e13ad9da185b745dc193657ceda79ca0290193579db479bb633d8c12c9e.

Nie uznawać tego za uruchomienie sterowania ładowaniem: nadal brak sprawdzonej temperatury, capacity, prądu baterii, FAN54005 i oszczędzania energii. Kolejne prace: odtworzyć tor termistora i parametry z 4.4, potem uruchomić komunikację I2C z ładowarką i odczyt identyfikacji/statusu przed konfiguracją.
