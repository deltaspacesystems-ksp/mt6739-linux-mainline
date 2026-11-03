from pathlib import Path
w=Path(r'D:\y5_linux_work')
s=(w/'out/v14.dts').read_text()
assert s.count('framebuffer@771e0000')==1
assert s.count('reg = <0 0x775e8e38 0 0x3f4800>')==1
s=s.replace('framebuffer@771e0000','framebuffer@775eb000').replace('reg = <0 0x775e8e38 0 0x3f4800>','reg = <0 0x775eb000 0 0x3f4800>')
(w/'out/v15.dts').write_text(s,newline='\n')
build='''#!/bin/bash
set -eu
cd /mnt/d/y5_linux_work
dtc -I dts -O dtb -o out/v15.dtb out/v15.dts 2>out/v15-dtc.log
fdtget -t x out/v15.dtb /framebuffer@775eb000 reg
'''
(w/'build_dtbv15.sh').write_text(build,newline='\n')
s=(w/'bm_mfw_v54.py').read_text().replace('"v14.dtb"','"v15.dtb"').replace('"mfw_v54.img"','"mfw_v55.img"')
assert '"Image_mfw_v54"' in s # Same #40 kernel, only DT changes.
(w/'bm_mfw_v55.py').write_text(s,newline='\n')
print('Prepared v55 DT source: measured OVL3 address; no kernel/firmware changes')
