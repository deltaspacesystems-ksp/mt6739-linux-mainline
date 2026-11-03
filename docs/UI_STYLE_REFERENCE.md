# MICROFW — wzorzec UI od wlasciciela, 2026-10-26

Dziewiec przeslanych zdjec jest wzorcem stylu i przejsc. Zachowujemy poprzednie wymagania: C/C++, angielski, kompaktowa skala, desktop z oknami, fullscreen z mozliwoscia przywrocenia standardowych dekoracji Wayland.

- Photos1–6: czarne tlo Settings, grafitowe grupy z duzym promieniem, cienkie separatory odsuniete od ikon, stonowany tekst drugorzedny; male ciemne kafelki ikon z kolorowymi symbolami.
- Photos1–4: interaktywne przejscie poziome. Nowa strona podaza za palcem, poprzednia pozostaje widoczna z przesunieciem/paralaksa i przyciemnieniem. Zaokraglone krawedzie, ciagle wejscie/wyjscie, mozliwosc anulowania gestu.
- Photos5–6: nieruchomy polprzezroczysty naglowek i plywajaca kapsulka wyszukiwania; tresc przechodzi pod nimi.
- Photos7–9: przyciski na okraglym szkle, cienki jasny dolny uchwyt; karta przy gescie deformuje/przemieszcza obraz tla i ma subtelny jasny/rainbow rim. Ze zdjec nie da sie wyznaczyc czasu ani krzywej animacji — dobieramy i sprawdzamy na telefonie.

Kolejnosc: (1) statyczne kolory/karty/typografia, (2) model przejscia i przewijanie/gest anulowalny, (3) frame-callback + czas monotoniczny/spring i damage, (4) cached/downsampled backdrop blur dla wlasnych powierzchni, (5) refrakcja i dynamiczne efekty kompozytora po dzialajacym GPU. Klient nie moze sam probkowac obcych okien ani przechwytywac ich dotyku. Nie reklamowac samej przezroczystosci jako prawdziwego blur.

Wdrozone w kodzie naPC: settings grouped cards/czarne tlo i neutralne glass highlights. NativeC/Cairo, build-Werror PASS, podglad720x1440 sprawdzony wzrokowo; demo pokazuje dane przykladowe. Nie wdrozono na telefon. Bez animacji, blur i refrakcji na tym etapie. Podglad lokalny D:/y5_linux_work/out/settings-reference-style.png.


## 2026-10-26 — pierwszy silnik animacji C

Wlasciciel chce piekne animacje wzorowane na iPhone. Dodano motion.c/h: znormalizowana krytycznie tlumiona krzywa420ms (bez arbitralnego skoku ostatniej klatki), slide/paralaksa20%, dim24%, promien karty plynnie0->28->0. Monotoniczny zegar, stany koncowe renderowane do cache raz przy zmianie strony, nastepne klatki Cairo kompozytuja obrazy. Retarget pobiera biezacy obraz, nie skacze do poprzedniego endpointu. Cache zwalniane po zakonczeniu/resize/shutdown; maks4Mpix/obraz.
Integracja: klient xdg-shell i tlo/overview/control desktop layer; kolejne klatki tylko przy aktywnej animacji i po framecallback, brak idle60fps. Nie animuje jeszcze obcych okien ani nie sledzi palca przez cala droge gestu. Launch Settings jako osobnyclient nie ma jeszcze animacji compositorowej. Blur/refrakcja tez nadal otwarte.
Test exact motion ASan/UBSan: firstframe, retargetpixelcontinuity, endpoint, resize, powtorzonecleanup PASS. Build-Werror, gesty/C++ABI i realWeston/labwc protocolregression PASS; powtorzony motiontest/build po dodaniu zaokraglen PASS. Nie wdrozono na telefon, FPS/plynnosc/hardware niezmierzone.


## Decyzja wlasciciela — styl UI macOS 27

Aktualny kierunek stylistyczny MICROFW: **macOS 27**, zgodnie z poleceniem wlasciciela. Zastepuje wczesniejszy ogolny cel kopiowania interfejsu iPhone/iOS. Zachowujemy natywny desktop z oknami, runtime C/C++, angielski interfejs, kompaktowa skale, gesty i multitouch dostosowane do telefonu. Wczesniejsze zdjecia sa referencja wybranych efektow szkla/ruchu; przy konflikcie priorytet ma nowa decyzja o stylu macOS 27. Nie dopowiadano szczegolow wygladu konkretnej wersji bez referencji. To zapis preferencji, nie deklaracja gotowego wdrozenia.
