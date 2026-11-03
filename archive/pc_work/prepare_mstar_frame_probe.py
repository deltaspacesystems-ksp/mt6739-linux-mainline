from pathlib import Path
import struct
w=Path(r'D:\y5_linux_work')
blob=(w/'out/stock_Image').read_bytes()
assert struct.unpack_from('<I',blob,0xffffff80094ae4e0-0xffffff8008080000)[0]==0x26
s=(w/'y5_mstar_detect_probe.c').read_text()
start=s.index('static int read_product(');end=s.index('int main(',start)
s=s[:start]+'''/* Stock demo mode: one address-only READ of43B, header0x5a,
 * ten4B slots and checksum. Drain the PIO FIFO while transfer is active;
 * accept no coordinates unless the entire packet and checksum validate. */
static int read_product(unsigned char data[43])
{
 const unsigned offsets[]={4,8,0x10,0x14,0x18,0x6c};unsigned saved[6],status=0,n=0;int ret=-1;
 if((bus[0x24/2]&1)||((bus[0x30/2]>>4)&15))return -2;
 for(unsigned i=0;i<6;i++)saved[i]=bus[offsets[i]/2];
 bw(8,saved[1]&~7);bw(0xc,0x1ff);bw(0x38,1);bw(0x10,0x28);
 bw(4,(0x26<<1)|1);bw(0x14,43);bw(0x6c,0);bw(0x18,1);bw(0x24,1);
 uint64_t until=now_ns()+20000000;
 do{
   unsigned count=(bus[0x30/2]>>4)&15;
   while(count && n<43){data[n++]=bus[0]&255;count=(bus[0x30/2]>>4)&15;}
   status=bus[0xc/2];
   if((status&6)||((status&1)&&(n==43||!count)))break;
 }while(now_ns()<until);
 unsigned sum=0;for(unsigned i=0;i<n;i++)sum=(sum+data[i])&255;
 if((status&7)==1 && n==43 && data[0]==0x5a && sum==0)ret=0;
 printf("MSTAR frame addr=0x26 status=0x%04x bytes=%u header=0x%02x checksum_mod256=%u valid=%d\\n",status,n,data[0],sum,ret==0);
 unsigned nonzero=0;for(unsigned i=0;i<n;i++)if(data[i])nonzero++;
 printf("MSTAR nonzero_bytes=%u prefix=",nonzero);for(unsigned i=0;i<n&&i<12;i++)printf(" %02x",data[i]);puts("");
 if(!ret){unsigned contacts=0;for(unsigned i=0;i<10;i++){
   unsigned char *p=data+1+4*i;if(p[0]==255&&p[1]==255&&p[2]==255)continue;
   unsigned x=((p[0]&0xf0)<<4)|p[1],y=((p[0]&15)<<8)|p[2];
   printf("CONTACT slot=%u raw_x=%u raw_y=%u pressure=%u\\n",i,x,y,p[3]);contacts++;
 }printf("MSTAR contacts=%u\\n",contacts);}
 bw(0xc,status);bw(0x38,1);for(unsigned i=0;i<6;i++)bw(offsets[i],saved[i]);
 return ret;
}
'''+s[end:]
s=s.replace('--probe-mstar-detect','--probe-mstar-frame').replace('unsigned char data[6]={0};','unsigned char data[43]={0};')
s=s.replace('   ret=read_product(data);','   pin_dir(6,0);pause_ms(100);\n   printf("INT6 before read=%u\\n",!!(gpio[0x200/4]&(1U<<6)));\n   ret=read_product(data);')
s=s.replace('   ret=read_product(data);','   for(unsigned attempt=0;attempt<5;attempt++){ret=read_product(data);if(!ret)break;pause_ms(100);}')
s=s.replace('MSTAR stock_detect_tx37 addr=0x62','MSTAR bounded_normal_frame')
s=s.replace('    * Only stock detection command0x37 to debug address0x62.','    * Normal read only: no detection command or debug session.')
(w/'y5_mstar_frame_probe.c').write_text(s,newline='\n')
script=(w/'vtouch_snapshot.sh').read_text().replace('timeout 3 /tmp/y5_vtouch_snapshot','timeout 3 /var/cache/mfw-ui/y5_mstar_frame_probe --probe-mstar-frame')
(w/'mstar_frame_probe.sh').write_text(script,newline='\n')
print('Prepared one bounded normal report read; no controller command/data writes')
