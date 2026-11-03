from pathlib import Path
import json,shlex,subprocess
r=Path('/home/y5/mesa-native');b=r/'build-mfw';e=next(x for x in json.loads((b/'compile_commands.json').read_text()) if x['file'].endswith('/pvr_pds.c'));a=shlex.split(e['command']);a[a.index('-o')+1]='/tmp/mfw-ppp-triangle.o';a[a.index('-c')+1]='/mnt/d/y5_linux_work/ppp_triangle_probe.c';a += ['-fsanitize=address,undefined'];subprocess.run(a,cwd=b,check=True)
subprocess.run(['cc','-fsanitize=address,undefined','/tmp/mfw-ppp-triangle.o',str(b/'src/imagination/vulkan/pds/libpvr_pds.a'),str(b/'src/util/libmesa_util.a'),'-lm','-lz','-lzstd','-pthread','-o','/tmp/mfw-ppp-triangle'],check=True)
subprocess.run(['/tmp/mfw-ppp-triangle'],check=True)



