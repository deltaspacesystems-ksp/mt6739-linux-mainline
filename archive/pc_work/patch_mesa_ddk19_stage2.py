from pathlib import Path
p=Path('/home/y5/mesa-native/src/imagination/vulkan/winsys/pvrsrvkm/pvr_srv_ddk19.h');s=p.read_text();anchor=' /* Never send an unchecked1.17 command/layout to a1.9 kernel. */';assert anchor in s;s=s.replace(anchor,Path('/mnt/d/y5_linux_work/mesa_ddk19_memory_stage2.inc').read_text()+'\n'+anchor);p.write_text(s);Path('/mnt/d/y5_linux_work/pvr_srv_ddk19.h').write_text(s)
