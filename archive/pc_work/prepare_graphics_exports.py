from pathlib import Path
b=Path('D:/y5_linux_work')
for name,count,target in [('ppp_triangle_probe.c',25,'graphics-ppp.bin'),('vdm_triangle_segment_probe.c',8,'graphics-vdm-segment.bin')]:
 s=(b/name).read_text();needle=' for(unsigned i=1;i<26;i++)' if count==25 else ' for(unsigned n=1;n<9;n++)';a=s.index(needle)
 s=s[:a]+f' FILE *out=fopen("/mnt/d/y5_linux_work/out/{target}","wb");assert(out);assert(fwrite('+('storage' if count==25 else 's')+f'+1,4,{count},out)=={count});assert(!fclose(out));\n'+s[a:]
 n='generate_'+('graphics_ppp.c' if count==25 else 'graphics_vdm_segment.c');(b/n).write_text(s,newline='\n')
 build=(b/'build_pds_fragment_kick_probe.py').read_text().replace('pds_fragment_kick_probe.c',n).replace('mfw-pds-fragment-kick','mfw-'+n[:-2]);(b/('build_'+n[:-2]+'.py')).write_text(build,newline='\n')
s=(b/'pbe_linear64_probe.c').read_text().replace('0x65c0010000','0x65c0000000');needle=' printf("PBE_RGBA8';a=s.index(needle);s=s[:a]+' FILE *out=fopen("/mnt/d/y5_linux_work/out/graphics-pbe-output0.bin","wb");assert(out);assert(fwrite(cs+1,4,2,out)==2);assert(fwrite(regs+1,8,3,out)==3);assert(!fclose(out));\n'+s[a:];(b/'pbe_output0_probe.c').write_text(s,newline='\n');s=(b/'build_pbe_linear64_probe.py').read_text().replace('pbe_linear64_probe.c','pbe_output0_probe.c').replace('mfw-pbe-linear64','mfw-pbe-output0');(b/'build_pbe_output0_probe.py').write_text(s,newline='\n')
