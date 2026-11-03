from pathlib import Path
p=Path('/mnt/d/y5_linux_work/test_mesa_ddk19_adapter.c');s=p.read_text();preamble=s[:s.index('static int mode,ioctls,version_frees;')]
body='''
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "pvr_srv_ddk19.h"
static int drop_handle(int fd,unsigned command,void *handle){
 for(unsigned i=0;i<20;i++){
  enum pvr_srv_error error=PVR_SRV_ERROR_BRIDGE_CALL_FAILED;
  if(pvr_srv_ddk19_bridge_call(fd,6,command,&handle,8,&error,4))return 1;
  if(error==PVR_SRV_OK)return 0;
  if(error!=PVR_SRV_ERROR_RETRY)return 1;
  struct timespec t={.tv_nsec=10000000};nanosleep(&t,NULL);
 }
 return 1;
}
int main(void){
 _Static_assert(sizeof(void*)==8,"64-bit ABI only");
 int fd=open("/dev/dri/renderD128",O_RDWR|O_CLOEXEC);if(fd<0){perror("render open");return 1;}
 struct pvr_srv_bridge_connect_cmd in={.flags=32,.build_options=0x80000010,.DDK_version=0x10009,.DDK_build=4893595};
 struct pvr_srv_bridge_connect_ret out={0};
 struct pvr_srv_devmem_int_ctx_create_cmd ci={0};struct pvr_srv_devmem_int_ctx_create_ret co={0};
 struct pvr_srv_heap_count_cmd hi={0};struct pvr_srv_heap_count_ret ho={0};
 struct pvr_srv_devmem_int_heap_create_cmd heap_in={.base_addr={.addr=UINT64_C(0x65c0000000)},.size=0x80000000,.log2_page_size=12};
 struct pvr_srv_devmem_int_heap_create_ret heap_out={0};
 int connected=0,rc=1;
 if(pvr_srv_ddk19_bridge_call(fd,1,0,&in,sizeof(in),&out,sizeof(out))||out.error)goto done;
 connected=1;
 if(pvr_srv_ddk19_bridge_call(fd,6,15,&ci,sizeof(ci),&co,sizeof(co))||co.error||!co.server_memctx)goto done;
 if(pvr_srv_ddk19_bridge_call(fd,6,30,&hi,sizeof(hi),&ho,sizeof(ho))||ho.error||!ho.heap_count||ho.heap_count>64)goto done;
 heap_in.server_memctx=co.server_memctx;
 if(pvr_srv_ddk19_bridge_call(fd,6,17,&heap_in,sizeof(heap_in),&heap_out,sizeof(heap_out))||heap_out.error||!heap_out.server_heap)goto done;
 printf("MESA19_NATIVE_CONTEXT_PASS heaps=%u cache_line=%u; no rendering job\\n",ho.heap_count,co.cpu_cache_line_size);
 rc=0;
done:
 if(heap_out.server_heap&&drop_handle(fd,18,heap_out.server_heap)){rc=1;goto disconnect;}
 if(co.server_memctx&&drop_handle(fd,16,co.server_memctx))rc=1;
disconnect:
 if(connected){struct pvr_srv_bridge_disconnect_ret d;if(pvr_srv_ddk19_bridge_call(fd,1,1,NULL,0,&d,sizeof(d))||d.error)rc=1;}
 close(fd);printf("MESA19_TRIAL_RC=%d\\n",rc);return rc;
}
'''
Path('/mnt/d/y5_linux_work/mesa19_phone_smoke.c').write_text('#define _POSIX_C_SOURCE 200809L\n'+preamble+body)
