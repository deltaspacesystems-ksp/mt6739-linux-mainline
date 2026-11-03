from pathlib import Path
import json,shlex,subprocess,re,struct,hashlib,os
r=Path('/home/y5/mesa-native');b=r/'build-mfw';entries=json.loads((b/'compile_commands.json').read_text())
ra=Path('/mnt/d/y5_linux_work/pco_ra_budget8.c').read_text().replace('unsigned hw_temps = 8;','unsigned hw_temps = 20;').replace('shader->data.common.temps <= 8','shader->data.common.temps <= 20')
Path('/mnt/d/y5_linux_work/pco_ra_context20.c').write_text(ra)
main=(r/'src/imagination/pco/uscgen/pco_clc.c').read_text();needle='nir_foreach_entrypoint (func, nir) {';assert main.count(needle)==1
main=main.replace(needle,needle+'\n      if(strcmp(func->name,"cs_store_sr_1024_common") && strcmp(func->name,"cs_load_sr_256_common")) continue;')
needle='pco_ctx_update_dev_info(ctx, &device_infos[d]);';assert main.count(needle)==1
main=main.replace(needle,'''struct pvr_device_info restricted_unknown={.ident={.b=22,.v=49,.n=21,.c=16}};
            /* No synthetic hardware capacity: RA is explicitly bounded in standalone override. */
            pco_ctx_update_dev_info(ctx, &restricted_unknown);''')
Path('/mnt/d/y5_linux_work/pco_context_clc.c').write_text(main)
def compile(suffix,source,out):
 e=next(x for x in entries if x['file'].endswith(suffix));a=shlex.split(e['command']);a[a.index('-o')+1]=out;a[a.index('-c')+1]=source;subprocess.run(a,cwd=b,check=True)
compile('/pco_ra.c','/mnt/d/y5_linux_work/pco_ra_context20.c','/tmp/mfw-context-ra.o');compile('/pco_clc.c','/mnt/d/y5_linux_work/pco_context_clc.c','/tmp/mfw-context-clc.o')
line=subprocess.check_output(['ninja','-C',str(b),'-t','commands','src/imagination/pco/uscgen/pco_clc'],text=True).splitlines()[-1];a=shlex.split(line);a[a.index('-o')+1]='/tmp/mfw-context-clc';a=[('/tmp/mfw-context-clc.o' if x.endswith('/pco_clc.c.o') else x) for x in a];a.insert(a.index('/tmp/mfw-context-clc.o')+1,'/tmp/mfw-context-ra.o');subprocess.run(a,cwd=b,check=True)
env=dict(os.environ,PCO_DEBUG='no_pred_cf');subprocess.run(['/tmp/mfw-context-clc',str(b/'src/imagination/pco/usclib/pco_usclib.spv'),'/tmp/mfw-context.h','/tmp/mfw-context.c'],env=env,check=True)
s=Path('/tmp/mfw-context.c').read_text();result={}
for name in ['cs_store_sr_1024_common','cs_load_sr_256_common']:
 m=re.search(r'static const uint32_t '+name+r'_0_common\[(\d+)\] = \{(.*?)\};',s,re.S);assert m,name;words=[int(x,16) for x in re.findall(r'0x[0-9a-f]+',m[2])];raw=struct.pack('<'+'I'*len(words),*words);count=struct.unpack_from('<H',raw,5)[0];binary=raw[8:];assert len(binary)==count*4 and raw[0]<=20
 Path('/mnt/d/y5_linux_work/out/'+name+'-bounded.bin').write_bytes(binary);result[name]={'bytes':len(binary),'temps':raw[0],'vtxins':raw[1],'sha256':hashlib.sha256(binary).hexdigest()}
Path('/mnt/d/y5_linux_work/out/context20-result.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
