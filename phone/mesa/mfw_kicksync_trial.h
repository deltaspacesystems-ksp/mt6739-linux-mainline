/* SPDX-License-Identifier: MIT */
/* Firmware NULL command + UFO update only. No shader/3D register payload. */
static int kicksync_call(int fd,unsigned fn,void *in,unsigned insize,void *out,unsigned outsize){
 struct drm_srvkm_cmd cmd={.bridge_id=138,.bridge_func_id=fn,.in_data_ptr=(uintptr_t)in,.out_data_ptr=(uintptr_t)out,.in_data_size=insize,.out_data_size=outsize};
 if(fn>2)return -1;
 return drmIoctl(fd,DRM_IOCTL_SRVKM_CMD,&cmd);
}
static int firmware_sync_trial(int fd,void *private_data,void *sync_handle,volatile uint32_t *memory){
 PVRSRV_BRIDGE_IN_RGXCREATEKICKSYNCCONTEXT ci={.hPrivData=private_data};
 PVRSRV_BRIDGE_OUT_RGXCREATEKICKSYNCCONTEXT co={.eError=UINT32_MAX};
 void *block=sync_handle;uint32_t offset=0,value=0x87654321;char name[32]="MICROFW null-command trial";
 PVRSRV_BRIDGE_IN_RGXKICKSYNC ki={.hCheckFenceFD=-1,.hTimelineFenceFD=-1,.ui32ClientUpdateCount=1,.phUpdateUFOSyncPrimBlock=&block,.pui32UpdateSyncOffset=&offset,.pui32UpdateValue=&value,.puiUpdateFenceName=name};
 PVRSRV_BRIDGE_OUT_RGXKICKSYNC ko={.hUpdateFenceFD=-1,.eError=UINT32_MAX};
 int rc=1;
 if(!private_data||!sync_handle||memory[0]!=0x12345678)return 1;
 if(kicksync_call(fd,0,&ci,sizeof(ci),&co,sizeof(co))||co.eError||!co.hKickSyncContext){fprintf(stderr,"KickSync create error=%u\n",co.eError);return 1;}
 ki.hKickSyncContext=co.hKickSyncContext;
 if(kicksync_call(fd,2,&ki,sizeof(ki),&ko,sizeof(ko))||ko.eError){fprintf(stderr,"KickSync submit error=%u\n",ko.eError);goto cleanup;}
 for(unsigned i=0;i<200;i++){
  __sync_synchronize();
  if(memory[0]==value){printf("FIRMWARE_NULL_COMMAND_PASS observed=%08x; not a shader/render job\n",memory[0]);rc=0;break;}
  struct timespec pause={.tv_nsec=10000000};nanosleep(&pause,NULL);
 }
 if(rc){fprintf(stderr,"KickSync completion timeout observed=%08x; retain dependent resources\n",memory[0]);return 2;}
cleanup:
 for(unsigned i=0;i<50;i++){
  PVRSRV_BRIDGE_IN_RGXDESTROYKICKSYNCCONTEXT di={.hKickSyncContext=co.hKickSyncContext};PVRSRV_BRIDGE_OUT_RGXDESTROYKICKSYNCCONTEXT result={.eError=UINT32_MAX};
  if(kicksync_call(fd,1,&di,sizeof(di),&result,sizeof(result)))return 2;
  if(!result.eError)return rc;
  if(result.eError!=25)return 2;
  struct timespec pause={.tv_nsec=10000000};nanosleep(&pause,NULL);
 }
 return 2;
}
