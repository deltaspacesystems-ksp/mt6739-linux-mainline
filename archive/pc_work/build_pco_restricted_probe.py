from pathlib import Path
import json,shlex,subprocess
r=Path('/home/y5/mesa-native');build=r/'build-mfw'
cmds=json.loads((build/'compile_commands.json').read_text());entry=next(c for c in cmds if c['file'].endswith('/pco_clc.c'))
args=shlex.split(entry['command']);i=args.index('-o');args[i+1]='/tmp/mfw-pco-restricted.o';i=args.index('-c');args[i+1]='/mnt/d/y5_linux_work/pco_restricted_probe.c'
subprocess.run(args,cwd=build,check=True)
link=subprocess.check_output(['ninja','-C',str(build),'-t','commands','src/imagination/pco/uscgen/pco_clc'],text=True).splitlines()[-1];args=shlex.split(link);i=args.index('-o');args[i+1]='/tmp/mfw-pco-restricted';args=[('/tmp/mfw-pco-restricted.o' if a.endswith('/pco_clc.c.o') else a) for a in args]
subprocess.run(args,cwd=build,check=True);subprocess.run(['/tmp/mfw-pco-restricted','64','4','0'],check=True)
