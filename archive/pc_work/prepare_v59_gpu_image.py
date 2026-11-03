from pathlib import Path
import re,json,struct
w=Path(r'D:\y5_linux_work')
# Reuse the pure FDT parser, not the executable validator body.
s=(w/'verify_mfw_v58.py').read_text();a=s.index('def props(');b=s.index('op=props(',a);ns={'struct':struct};exec(s[a:b],ns);props=ns['props']
old=props((w/'out/v17.dtb').read_bytes());used=[struct.unpack('>I',v)[0] for (p,n),v in old.items() if n=='phandle']
base=max(used)+1;vcore,clock=base,base+1
pmic=struct.unpack('>I',old[('/pwrap@1000d000','phandle')])[0] if ('/pwrap@1000d000','phandle') in old else base+2
extra=f'''
/ {{
 pwrap@1000d000 {{
  phandle = <{pmic}>;
  vcore-readonly {{
   phandle = <{vcore}>;
   regulator-name = "microfw-vcore-readonly";
  }};
 }};
 mfw-mfg-probe-clock@10000000 {{
  compatible = "microfw,mt6739-mfg-probe-clock";
  reg = <0 0x10000000 0 0x1000>, <0 0x1000c000 0 0x1000>, <0 0x10006000 0 0x1000>;
  #clock-cells = <0>;
  phandle = <{clock}>;
 }};
 mfw-gpu-id-probe@10006000 {{
  compatible = "microfw,mt6739-gpu-id-probe";
  reg = <0 0x10006000 0 0x1000>, <0 0x10001000 0 0x1000>, <0 0x1020e000 0 0x1000>;
  clocks = <{clock}>;
  vcore-supply = <{vcore}>;
  microfw,pmic-provider = <{pmic}>;
 }};
}};
'''
(w/'out/v18.dts').write_text((w/'out/v17.dts').read_text()+extra,newline='\n')
(w/'microfw-gpu/gpu-probe-v59.dtsi').write_text('// Include after the v58 board DT. Explicit fresh phandles from its compiledDT.\n'+extra,newline='\n')
(w/'out/v59-dt-handles.json').write_text(json.dumps({'vcore':vcore,'clock':clock,'pmic':pmic,'pmic_phandle_added':('/pwrap@1000d000','phandle') not in old})+'\n')
(w/'build_dtbv18.sh').write_text('''#!/bin/bash
set -eu
cd /mnt/d/y5_linux_work
dtc -I dts -O dtb -o out/v18.dtb out/v18.dts 2>out/v18-dtc.log
fdtget -t s out/v18.dtb /mfw-gpu-id-probe@10006000 compatible
fdtget -t x out/v18.dtb /mfw-gpu-id-probe@10006000 vcore-supply
''',newline='\n')
(w/'build_v59_gpu_probe.sh').write_text('''#!/bin/bash
set -eu
cd /home/y5/linux-7.3
cmp .config /mnt/d/y5_linux_work/export/config-7.3-mfw-v58
cmp drivers/power/supply/y5-pmic-monitor.c /mnt/d/y5_linux_work/export/y5-pmic-before-v59.c
cp /mnt/d/y5_linux_work/export/y5-pmic-v59.c drivers/power/supply/y5-pmic-monitor.c
cp /mnt/d/y5_linux_work/microfw-gpu/clk-mfw-mfg-probe.c /mnt/d/y5_linux_work/microfw-gpu/mfw-mfg-clock-math.h drivers/clk/mediatek/
if ! grep -q '^obj-y += clk-mfw-mfg-probe.o$' drivers/clk/mediatek/Makefile; then printf '\\nobj-y += clk-mfw-mfg-probe.o\\n' >> drivers/clk/mediatek/Makefile; fi
cp /mnt/d/y5_linux_work/microfw-gpu/mfw-gpu-id-probe.c /mnt/d/y5_linux_work/microfw-gpu/mfw-gpu-id.h /mnt/d/y5_linux_work/microfw-gpu/mfw-mfg-power-sequence.h drivers/gpu/drm/mfw-gpu-probe/
if ! grep -q '^obj-y += mfw-gpu-probe/$' drivers/gpu/drm/Makefile; then printf '\\nobj-y += mfw-gpu-probe/\\n' >> drivers/gpu/drm/Makefile; fi
touch /mnt/d/y5_linux_work/out/v59-build-start
nohup bash /mnt/d/y5_linux_work/bld.sh > /dev/null 2>&1 &
build_pid=$!
wait "$build_pid"
test arch/arm64/boot/Image -nt /mnt/d/y5_linux_work/out/v59-build-start
cp arch/arm64/boot/Image /mnt/d/y5_linux_work/out/Image_mfw_v59
cp .config /mnt/d/y5_linux_work/export/config-7.3-mfw-v59
cat include/generated/utsversion.h
tail -n 5 /home/y5/build73f.log
''',newline='\n')
s=(w/'bm_mfw_v58.py').read_text().replace('Image_mfw_v58','Image_mfw_v59').replace('v17.dtb','v18.dtb').replace('mfw_v58.img','mfw_v59.img');(w/'bm_mfw_v59.py').write_text(s,newline='\n')
s=(w/'verify_mfw_v58.py').read_text().replace("old=(w/'mfw_v57.img')","old=(w/'mfw_v58.img')").replace("new=(w/'mfw_v58.img')","new=(w/'mfw_v59.img')").replace('Image_mfw_v58','Image_mfw_v59').replace('v17.dtb','v18.dtb')
a=s.index('op=props(od);np=props(nd)');b=s.index('assert len(new)',a)
s=s[:a]+'''import json
op=props(od);np=props(nd);h=json.loads((w/'out/v59-dt-handles.json').read_text())
added_paths=['/pwrap@1000d000/vcore-readonly','/mfw-mfg-probe-clock@10000000','/mfw-gpu-id-probe@10006000']
new={(p,n):v for (p,n),v in np.items() if p in added_paths}
assert new[('/pwrap@1000d000/vcore-readonly','phandle')]==struct.pack('>I',h['vcore'])
assert new[('/mfw-gpu-id-probe@10006000','vcore-supply')]==struct.pack('>I',h['vcore'])
assert new[('/mfw-gpu-id-probe@10006000','clocks')]==struct.pack('>I',h['clock'])
assert new[('/mfw-gpu-id-probe@10006000','microfw,pmic-provider')]==struct.pack('>I',h['pmic'])
expected={
 added_paths[0]:{'phandle','regulator-name'},
 added_paths[1]:{'compatible','reg','phandle','#clock-cells'},
 added_paths[2]:{'compatible','reg','clocks','vcore-supply','microfw,pmic-provider'},
}
for path,names in expected.items():assert {n for (p,n) in new if p==path}==names
for path,bases in [(added_paths[1],[0x10000000,0x1000c000,0x10006000]),(added_paths[2],[0x10006000,0x10001000,0x1020e000])]:
 assert new[(path,'reg')]==b''.join(struct.pack('>4I',0,base,0,4096) for base in bases)
np={k:v for k,v in np.items() if k not in new}
if h['pmic_phandle_added']:assert np.pop(('/pwrap@1000d000','phandle'))==struct.pack('>I',h['pmic'])
assert np==op,'Unexpected existing DT property change'
for marker in [b'GPU power/ID probe ready',b'VCORE readonly regulator registered',b'MFG CCF probe clock registered']:
 assert marker in ni,marker
assert (w/'export/config-7.3-mfw-v58').read_bytes()==(w/'export/config-7.3-mfw-v59').read_bytes()
'''+s[b:]
s=s.replace('exact DT delta only DMA-test opt-in+3additional MMIO resources','DT delta only readonly VCORE/CCF clock/explicit GPU-ID probe nodes');(w/'verify_mfw_v59.py').write_text(s,newline='\n')
s=(w/'export_v58_patch.py').read_text().replace('v58.patch','v59.patch').replace('v57.patch','v58.patch').replace('len(headers)==23','len(headers)==24')
s=s.replace("headers.append(('/dev/null','drivers/video/mfw-frame-switch.h'))",'''additions=[
 ('/dev/null','drivers/clk/mediatek/clk-mfw-mfg-probe.c'),
 ('/dev/null','drivers/clk/mediatek/mfw-mfg-clock-math.h'),
 ('a/drivers/gpu/drm/Makefile','drivers/gpu/drm/Makefile'),
 ('/dev/null','drivers/gpu/drm/mfw-gpu-probe/Makefile'),
 ('/dev/null','drivers/gpu/drm/mfw-gpu-probe/mfw-gpu-id-probe.c'),
 ('/dev/null','drivers/gpu/drm/mfw-gpu-probe/mfw-gpu-id.h'),
 ('/dev/null','drivers/gpu/drm/mfw-gpu-probe/mfw-mfg-power-sequence.h')]
for old,name in additions:
 if name not in {n for _,n in headers}:headers.append((old,name))''').replace('complete24','complete31');(w/'export_v59_patch.py').write_text(s,newline='\n')
s=(w/'check_v58_patch.py').read_text().replace('v58','v59').replace('==24','==31').replace('complete24','complete31');(w/'check_v59_patch.py').write_text(s,newline='\n')
print('Prepared v59 GPU power/identity probe, no automatic GPU activation on boot')
