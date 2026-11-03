from pathlib import Path
w=Path(r'D:\y5_linux_work')
s=(w/'export/y5disp.c').read_text()
assert 'static void y5d_fire(void)\n{' in s
s=s.replace('static void y5d_fire(void)\n{','''/* DSI_MODE_CTRL[1:0]:0=command,1/2/3=continuous video.
 * Stock4.4 dsi_wait_not_busy also skips BUSY waits for video mode.
 * Measured on DURA-L21: MODE=1; RAM-only patch is displayed without kicks.
 */
static bool y5d_video_mode(void)
{
 return (readl(y5d_dsi + 0x14) & 3) != 0;
}

static void y5d_fire(void)
{
 if (y5d_video_mode())
  return;''')
s=s.replace('unsigned long min_gap = msecs_to_jiffies(40);','''unsigned long min_gap = msecs_to_jiffies(40);

 /* Do not restart a running video stream or poll it periodically. */
 if (y5d_video_mode())
  return;''')
s=s.replace('if (y5d_ready)\n\t\tschedule_delayed_work','if (y5d_ready && !y5d_video_mode())\n\t\tschedule_delayed_work')
s=s.replace('static ssize_t hz_show(','''static ssize_t mode_show(struct kobject *k, struct kobj_attribute *a, char *buf)
{
 return sysfs_emit(buf, "%u\\n", readl(y5d_dsi + 0x14) & 3);
}

static ssize_t hz_show(''')
s=s.replace('static struct kobj_attribute y5d_hz_attr','static struct kobj_attribute y5d_mode_attr = __ATTR_RO(mode);\nstatic struct kobj_attribute y5d_hz_attr')
s=s.replace('sysfs_create_file(kobj, &y5d_hz_attr.attr);','sysfs_create_file(kobj, &y5d_mode_attr.attr);\n\t\tsysfs_create_file(kobj, &y5d_hz_attr.attr);')
s=s.replace('pr_info("y5disp: wyzwalanie panelu po zmianie bufora (opcjonalnie cyklicznie: /sys/kernel/y5disp/hz)\\n");','''if (y5d_video_mode())
  pr_info("y5disp: continuous video scanout; manual kicks disabled\\n");
 else
  pr_info("y5disp: command mode; refresh after framebuffer updates\\n");''')
s=s.replace(' * Panel wymaga wyzwolenia po kazdej zmianie bufora', ' * Command-mode bootstrap (video mode never retriggered): legacy sequence')
assert s.count('y5d_video_mode()')>=4
(w/'export/y5disp-v56.c').write_text(s,newline='\n')
build='''#!/bin/bash
set -eu
cd /home/y5/linux-7.3
grep -qx CONFIG_ARM64=y .config
grep -qx CONFIG_INPUT_UINPUT=y .config
cmp .config /mnt/d/y5_linux_work/export/config-7.3-mfw-v54
cp drivers/video/y5disp.c /mnt/d/y5_linux_work/export/y5disp-before-v56.c
cmp drivers/video/y5disp.c /mnt/d/y5_linux_work/export/y5disp.c
cp /mnt/d/y5_linux_work/export/y5disp-v56.c drivers/video/y5disp.c
touch /mnt/d/y5_linux_work/out/v56-build-start
nohup bash /mnt/d/y5_linux_work/bld.sh > /dev/null 2>&1 &
build_pid=$!
wait "$build_pid"
test arch/arm64/boot/Image -nt /mnt/d/y5_linux_work/out/v56-build-start
grep -Fq 'continuous video scanout; manual kicks disabled' drivers/video/y5disp.c
cp arch/arm64/boot/Image /mnt/d/y5_linux_work/out/Image_mfw_v56
cp .config /mnt/d/y5_linux_work/export/config-7.3-mfw-v56
cat include/generated/utsversion.h
tail -n 5 /home/y5/build73f.log
'''
(w/'build_v56_video.sh').write_text(build,newline='\n')
s=(w/'bm_mfw_v55.py').read_text().replace('"Image_mfw_v54"','"Image_mfw_v56"').replace('"mfw_v55.img"','"mfw_v56.img"')
(w/'bm_mfw_v56.py').write_text(s,newline='\n')
print('Prepared video-mode no-retrigger kernel change; DTv15 unchanged')
