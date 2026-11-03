// Test Vulkan HAL module (hwvulkan): dlopen vulkan.mt6739.so, open "vk", create instance, enumerate GPUs
extern void *dlopen(const char*,int); extern void *dlsym(void*,const char*); extern char *dlerror(void);
extern int write(int,const void*,unsigned long); extern unsigned long strlen(const char*); extern void _exit(int);
static void P(const char*s){write(1,s,strlen(s));}
static void H(unsigned v){const char*h="0123456789abcdef";char b[9];for(int i=0;i<8;i++)b[i]=h[(v>>((7-i)*4))&15];b[8]=0;P("0x");P(b);}
typedef struct { unsigned sType; const void*pNext; unsigned flags; const void*pApp; unsigned nL; const char*const*L; unsigned nE; const char*const*E; } InstCI;
typedef struct { unsigned apiVersion, driverVersion, vendorID, deviceID, deviceType; char deviceName[256]; unsigned char uuid[16]; unsigned char rest[600]; } PDProps;
int main(void){
  P("== vktest ==\n");
  void*h=dlopen("/vendor/lib64/hw/vulkan.mt6739.so",2|0x100);
  if(!h){P("dlopen FAIL: ");char*e=dlerror();P(e?e:"?");P("\n");_exit(1);}
  P("dlopen OK\n");
  char*mod=dlsym(h,"HMI");
  if(!mod){P("no HMI\n");_exit(2);}
  P("module id: "); P(*(char**)(mod+8)); P("\n");
  char*meth=*(char**)(mod+32);
  int(*op)(void*,const char*,void**)=*(void**)meth;
  char*dev=0; int rr=op(mod,"vk",(void**)&dev);
  P("open vk r="); H(rr); P("\n");
  if(rr||!dev)_exit(2);
  void*(*gpa)(void*,const char*)=*(void**)(dev+88);
  if(!gpa){P("no GetInstanceProcAddr\n");_exit(2);}
  P("GetInstanceProcAddr OK\n");
  int(*createInst)(const InstCI*,const void*,void**)=*(void**)(dev+80);
  InstCI ci={1,0,0,0,0,0,0,0}; void*inst=0;
  int r=createInst(&ci,0,&inst);
  P("vkCreateInstance r="); H(r); P("\n");
  if(r||!inst)_exit(3);
  int(*enumPD)(void*,unsigned*,void**)=gpa(inst,"vkEnumeratePhysicalDevices");
  unsigned pc=0; r=enumPD(inst,&pc,0);
  P("physical devices: "); H(pc); P(" r="); H(r); P("\n");
  if(pc){
    void*pd[4]; if(pc>4)pc=4; enumPD(inst,&pc,pd);
    void(*getProps)(void*,PDProps*)=gpa(inst,"vkGetPhysicalDeviceProperties");
    PDProps pr; getProps(pd[0],&pr);
    P("deviceName: "); P(pr.deviceName); P("\napiVersion: "); H(pr.apiVersion); P(" vendor: "); H(pr.vendorID); P("\n");
  }
  _exit(0);
}
