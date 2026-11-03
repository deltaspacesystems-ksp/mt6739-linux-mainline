from pathlib import Path
p=Path('D:/y5_linux_work/mesa19_phone_smoke.c');s=p.read_text(encoding='utf-8');Path('D:/y5_linux_work/out/mesa19_phone_smoke-pre-kick.c').write_text(s,encoding='utf-8')
s=s.replace('static int sync_readback(int fd,void *export_handle,uint32_t expected){','#include "mfw_kicksync_abi.h"\n#include "mfw_kicksync_trial.h"\nstatic int sync_readback(int fd,void *export_handle,uint32_t expected,void *private_data,void *sync_handle){')
s=s.replace(' done:\n if(memory!=MAP_FAILED', ' int kick_rc=firmware_sync_trial(fd,private_data,sync_handle,memory);\n if(kick_rc==2)return 2;\n if(kick_rc)rc=1;\n done:\n if(memory!=MAP_FAILED')
s=s.replace('sync_readback(fd,sync.pmr,si.value)','sync_readback(fd,sync.pmr,si.value,co.server_memctx_data,sync.handle)')
p.write_text(s,encoding='utf-8',newline='\n')
