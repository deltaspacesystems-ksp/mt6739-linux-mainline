from pathlib import Path
base = Path('D:/y5_linux_work/rogue-ddk19-port')
for relative, functions in {'services/server/common/pmr.c':['PMRInit', 'PMRDeInit'], 'services/server/common/pvrsrv.c':['PVRSRVGetPVRSRVData']}.items():
    p = base / relative
    s = p.read_text()
    for name in functions:
        s = s.replace(name + '()\n{', name + '(void)\n{')
    p.write_text(s, newline='\n')
