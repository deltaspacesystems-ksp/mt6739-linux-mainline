$src = 'D:\y5_linux_work'
$st = 'D:\y5_linux_work\export\pc_work'
Remove-Item $st -Recurse -Force -ErrorAction SilentlyContinue
New-Item -ItemType Directory $st -Force | Out-Null
$ext = '.sh','.py','.c','.h','.bat','.java','.ps1','.dts','.dtsi','.md','.txt','.S','.cfg','.conf'
$skip = 'authkey|token|secret|passw|_key|key_|tskey|audit|\.pem|\.key'
$n = 0; $ex = @()
Get-ChildItem $src -File | Where-Object { $ext -contains $_.Extension -and $_.Length -lt 3MB } | ForEach-Object {
  if ($_.Name -match $skip) { $ex += $_.Name } else { Copy-Item $_.FullName $st; $n++ }
}
foreach ($d in 'kernel','tools','src') { if (Test-Path "$src\$d") { Copy-Item "$src\$d" "$st\$d" -Recurse -Force } }
Copy-Item "$src\out\stock_0.dts" $st -ErrorAction SilentlyContinue
Copy-Item "$src\out\dtc_v9.err" $st -ErrorAction SilentlyContinue
"copied=$n excluded=" + ($ex -join ',')
Get-ChildItem $st -Recurse -File | Measure-Object Length -Sum | ForEach-Object { "files=$($_.Count) bytes=$($_.Sum)" }
Remove-Item 'D:\y5_linux_work\export\pc_work.zip' -ErrorAction SilentlyContinue
Compress-Archive -Path "$st\*" -DestinationPath 'D:\y5_linux_work\export\pc_work.zip'
(Get-Item 'D:\y5_linux_work\export\pc_work.zip').Length
Get-ChildItem "$src\kernel","$src\tools","$src\src" -ErrorAction SilentlyContinue | Select-Object -First 15 | % Name
