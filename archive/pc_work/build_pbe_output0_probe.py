from pathlib import Path
import json,shlex,subprocess
r=Path('/home/y5/mesa-native');b=r/'build-mfw';e=next(x for x in json.loads((b/'compile_commands.json').read_text()) if x['file'].endswith('/pvr_arch_job_common.c'))
a=shlex.split(e['command']);a[a.index('-o')+1]='/tmp/mfw-job-common.o';a+=['-fsanitize=address,undefined'];subprocess.run(a,cwd=b,check=True)
a=shlex.split(e['command']);a[a.index('-o')+1]='/tmp/mfw-pbe-output0.o';a[a.index('-c')+1]='/mnt/d/y5_linux_work/pbe_output0_probe.c';a+=['-fsanitize=address,undefined'];subprocess.run(a,cwd=b,check=True)
subprocess.run(['cc','-fsanitize=address,undefined','-Wl,--gc-sections','/tmp/mfw-pbe-output0.o','/tmp/mfw-job-common.o',str(b/'src/util/libmesa_util.a'),'-lm','-lz','-lzstd','-pthread','-o','/tmp/mfw-pbe-output0'],check=True)
subprocess.run(['/tmp/mfw-pbe-output0'],check=True)
