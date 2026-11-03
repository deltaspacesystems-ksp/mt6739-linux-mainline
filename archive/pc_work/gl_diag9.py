import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
T=/sys/kernel/debug/tracing
ls $T/events/syscalls 2>&1 | head -n 3
echo 0 > $T/tracing_on; echo > $T/trace
echo 'comm == "gltest2"' > $T/events/syscalls/sys_enter_openat/filter 2>&1
echo 1 > $T/events/syscalls/sys_enter_openat/enable 2>&1
echo 1 > $T/events/syscalls/sys_exit_openat/enable 2>&1
echo 'comm == "gltest2"' > $T/events/syscalls/sys_exit_openat/filter 2>&1
echo 1 > $T/tracing_on
timeout 20 chroot /aroot /data/gltest2 >/tmp/gl3.out 2>&1 </dev/null
echo 0 > $T/tracing_on
echo 0 > $T/events/syscalls/sys_enter_openat/enable; echo 0 > $T/events/syscalls/sys_exit_openat/enable
grep gltest2 $T/trace | tail -n 60
""", timeout=60))
