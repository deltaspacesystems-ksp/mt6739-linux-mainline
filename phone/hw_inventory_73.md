# Huawei Y5 2018 (DURA-L21, MT6739) - inwentarz sprzetu pod kernel 7.3

Zrodlo: dzialajacy telefon na kernelu 4.4 (2026-10-23). "Zaladowany" = sterownik kernela 4.4 podpial sie do klienta I2C
(/sys/bus/i2c/devices/*/driver). Pozostale wezly z drzewa urzadzenia (inne akcelerometry, zyroskop, magnetometr, barometr, NFC,
rt9458) NIE sa podpiete = najpewniej nie ma ich na tej sztuce. Kolumna "mainline" to moja wiedza z pamieci - DO SPRAWDZENIA w drzewie 7.3.

| uklad (4.4) | magistrala/adres | funkcja | co jest w mainline (do weryfikacji) |
|---|---|---|---|
| MC3XXX (DT: mc3410-ontim) | i2c-1 0x4c | akcelerometr (jedyny aktywny) | iio `mc3230` (inna rodzina?); w razie braku maly sterownik IIO |
| ltr577_alsps | i2c-1 0x53 | swiatlo + zblizeniowy | iio `ltr501` (LTR-5xx), sprawdzic czy obejmuje LTR-577 |
| sx9320 | i2c-1 0x28 | czujnik SAR/zblizeniowy (antena) | iio `sx9310`/`sx9324`, SX9320 pokrewny - sprawdzic |
| flashlights-aw3643 (strobe_main) | i2c-1 0x63 + GPIO hwen/strobe | dwukanalowy flash LED (torch-light0/1) | brak znanego sterownika AW3643 - maly sterownik LED/flash (I2C + GPIO) |
| hw-ts (DT cap_touch) | i2c-0 0x5d | dotyk (Huawei ts_kit, prawdopodobnie Goodix GT9xx) | `goodix` jesli to GT9xx - potwierdzic chip (log ts_kit) |
| NT50358A | i2c-0 0x3e | bias LCD | regulator/panel - czesc inicjalizacji wyswietlacza |
| MAINAF, kd_camera_hw*, CAM_CAL | i2c-2 0x18/0x41/0x43/0x50 | aparat tyl/przod (AF, EEPROM) | sensory kamery: osobny, duzy temat |
| HL7005D (stockowy sterownik `fan54005`) | i2c-5 0x6a | ladowarka | ID rejestru3=0x41, stock mapuje do HL7005D; nie zakladac zgodnosci wszystkich bitow FAN54005 |
| led_simple (mt65xx_leds) | GPIO (pinctrl led_output0/1) | diody czerwona/zielona z przodu | `leds-gpio` - wystarczy znac numery GPIO z pinctrl |
| brak (nie podpiete) | - | zyroskop, magnetometr, barometr, NFC | n/d na tej sztuce => **"IMU" to tylko akcelerometr MC3410** |
| MT6357: bateria / fuel gauge | PWRAP + AUXADC | napiecie i temperatura | v50: swieze VBAT i NTC; gauge, procent i prad jeszcze nieobslugiwane |
| GPS | connsys SoC | - | anteny odlaczone (notatki) - pomijamy |

W kernelu 4.4 nie ma /dev/i2c-* (brak CONFIG_I2C_CHARDEV) - w 7.3 wlaczyc `CONFIG_I2C_CHARDEV=y` do diagnostyki (i2cdetect/i2cget).

## Zasadnicza uwaga o flashu
Flash (AW3643) nie dziala na 4.4 z powodu `flashlight_core`: na kazde wlaczenie logi mowia
`power saving other value, no respond flashlight_0/1`. To logika MediaTek zwiazana ze stanem baterii (u nas capacity=-1, brak demona
fuelgauged) - hipoteza do sprawdzenia, nie potwierdzona. Na kernelu 7.3 sterownik flasha bedzie wlasny, wiec tego ograniczenia nie bedzie.

## Proponowana kolejnosc prac na kernelu 7.3 (kazdy etap = nowy obraz boot, odzysk BROM, backup gotowy)
1. Bootowalny obraz: watchdog + rezerwacje pamieci + simplefb (patrz kernel73_boot_notes.md) -> widac log na ekranie.
2. USB gadget (RNDIS) + sshd w ramdisk -> zdalny dostep (po tym wszystko moge robic z chmury jak teraz).
3. eMMC + rootfs z naszego Alpine (userdata_rootfs.tar.gz z backupu).
4. Latwe: diody GPIO (leds-gpio), akcelerometr / swiatlo / zblizeniowy (IIO), czujnik SAR, flash AW3643.
5. Dotyk (potwierdzic chip, `goodix`) - bez tego UI jest bezuzyteczne, wiec to krytyczne.
6. Wyswietlacz/podswietlenie poza simplefb (brak sterownika DISP MT6739 w mainline - prawdopodobnie zostaje simplefb + wlasna kontrola jasnosci).
7. Wi-Fi/BT (connsys WMT) - najtrudniejsze, brak odpowiednika w mainline (sterownik 4.4 jest modulem na 4.4).
8. Modem - osobny, otwarty temat (HS2_FAIL), nie blokuje reszty.

Aktualizacja 2026-10-25: v50 z VBAT/NTC potwierdzony. Kandydat mfw_v51 dodaje stan HL7005D i read-only zegar CPU (~950 MHz odczytane z PLL); CPUFreq/DVFS nadal brak. Szczegoly: docs/MFW_V51_UPDATES.md.
