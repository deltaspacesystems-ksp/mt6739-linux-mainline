from pathlib import Path
import re
b=Path('D:/y5_linux_work');s=(b/'graphics19_memory_preflight.c').read_text();h=(b/'render19_hwrt_trial.c').read_text()
extra=h[h.index('struct target_out'):h.index('static int render_call')]
s=s.replace('static int render_call',extra+'static int render_call').replace('if(fn!=8&&','if(fn!=0&&fn!=1&&fn!=2&&fn!=3&&fn!=8&&')
s=s.replace('int main(void){','''static int load_exact(const char *name,void *dst,size_t bytes){char path[200];if(snprintf(path,sizeof(path),"/tmp/mfw-graphics64/%s",name)>=(int)sizeof(path))return 1;FILE *f=fopen(path,"rb");if(!f)return 1;size_t n=fread(dst,1,bytes,f);int extra=fgetc(f);int e=fclose(f);return n!=bytes||extra!=EOF||e;}
static uint32_t float_bits(float x){uint32_t u;memcpy(&u,&x,4);return u;}
int main(void){''')
s=s.replace('struct render_create_out render={0},freelist={0},local_list={0};','struct render_create_out render={0},freelist={0},local_list={0};\n struct target_out target={0};struct hwrt_out hwrt={0};')
needle=' puts("GRAPHICS_MEMORY32_PASS'
a=s.index(needle);z=s.index('done:\n',a)
load='''
 for(unsigned i=0;i<32;i++)if(!plan[i].protected_table)memset((void*)pages[i].cpu,0,4096);
 for(unsigned i=0;i<4;i++)for(unsigned w=0;w<1024;w++)pages[i].cpu[w]=0xdeadbeef;
 const struct {unsigned page;const char *name;} files[]={{4,"general-vdm.bin"},{5,"general-ppp.bin"},{15,"pds-main.bin"},{16,"pds-context.bin"},{28,"usc-main.bin"},{29,"usc-context.bin"}};
 for(unsigned i=0;i<sizeof(files)/sizeof(files[0]);i++){uint8_t temp[4096];if(load_exact(files[i].name,temp,sizeof(temp)))goto done;memcpy((void*)pages[files[i].page].cpu,temp,sizeof(temp));__sync_synchronize();if(memcmp((void*)pages[files[i].page].cpu,temp,sizeof(temp)))goto done;}
 uint64_t context[14]={0};uint32_t pbe[8]={0};if(load_exact("ta-context.bin",context,sizeof(context))||load_exact("pbe-state.bin",pbe,sizeof(pbe)))goto done;
 if(context[0]!=pages[8].va||context[1]!=pages[9].va||pbe[0]!=0x70000000||pbe[1]!=0x82019)goto done;
 if(pages[4].cpu[2]!=0x4c000f80||pages[4].cpu[5]!=5||pages[5].cpu[19]!=0x05040000)goto done;
 puts("GRAPHICS_IMAGES_CPU_STAGED_PASS 6pageimages/context112/PBE32; no GPU job");
'''
create=h[h.index(' uint8_t framework[16]'):h.index('done:\n',h.index(' uint8_t framework[16]'))]
create='\n'.join(line for line in create.splitlines() if 'memset(' not in line)+'\n'
create=create.replace('vdm_stack=pages[2].va','vdm_stack=pages[11].va').replace('pages[10]','pages[13]').replace('pages[11].va,.pmr=pages[11].pmr','pages[14].va,.pmr=pages[14].pmr')
create=create.replace('vheap=pages[3].va','vheap=pages[6].va+128').replace('mlist=pages[4].va','mlist=pages[10].va').replace('tail=pages[5].va,.max_rts=1','tail=pages[6].va,.max_rts=1,.ppp_screen=0x003f003f,.tpc_size=128,.te_screen=0x3003,.mtile1=0x100000,.mtile2=0x100000,.mtile_stride=16,.lower_x=float_bits(0x1.126138p-2f/64),.lower_y=float_bits(0x1.126138p-2f/64),.upper_x=float_bits(0x1.bb67b2p+0f/64),.upper_y=float_bits(0x1.bb67b2p+0f/64),.scale_x=float_bits(64*16.0f/0x1.76cf64p+0f),.scale_y=float_bits(64*16.0f/0x1.76cf64p+0f)')
# Constants above are exact bit patterns 3e89309c/3fddb3d9/3fbb67b2.
create=create.replace('puts("HWRT_CREATE_PASS lifecycle only; no TA/3D submitted");rc=0;','puts("GRAPHICS_RENDER_RESOURCES_STAGED_PASS geometry64x64/ownedbuffers; no TA/3D submitted");rc=0;')
s=s[:a]+load+create+s[z:]
cleanup=h[h.index('done:\n')+len('done:\n'):h.index(' if(local_list.context)',h.index('done:\n'))]
s=s.replace('done:\n if(local_list.context)','done:\n'+cleanup+' if(local_list.context)')
s=s.replace('GRAPHICS_MEMORY32_RC','GRAPHICS64_STAGE_RC')
(b/'graphics19_stage_trial.c').write_text(s,newline='\n')
