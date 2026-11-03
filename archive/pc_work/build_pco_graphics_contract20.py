from pathlib import Path
import json,shlex,subprocess,hashlib
r=Path('/home/y5/mesa-native');b=r/'build-mfw';source=(r/'src/imagination/pco/pco_ra.c').read_text()
old='unsigned hw_temps = rogue_get_temps(shader->ctx->dev_info);'
new='''/* MICROFW STANDALONE EXPERIMENT ONLY: software allocation budget, NOT physical profile. */
   assert(shader->stage == MESA_SHADER_VERTEX || shader->stage == MESA_SHADER_FRAGMENT);
   assert(!shader->data.common.uses.barriers);
   /* No compute workgroup assumptions for selected graphics shaders. */
   assert(shader->data.common.shareds == 0);
   unsigned hw_temps = 20;'''
assert source.count(old)==1;source=source.replace(old,new)
old='''ctx.allocable_temps =
         rogue_max_wg_temps(shader->ctx->dev_info,
                            ctx.allocable_temps,
                            wg_size,
                            shader->data.common.uses.barriers);'''
assert source.count(old)==1;source=source.replace(old,'assert(wg_size == 1); /* No hardware slot/depth inference. */')
source=source.replace('shader->data.common.spilled_temps = ctx.spilled_temps;', 'assert(!ctx.spilled_temps); assert(shader->data.common.temps <= 20); shader->data.common.spilled_temps = ctx.spilled_temps;')
source=source.replace('assert(!shader->is_grouped);','assert(!shader->is_grouped);\n   _Static_assert(sizeof(unsigned)==4,"32-bit RA budget arithmetic");')
source=source.replace('assert(shader->data.common.shareds == 0);', 'assert(shader->data.common.shareds == (shader->stage == MESA_SHADER_FRAGMENT ? 1 : 0)); assert(shader->stage != MESA_SHADER_FRAGMENT || (shader->data.fs.meta.start == 0 && shader->data.fs.meta.count == 1));')
Path('/mnt/d/y5_linux_work/pco_ra_graphics-contract20.c').write_text(source)
entries=json.loads((b/'compile_commands.json').read_text());entry=next(x for x in entries if x['file'].endswith('/pco_ra.c'))
a=shlex.split(entry['command']);a[a.index('-o')+1]='/tmp/mfw-pco-ra-graphics-contract20.o';a[a.index('-c')+1]='/mnt/d/y5_linux_work/pco_ra_graphics-contract20.c';subprocess.run(a,cwd=b,check=True)
entry=next(x for x in entries if x['file'].endswith('/pco_clc.c'));a=shlex.split(entry['command']);a[a.index('-o')+1]='/tmp/mfw-pco-graphics-contract20-main.o';a[a.index('-c')+1]='/mnt/d/y5_linux_work/pco_graphics_contract_probe.c';subprocess.run(a,cwd=b,check=True)
link=subprocess.check_output(['ninja','-C',str(b),'-t','commands','src/imagination/pco/uscgen/pco_clc'],text=True).splitlines()[-1];a=shlex.split(link);a[a.index('-o')+1]='/tmp/mfw-pco-graphics-contract20';a=[('/tmp/mfw-pco-graphics-contract20-main.o' if x.endswith('/pco_clc.c.o') else x) for x in a];idx=a.index('/tmp/mfw-pco-graphics-contract20-main.o');a.insert(idx+1,'/tmp/mfw-pco-ra-graphics-contract20.o');subprocess.run(a,cwd=b,check=True)
for stage,name in [(1,'vs'),(2,'fs')]:
 result=subprocess.run(['/tmp/mfw-pco-graphics-contract20','0','1',str(stage)],capture_output=True,text=True)
 (Path('/mnt/d/y5_linux_work/out')/('pco-graphics-contract20-'+name+'.log')).write_text(result.stdout+result.stderr)
 print(name,'RC',result.returncode,'\n','\n'.join(result.stdout.splitlines()[:5]),result.stderr[-1000:])
 if result.returncode==0:
  data=Path('/tmp/mfw-pco-restricted.bin').read_bytes();print('SHA256',hashlib.sha256(data).hexdigest());(Path('/mnt/d/y5_linux_work/out')/('pco-graphics-contract20-'+name+'.bin')).write_bytes(data)
