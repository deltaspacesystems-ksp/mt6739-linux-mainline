# Klucze mostu i sterowania PC jako zmienne środowiskowe

Ten poradnik opisuje, jak wygenerować klucze, gdzie je wpisać (PC z mostem, nginx, środowisko chmurowe Claude) i jak sprawdzić,
że wszystko działa. **Żadnego z tych kluczy nie wolno wklejać do czatu ani commitować do repozytorium** (`.gitignore` ich pilnuje).

## 1. Jakie są klucze

| Zmienna | Do czego | Gdzie jest używana |
|---|---|---|
| `BRIDGE_TOKEN` | Bearer do ścieżki telefonu (`/y5-<losowy>/`) | most na PC, **nginx**, klient w chmurze |
| `BRIDGE_SIGN_KEY` | podpis HMAC żądań do telefonu | most na PC i klient w chmurze (**nie ma go w nginx**, nie jedzie przez sieć) |
| `BRIDGE_PS_TOKEN` | Bearer do ścieżki sterowania PC (`/pc-<losowy>/`) | most (tylko z `-Pc`), **nginx**, klient |
| `BRIDGE_PS_SIGN_KEY` | podpis HMAC żądań do PC | most (z `-Pc`) i klient |
| `BRIDGE_URL` | adres telefonu: `https://twoja-domena/y5-<losowy>` (bez końcowego `/`) | klient w chmurze |
| `PC_URL` | adres sterowania PC: `https://twoja-domena/pc-<losowy>` (bez końcowego `/`) | klient w chmurze |

Wszystkie cztery sekrety muszą być **różne** (most sam odmówi startu, jeśli się powtarzają). Minimalne długości: `BRIDGE_TOKEN` i
`BRIDGE_SIGN_KEY` ≥ 64 znaków, `BRIDGE_PS_TOKEN` i `BRIDGE_PS_SIGN_KEY` ≥ 100 znaków. Zalecane: **150 znaków** każdy.

## 2. Generowanie (PowerShell na PC)

Używa kryptograficznego generatora losowego. Wykonaj w folderze mostu (np. `C:\Users\pc\Documents\y5_linux_work`):

```powershell
$rng = [Security.Cryptography.RandomNumberGenerator]::Create()
function New-Secret([int]$n = 150) {
  $c = [char[]]'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_'   # 64 znaki, wiec % 64 jest rowno
  $b = New-Object byte[] $n; $rng.GetBytes($b); -join ($b | ForEach-Object { $c[$_ % 64] })
}
function New-Hex([int]$n = 16) { $b = New-Object byte[] $n; $rng.GetBytes($b); -join ($b | ForEach-Object { $_.ToString('x2') }) }

New-Secret | Set-Content TOKEN.txt             -NoNewline -Encoding ascii
New-Secret | Set-Content BRIDGE_SIGN_KEY.txt   -NoNewline -Encoding ascii
New-Secret | Set-Content PS_TOKEN.txt          -NoNewline -Encoding ascii
New-Secret | Set-Content PS_SIGN_KEY.txt       -NoNewline -Encoding ascii
"/y5-$(New-Hex)/" | Set-Content PATH_SEGMENT.txt    -NoNewline -Encoding ascii
"/pc-$(New-Hex)/" | Set-Content PC_PATH_SEGMENT.txt -NoNewline -Encoding ascii
```

Zabezpiecz folder (tylko Twoje konto):
```powershell
icacls . /inheritance:r /grant:r "$($env:USERNAME):(OI)(CI)F"
```
Trzymaj te pliki **lokalnie**, nie na udziale sieciowym.

## 3. PC z mostem (zmienne ustawia skrypt startowy)

Zalecany sposób: `bridge/start_bridge_signed.ps1` czyta pliki i ustawia zmienne **tylko dla procesu mostu** (nic nie zostaje w rejestrze):

```powershell
# tylko telefon
PowerShell -ExecutionPolicy Bypass -File .\start_bridge_signed.ps1 -NginxIp <IP_PC_Z_NGINX>
# z kontrola PC na 8 godzin (zwykle okno, NIE administrator)
PowerShell -ExecutionPolicy Bypass -File .\start_bridge_signed.ps1 -NginxIp <IP_PC_Z_NGINX> -Pc -PcHours 8
```
Skrypt ustawia: `BRIDGE_BIND`, `BRIDGE_ALLOW_FROM`, `BRIDGE_TOKEN`, `BRIDGE_SIGN_KEY` oraz (z `-Pc`) `BRIDGE_PS`, `BRIDGE_PS_TOKEN`,
`BRIDGE_PS_SIGN_KEY`, `BRIDGE_PS_TTL_MIN`. Ręcznie (to samo, tylko w bieżącym oknie PowerShell):

```powershell
$env:BRIDGE_BIND        = "192.168.1.64"          # adres LAN tego PC (z telefonem)
$env:BRIDGE_ALLOW_FROM  = "192.168.1.65"          # adres PC z nginx
$env:BRIDGE_TOKEN       = (Get-Content TOKEN.txt -Raw).Trim()
$env:BRIDGE_SIGN_KEY    = (Get-Content BRIDGE_SIGN_KEY.txt -Raw).Trim()
# opcjonalnie sterowanie PC:
$env:BRIDGE_PS          = "1"
$env:BRIDGE_PS_TOKEN    = (Get-Content PS_TOKEN.txt -Raw).Trim()
$env:BRIDGE_PS_SIGN_KEY = (Get-Content PS_SIGN_KEY.txt -Raw).Trim()
$env:BRIDGE_PS_TTL_MIN  = "480"                    # wygasa po 8 h
python bridge_server.py
```
Nie używaj `setx` ani zmiennych systemowych na stałe: sekrety zostałyby w rejestrze i byłyby widoczne dla innych procesów.
Wyłącznik awaryjny sterowania PC: `New-Item DISABLE_PC -ItemType File` obok `bridge_server.py` (bez restartu).

## 4. nginx (tylko token i ścieżki, bez klucza podpisu)

Do bloków `location` wstawiasz **`BRIDGE_TOKEN`**, **`BRIDGE_PS_TOKEN`** oraz ścieżki z `PATH_SEGMENT.txt` i `PC_PATH_SEGMENT.txt`.
Gotowy szablon: `bridge/nginx_y5_snippet.conf` (znaczniki `__Y5_PATH__`, `__Y5_TOKEN__`, `__PC_PATH__`, `__PS_TOKEN__`).
Podmiana jednym poleceniem (PowerShell, w folderze z plikami kluczy):
```powershell
$c = Get-Content nginx_y5_snippet.conf -Raw
$c = $c.Replace('__Y5_PATH__',  (Get-Content PATH_SEGMENT.txt -Raw).Trim()).Replace('__Y5_TOKEN__', (Get-Content TOKEN.txt -Raw).Trim())
$c = $c.Replace('__PC_PATH__',  (Get-Content PC_PATH_SEGMENT.txt -Raw).Trim()).Replace('__PS_TOKEN__', (Get-Content PS_TOKEN.txt -Raw).Trim())
[IO.File]::WriteAllText("$PWD\y5_locations.conf", $c, (New-Object Text.UTF8Encoding($false)))   # UTF-8 bez BOM
```
Potem `include y5_locations.conf;` w bloku `server { listen 443 ... }`, `nginx -t` i **twardy restart** (`taskkill /F /IM nginx.exe`, uruchom ponownie;
na Windowsie `nginx -s reload` zostawia stare procesy ze starą konfiguracją). Plik `y5_locations.conf` zawiera tokeny: nie commituj.

## 5. Środowisko chmurowe Claude (nowe sesje)

W ustawieniach środowiska: menu środowiska w pasku tytułu sesji → **Edit** → **Environment variables**. **Nie używaj „Network secrets”**: ten
mechanizm dokleja jeden nagłówek do każdego żądania do hosta, więc nie rozróżni dwóch tokenów na tej samej domenie i nie wyliczy podpisu.

Dodaj sześć zmiennych (wartości kopiujesz z plików; zmienne obowiązują w **nowych** sesjach):

| Zmienna | Wartość |
|---|---|
| `BRIDGE_URL` | `https://twoja-domena` + zawartość `PATH_SEGMENT.txt` bez końcowego `/` |
| `BRIDGE_TOKEN` | `TOKEN.txt` |
| `BRIDGE_SIGN_KEY` | `BRIDGE_SIGN_KEY.txt` |
| `PC_URL` | `https://twoja-domena` + zawartość `PC_PATH_SEGMENT.txt` bez końcowego `/` |
| `BRIDGE_PS_TOKEN` | `PS_TOKEN.txt` |
| `BRIDGE_PS_SIGN_KEY` | `PS_SIGN_KEY.txt` |

Wartości zmiennych środowiska widzi sesja (Claude), więc to nie jest „ukryty” sekret. Dlatego trzymaj `-PcHours` krótko i włączaj `-Pc` tylko wtedy, gdy
Claude ma pracować na PC.

## 6. Test działania

Z sesji chmurowej (po ustawieniu zmiennych):
```bash
python3 bridge/y5sh_cloud.py run "uname -a"       # telefon (zwraca uname z telefonu)
python3 bridge/y5sh_cloud.py pc-info              # PC (tylko z -Pc na moście)
```
Typowe wyniki błędów: `401` (zły podpis, zły token albo zegary rozjechane o > 120 s), `404` (zła ścieżka albo sterowanie PC wyłączone/wygasło),
`520` (nginx zamknął połączenie: zły token, brak Cloudflare), `502` (nginx nie dosięga mostu: most nie działa lub zły `-NginxIp`/adres).
Pytanie kontrolne: żądanie bez podpisu musi dostać `401`, a to samo żądanie powtórzone (replay) także `401`.

## 7. Rotacja kluczy

1. Wygeneruj nowe pliki (pkt 2), 2. zatrzymaj most, 3. zbuduj nowy `y5_locations.conf` i zrób twardy restart nginx, 4. uruchom most,
5. zaktualizuj zmienne w środowisku chmurowym. Klucze z czatu lub z historii traktuj jako spalone i wymieniaj od razu.
