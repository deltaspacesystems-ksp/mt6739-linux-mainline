from pathlib import Path
w=Path(r'D:\y5_linux_work')
s=(w/'build_renderer_update_phone.sh').read_text();s=s.replace('src/main.c src/model.c','src/main.c src/model.c src/sampler.c').replace('-O2 -g -std=c11','-O2 -g -pthread -std=c11');s='''#!/bin/sh
set -eu
cd /opt/microfw/ui
cc -O2 -pthread -std=c11 -Wall -Wextra -Werror -Isrc $(pkg-config --cflags cairo) tools/test_sampler.c src/sampler.c -o build/test-sampler
./build/test-sampler
'''+s[s.index('cc -Isrc'):];(w/'build_async_ui_phone.sh').write_text(s,newline='\n')
(w/'deploy_async_ui_phone.sh').write_text('''#!/bin/sh
set -eu
cd /opt/microfw/ui
test "$(cat /var/cache/mfw-ui/ui-update.status)" = READY
test -f build/microfw-shell.next
test ! -S /run/seatd.sock
if [ ! -e build/microfw-shell.pre-async ]; then cp -p build/microfw-shell build/microfw-shell.pre-async; fi
mv -f build/microfw-shell.next build/microfw-shell
printf 'DEPLOYED_ASYNC\\n' > /var/cache/mfw-ui/ui-update.status
''',newline='\n')
# Native snapshot benchmark without repeated hardware polling in the UI thread.
(w/'microfw-ui/tools/bench_sampler_read.c').write_text('''#define _POSIX_C_SOURCE 200809L
#include "sampler.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
static uint64_t ns(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return (uint64_t)t.tv_sec*1000000000ULL+t.tv_nsec;}
int main(void){struct mfw_model m={0};mfw_sample(&m);if(mfw_sampler_start(&m))return 1;uint64_t total=0,max=0;for(unsigned i=0;i<200;i++){uint64_t start=ns();mfw_sampler_read(&m);uint64_t dt=ns()-start;total+=dt;if(dt>max)max=dt;struct timespec t={.tv_nsec=1000000};nanosleep(&t,NULL);}mfw_sampler_stop();printf("SAMPLER_READ_BENCH n=200 avg_ns=%llu max_ns=%llu\\n",(unsigned long long)(total/200),(unsigned long long)max);return 0;}
''',newline='\n')
s=(w/'build_async_ui_phone.sh').read_text().replace("printf 'READY",'''cc -O2 -pthread -std=c11 -Wall -Wextra -Werror -Isrc $(pkg-config --cflags cairo) tools/bench_sampler_read.c src/sampler.c src/model.c -o build/bench-sampler-read
./build/bench-sampler-read
printf 'READY''');(w/'build_async_ui_phone.sh').write_text(s,newline='\n')
(w/'push_async_ui.py').write_text('''from pathlib import Path
import subprocess
w=Path(r"D:\\y5_linux_work")
for name in ['src/main.c','src/desktop.c','src/wayland.c','src/sampler.c','src/sampler.h','tools/test_sampler.c','tools/bench_sampler_read.c','Makefile']:
 subprocess.run(['python',str(w/'phone_push.py'),str(w/'microfw-ui'/name),'/opt/microfw/ui/'+name,'--port','9500','--mode','644'],check=True)
for name in ['build_async_ui_phone.sh','deploy_async_ui_phone.sh']:
 subprocess.run(['python',str(w/'phone_push.py'),str(w/name),'/var/cache/mfw-ui/'+name,'--port','9500','--mode','700'],check=True)
subprocess.run(['python',str(w/'phone_shell.py'),'sh /var/cache/mfw-ui/build_async_ui_phone.sh'],check=True)
''',newline='\n')
