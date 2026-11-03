from pathlib import Path
p=Path('/home/y5/mesa-native/src/imagination/vulkan/winsys/pvrsrvkm/pvr_srv_ddk19.h');s=p.read_text();Path('/mnt/d/y5_linux_work/out/pvr_srv_ddk19-stage4a.h').write_text(s)
a='if(insize!=sizeof(*in)||outsize!=sizeof(*out)||in->pid||in->pdump_flags||';assert a in s
s=s.replace('#ifdef PVR_MFW_DDK19','#ifdef PVR_MFW_DDK19\n#include <unistd.h>',1).replace(a,'if(insize!=sizeof(*in)||outsize!=sizeof(*out)||(in->pid&&in->pid!=(uint32_t)getpid())||in->pdump_flags||')
p.write_text(s);Path('/mnt/d/y5_linux_work/pvr_srv_ddk19.h').write_text(s)
p=Path('/mnt/d/y5_linux_work/generate_mesa_ddk19_tests.py');s=p.read_text();s=s.replace('previous=ioctls;ai.pid=1;', 'ai.pid=(uint32_t)getpid();assert(!pvr_srv_ddk19_bridge_call(9,6,10,&ai,sizeof(ai),&ao,sizeof(ao))&&ao.error==0);\n previous=ioctls;ai.pid=(uint32_t)getpid()+1;');p.write_text(s)
p=Path('/mnt/d/y5_linux_work/mesa19_phone_smoke.c');s=p.read_text();s=s.replace(' if(pvr_srv_ddk19_bridge_call(fd,6,10,&ai', ' ai.pid=(uint32_t)getpid();\n if(pvr_srv_ddk19_bridge_call(fd,6,10,&ai');p.write_text(s)
