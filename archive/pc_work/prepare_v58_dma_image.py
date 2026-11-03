from pathlib import Path
w=Path(r'D:\y5_linux_work');d=w/'microfw-display'
p=d/'mfw-rdma-dma-test.c';s=p.read_text().replace('memcpy(o->cpu[i], o->boot_image, MFW_FRAME_BYTES);','dev_info(dev, "DMA test buffer%u addr=%pad bytes=%u\\n", i, &o->dma[i], MFW_FRAME_BYTES);\n  memcpy(o->cpu[i], o->boot_image, MFW_FRAME_BYTES);');p.write_text(s,newline='\n')
s=(w/'out/v16.dts').read_text();old='reg = <0x00 0x1400b000 0x00 0x1000>;';assert s.count(old)==1
s=s.replace(old,'reg = <0x00 0x1400b000 0x00 0x1000 0x00 0x1400a000 0x00 0x1000 0x00 0x14003000 0x00 0x1000 0x00 0x14012000 0x00 0x1000>;')
s=s.replace('microfw,observe-lk-scanout;','microfw,observe-lk-scanout;\n\t\tmicrofw,scanout-dma-test;');(w/'out/v17.dts').write_text(s,newline='\n')
s=(w/'build_dtbv16.sh').read_text().replace('v16','v17');(w/'build_dtbv17.sh').write_text(s,newline='\n')
s=(w/'build_v57_observer.sh').read_text().replace('v57','v58').replace('v56','v57')
# v57's source is preserved in the local repository; only our observer changes.
s=s.replace('cmp drivers/video/y5disp.c /mnt/d/y5_linux_work/export/y5disp-v57.c','cmp drivers/video/y5disp.c /mnt/d/y5_linux_work/export/y5disp-v56.c')
s=s.replace('cp /mnt/d/y5_linux_work/microfw-display/mfw-rdma-observer.c drivers/video/mfw-rdma-observer.c','cp /mnt/d/y5_linux_work/microfw-display/mfw-rdma-dma-test.c drivers/video/mfw-rdma-observer.c\ncp /mnt/d/y5_linux_work/microfw-display/mfw-frame-switch.h drivers/video/')
(w/'build_v58_dma_test.sh').write_text(s,newline='\n')
s=(w/'bm_mfw_v57.py').read_text().replace('Image_mfw_v57','Image_mfw_v58').replace('v16.dtb','v17.dtb').replace('mfw_v57.img','mfw_v58.img');(w/'bm_mfw_v58.py').write_text(s,newline='\n')
s=(w/'verify_mfw_v57.py').read_text().replace("old=(w/'mfw_v56.img')","old=(w/'mfw_v57.img')").replace("new=(w/'mfw_v57.img')","new=(w/'mfw_v58.img')").replace('Image_mfw_v57','Image_mfw_v58').replace('v16.dtb','v17.dtb')
a=s.index('op=props(od);np=props(nd)');b=s.index('assert len(new)',a)
s=s[:a]+'''op=props(od);np=props(nd)
node='/disp_rdma0@1400b000'
assert np.pop((node,'microfw,scanout-dma-test'))==b''
assert np[(node,'reg')]==struct.pack('>16I',0,0x1400b000,0,4096,0,0x1400a000,0,4096,0,0x14003000,0,4096,0,0x14012000,0,4096)
np[(node,'reg')]=struct.pack('>4I',0,0x1400b000,0,4096)
assert op==np,'Unexpected DT change'
assert np[(node,'interrupts')]==struct.pack('>3I',0,202,8)
assert b'DMA flip test: green5s, magenta5s, original' in ni
assert (w/'export/config-7.3-mfw-v57').read_bytes()==(w/'export/config-7.3-mfw-v58').read_bytes()
'''+s[b:]
s=s.replace('exact DT delta only RDMA compatible+opt-in','exact DT delta only DMA-test opt-in+3additional MMIO resources');(w/'verify_mfw_v58.py').write_text(s,newline='\n')
s=(w/'export_v57_patch.py').read_text().replace('v57.patch','v58.patch').replace('v56.patch','v57.patch').replace('len(headers)==22','len(headers)==23').replace("headers.append(('/dev/null','drivers/video/mfw-rdma-observer.c'))","headers.append(('/dev/null','drivers/video/mfw-frame-switch.h'))").replace('complete23','complete24');(w/'export_v58_patch.py').write_text(s,newline='\n')
s=(w/'check_v57_patch.py').read_text().replace('v57','v58').replace('==23','==24').replace('complete23','complete24');(w/'check_v58_patch.py').write_text(s,newline='\n')
print('Prepared v58; observer default mode unchanged, address writes require explicit sysfs test')
