from pathlib import Path
w=Path(r'D:\y5_linux_work')
p=w/'microfw-display/mfw-rdma-observer.c';s=p.read_text().replace('#define RDMA_INT_ENABLE', '#ifdef MODULE\n#error "Bringup observer must be built-in; LK keeps the interrupt enabled"\n#endif\n\n#define RDMA_INT_ENABLE',1);p.write_text(s,newline='\n')
s=(w/'out/v15.dts').read_text()
a=s.index('\tdisp_rdma0@1400b000 {');b=s.index('\n\t};',a)+len('\n\t};')
old=s[a:b];assert 'interrupts = <0x00 0xca 0x08>;' in old and 'compatible = "mediatek,disp_rdma0";' in old
new=old.replace('compatible = "mediatek,disp_rdma0";','compatible = "microfw,mt6739-rdma-observer";\n\t\tmicrofw,observe-lk-scanout;')
(w/'out/v16.dts').write_text(s[:a]+new+s[b:],newline='\n')
(w/'build_dtbv16.sh').write_text('''#!/bin/bash
set -eu
cd /mnt/d/y5_linux_work
dtc -I dts -O dtb -o out/v16.dtb out/v16.dts 2>out/v16-dtc.log
fdtget -t x out/v16.dtb /disp_rdma0@1400b000 interrupts
fdtget -t s out/v16.dtb /disp_rdma0@1400b000 compatible
''',newline='\n')
(w/'build_v57_observer.sh').write_text('''#!/bin/bash
set -eu
cd /home/y5/linux-7.3
cmp .config /mnt/d/y5_linux_work/export/config-7.3-mfw-v56
cmp drivers/video/y5disp.c /mnt/d/y5_linux_work/export/y5disp-v56.c
cp /mnt/d/y5_linux_work/microfw-display/mfw-rdma-observer.c drivers/video/mfw-rdma-observer.c
cp drivers/video/Makefile /mnt/d/y5_linux_work/export/video-Makefile-before-v57
if ! grep -q '^obj-y += mfw-rdma-observer.o$' drivers/video/Makefile; then
 printf '\\n# MICROFW transitional IRQ observation, no modeset\\nobj-y += mfw-rdma-observer.o\\n' >> drivers/video/Makefile
fi
touch /mnt/d/y5_linux_work/out/v57-build-start
nohup bash /mnt/d/y5_linux_work/bld.sh > /dev/null 2>&1 &
build_pid=$!
wait "$build_pid"
test arch/arm64/boot/Image -nt /mnt/d/y5_linux_work/out/v57-build-start
grep -q 'LK scanout observer IRQ=' drivers/video/mfw-rdma-observer.c
cp arch/arm64/boot/Image /mnt/d/y5_linux_work/out/Image_mfw_v57
cp .config /mnt/d/y5_linux_work/export/config-7.3-mfw-v57
cp drivers/video/Makefile /mnt/d/y5_linux_work/export/video-Makefile-v57
cat include/generated/utsversion.h
tail -n 5 /home/y5/build73f.log
''',newline='\n')
s=(w/'bm_mfw_v56.py').read_text().replace('"Image_mfw_v56"','"Image_mfw_v57"').replace('"v15.dtb"','"v16.dtb"').replace('"mfw_v56.img"','"mfw_v57.img"');(w/'bm_mfw_v57.py').write_text(s,newline='\n')
print('Prepared v57: only IRQ observer + explicit DT opt-in; config and scanout unchanged')
