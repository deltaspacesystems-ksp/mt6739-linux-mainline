from pathlib import Path
import json,shlex,subprocess
r=Path('/home/y5/mesa-native');b=r/'build-mfw';e=next(x for x in json.loads((b/'compile_commands.json').read_text()) if x['file'].endswith('/pvr_pds.c'));a=shlex.split(e['command']);a[a.index('-o')+1]='/tmp/mfw-generate_graphics_vdm_segment.o';a[a.index('-c')+1]='/mnt/d/y5_linux_work/generate_graphics_vdm_segment.c';a += ['-fsanitize=address,undefined'];subprocess.run(a,cwd=b,check=True)
subprocess.run(['cc','-fsanitize=address,undefined','/tmp/mfw-generate_graphics_vdm_segment.o',str(b/'src/imagination/vulkan/pds/libpvr_pds.a'),str(b/'src/util/libmesa_util.a'),'-lm','-lz','-lzstd','-pthread','-o','/tmp/mfw-generate_graphics_vdm_segment'],check=True)
subprocess.run(['/tmp/mfw-generate_graphics_vdm_segment'],check=True)


