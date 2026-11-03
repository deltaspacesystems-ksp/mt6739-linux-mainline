from pathlib import Path
import subprocess
w=Path(r"D:\y5_linux_work")
for name in ['src/main.c','src/desktop.c','src/wayland.c','src/sampler.c','src/sampler.h','tools/test_sampler.c','tools/bench_sampler_read.c','Makefile']:
 subprocess.run(['python',str(w/'phone_push.py'),str(w/'microfw-ui'/name),'/opt/microfw/ui/'+name,'--port','9500','--mode','644'],check=True)
for name in ['build_async_ui_phone.sh','deploy_async_ui_phone.sh']:
 subprocess.run(['python',str(w/'phone_push.py'),str(w/name),'/var/cache/mfw-ui/'+name,'--port','9500','--mode','700'],check=True)
subprocess.run(['python',str(w/'phone_shell.py'),'sh /var/cache/mfw-ui/build_async_ui_phone.sh'],check=True)
