from pathlib import Path
p=Path('/mnt/d/y5_linux_work/mesa19_phone_smoke.c');s=p.read_text();replacement='''#include <sys/ioctl.h>
#define DRM_COMMAND_BASE 0x40
#define DRM_IOWR(n,t) _IOWR('d',n,t)
typedef struct {int version_major,version_minor,version_patchlevel;char *name;} drmVersion,*drmVersionPtr;
int drmIoctl(int,unsigned long,void*);
drmVersionPtr drmGetVersion(int);
void drmFreeVersion(drmVersionPtr);
''';s=s.replace('#include <xf86drm.h>',replacement)
point=s.index('static int drop_handle(')
func='''/* Standalone smoke wrapper for the real DRM UAPI, not libdrm mock IO. */
struct raw_drm_version {int major,minor,patch;uint64_t name_len;char *name;uint64_t date_len;char *date;uint64_t desc_len;char *desc;};
_Static_assert(sizeof(struct raw_drm_version)==64,"arm64 DRM version UAPI");
int drmIoctl(int fd,unsigned long request,void *arg){int rc;do{rc=ioctl(fd,request,arg);}while(rc<0&&errno==EINTR);return rc;}
drmVersionPtr drmGetVersion(int fd){
 static char name[64];static drmVersion result;
 memset(name,0,sizeof(name));struct raw_drm_version v={.name_len=63,.name=name};
 if(drmIoctl(fd,DRM_IOWR(0,struct raw_drm_version),&v))return NULL;
 result=(drmVersion){.version_major=v.major,.version_minor=v.minor,.version_patchlevel=v.patch,.name=name};return &result;
}
void drmFreeVersion(drmVersionPtr v){(void)v;}
'''
s=s[:point]+func+s[point:];p.write_text(s)
