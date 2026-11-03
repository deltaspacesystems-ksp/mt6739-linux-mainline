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
source=source.replace('assert(shader->data.common.shareds == 0);', 'assert(shader->stage == MESA_SHADER_FRAGMENT); assert(shader->data.common.shareds == 0); assert(shader->data.fs.uses.olchk_skip);')
Path('/mnt/d/y5_linux_work/pco_ra_eot20.c').write_text(source)
entries=json.loads((b/'compile_commands.json').read_text());entry=next(x for x in entries if x['file'].endswith('/pco_ra.c'))
a=shlex.split(entry['command']);a[a.index('-o')+1]='/tmp/mfw-pco-ra-eot20.o';a[a.index('-c')+1]='/mnt/d/y5_linux_work/pco_ra_eot20.c';subprocess.run(a,cwd=b,check=True)
entry=next(x for x in entries if x['file'].endswith('/pco_clc.c'));a=shlex.split(entry['command']);a[a.index('-o')+1]='/tmp/mfw-pco-eot-output0-main.o';a[a.index('-c')+1]='/mnt/d/y5_linux_work/pco_eot_output0_probe.c';subprocess.run(a,cwd=b,check=True)
link=subprocess.check_output(['ninja','-C',str(b),'-t','commands','src/imagination/pco/uscgen/pco_clc'],text=True).splitlines()[-1];a=shlex.split(link);a[a.index('-o')+1]='/tmp/mfw-pco-eot-output0';a=[('/tmp/mfw-pco-eot-output0-main.o' if x.endswith('/pco_clc.c.o') else x) for x in a];idx=a.index('/tmp/mfw-pco-eot-output0-main.o');a.insert(idx+1,'/tmp/mfw-pco-ra-eot20.o');subprocess.run(a,cwd=b,check=True)
subprocess.run(['/tmp/mfw-pco-eot-output0'],check=True)
