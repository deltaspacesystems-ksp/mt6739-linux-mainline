# Start mostu z podpisywaniem zadan. Uruchom w folderze z bridge_server.py, pc_agent.py, y5sh.py oraz plikami kluczy:
#   TOKEN.txt, BRIDGE_SIGN_KEY.txt            (telefon)
#   PS_TOKEN.txt, PS_SIGN_KEY.txt             (sterowanie PC - tylko z przelacznikiem -Pc)
#
#   PowerShell -ExecutionPolicy Bypass -File .\start_bridge_signed.ps1                       # tylko telefon
#   PowerShell -ExecutionPolicy Bypass -File .\start_bridge_signed.ps1 -Pc                   # + sterowanie PC (wygasa po 4 h)
#   PowerShell -ExecutionPolicy Bypass -File .\start_bridge_signed.ps1 -Pc -PcHours 1        # krocej
# Wylaczenie awaryjne sterowania PC bez restartu:  New-Item DISABLE_PC -ItemType File
param(
    [switch]$Pc,
    [int]$PcHours = 4,
    [string]$Bind = "192.168.1.64",          # adres LAN tego PC (na nim nasluchuje most)
    [string]$NginxIp = "<IP_PC_Z_NGINX>"     # tylko stad most przyjmuje polaczenia
)
$here = Split-Path -Parent $MyInvocation.MyCommand.Path
function Secret($name) { (Get-Content (Join-Path $here $name) -Raw).Trim() }

$env:BRIDGE_BIND       = $Bind
$env:BRIDGE_ALLOW_FROM = $NginxIp
$env:BRIDGE_TOKEN      = Secret "TOKEN.txt"
$env:BRIDGE_SIGN_KEY   = Secret "BRIDGE_SIGN_KEY.txt"
if ($Pc) {
    $env:BRIDGE_PS          = "1"
    $env:BRIDGE_PS_TOKEN    = Secret "PS_TOKEN.txt"
    $env:BRIDGE_PS_SIGN_KEY = Secret "PS_SIGN_KEY.txt"
    $env:BRIDGE_PS_TTL_MIN  = [string]($PcHours * 60)
} else {
    Remove-Item Env:BRIDGE_PS -ErrorAction SilentlyContinue
}
python (Join-Path $here "bridge_server.py")
