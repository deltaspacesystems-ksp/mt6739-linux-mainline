from pathlib import Path
p=Path('D:/y5_linux_work/mesa19_phone_smoke.c');s=p.read_text();Path('D:/y5_linux_work/out/mesa19_phone_smoke-stage4.c').write_text(s)
helper='''/* Diagnostic exact1.9-only import chain; not a generic modern bridge translation. */
static int legacy_import_call(int fd,uint32_t function,uint64_t handle,void *out,uint32_t size){
 struct drm_srvkm_cmd cmd={.bridge_id=6,.bridge_func_id=function,.in_data_ptr=(uintptr_t)&handle,.out_data_ptr=(uintptr_t)out,.in_data_size=8,.out_data_size=size};
 if(!handle||(function!=3&&function!=4&&function!=6&&function!=7))return -1;
 return drmIoctl(fd,DRM_IOCTL_SRVKM_CMD,&cmd);
}
static int sync_readback(int fd,void *export_handle,uint32_t expected){
 struct {uint64_t handle;uint32_t error;} PACKED made={.error=UINT32_MAX};
 struct {uint64_t pmr,size,alignment;uint32_t error;} PACKED imported={.error=UINT32_MAX};
 _Static_assert(sizeof(made)==12&&sizeof(imported)==28,"1.9 local import ABI");
 volatile uint32_t *memory=MAP_FAILED;int rc=1;uint32_t error;
 if(legacy_import_call(fd,3,(uintptr_t)export_handle,&made,sizeof(made))||made.error||!made.handle)goto done;
 if(legacy_import_call(fd,6,made.handle,&imported,sizeof(imported))||imported.error||!imported.pmr)goto done;
 if(imported.size!=4096||imported.alignment!=4096||imported.pmr>INT64_MAX/4096)goto done;
 memory=mmap(NULL,4096,PROT_READ,MAP_SHARED,fd,(off_t)(imported.pmr*4096));
 if(memory==MAP_FAILED){perror("sync local-import mmap");goto done;}
 __sync_synchronize();
 if(memory[0]!=expected){fprintf(stderr,"sync readback mismatch got=%08x expected=%08x\\n",memory[0],expected);goto done;}
 printf("MESA19_SYNC_READBACK_PASS value=%08x bytes=%llu; CPU readback, no GPU job\\n",memory[0],(unsigned long long)imported.size);rc=0;
 done:
 if(memory!=MAP_FAILED&&munmap((void*)memory,4096)){rc=2;return rc;}
 if(imported.pmr){error=UINT32_MAX;if(legacy_import_call(fd,7,imported.pmr,&error,4)||error)return 2;}
 if(made.handle){error=UINT32_MAX;if(legacy_import_call(fd,4,made.handle,&error,4)||error)return 2;}
 return rc;
}
'''
a='int main(void){';assert a in s and 'static int sync_readback' not in s;s=s.replace(a,helper+a)
a=' si.value=0;';s=s.replace(a,' int readback_rc=sync_readback(fd,sync.pmr,si.value);\n if(readback_rc==2)goto disconnect;\n if(readback_rc)goto done;\n'+a)
p.write_text(s,newline='\n')
