# Szyfrowana kopia kluczy mostu do repozytorium (haslo zna tylko wlasciciel; nie wpisuj go nigdzie poza tym promptem).
# Uzycie (PowerShell, w folderze z TOKEN.txt itd. - patrz docs/KLUCZE_I_ZMIENNE.md):
#   .\pack_secrets.ps1 -Folder C:\Users\pc\Documents\y5_linux_work -Domain twoja-domena
# Wynik: secrets.enc (AES-256, PBKDF2) - mozna go commitowac. secrets.env (jawny) jest kasowany.
# Odszyfrowanie: openssl enc -d -aes-256-cbc -pbkdf2 -iter 600000 -in secrets.enc -out secrets.env
param([Parameter(Mandatory)][string]$Folder, [Parameter(Mandatory)][string]$Domain)
$ErrorActionPreference = 'Stop'
function V($f) { (Get-Content (Join-Path $Folder $f) -Raw).Trim() }
$env_ = @(
  "BRIDGE_URL=https://$Domain" + (V 'PATH_SEGMENT.txt').TrimEnd('/'),
  "BRIDGE_TOKEN=" + (V 'TOKEN.txt'),
  "BRIDGE_SIGN_KEY=" + (V 'BRIDGE_SIGN_KEY.txt'),
  "PC_URL=https://$Domain" + (V 'PC_PATH_SEGMENT.txt').TrimEnd('/'),
  "BRIDGE_PS_TOKEN=" + (V 'PS_TOKEN.txt'),
  "BRIDGE_PS_SIGN_KEY=" + (V 'PS_SIGN_KEY.txt')
)
$plain = Join-Path $PSScriptRoot '..\secrets.env'
[IO.File]::WriteAllText($plain, ($env_ -join "`n") + "`n", (New-Object Text.UTF8Encoding($false)))
$out = Join-Path $PSScriptRoot '..\secrets.enc'
& openssl enc -aes-256-cbc -pbkdf2 -iter 600000 -salt -in $plain -out $out    # zapyta o haslo (dwa razy)
Remove-Item $plain -Force
Write-Host "Gotowe: $out (jawny secrets.env skasowany). Hasla nie zapisuj w repo."
