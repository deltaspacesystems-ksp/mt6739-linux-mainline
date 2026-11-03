// SPDX-License-Identifier: GPL-2.0
/*
 * y5disp: reczne wyzwalanie odswiezania panelu DSI na MT6739 (Huawei Y5 2018).
 * Command-mode bootstrap (video mode never retriggered): legacy sequence (mutex + RDMA + DSI start). Sekwencja zmierzona z uzytkownikowego y5txt:
 *   MUTEX0 EN   0x14015020 = 1
 *   RDMA0 start 0x1400b010 = 1
 *   DSI0 START  0x14012000 = 0, potem 1
 * Wyzwalanie: y5disp_kick() z simpledrm po zmianie bufora (z odstepem >= 40 ms). Tymczasowe, do czasu prawdziwego sterownika DRM/DSI.
 * Sterowanie: /sys/kernel/y5disp/{hz,trigger}.
 */
#include <linux/init.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/workqueue.h>

static void __iomem *y5d_mtx, *y5d_rdma, *y5d_dsi;
static unsigned int y5d_hz;		/* 0 = bez cyklicznego odswiezania (domyslnie): wyzwalamy tylko po zmianie bufora */
static bool y5d_ready;
static unsigned long y5d_last;
static struct delayed_work y5d_work;

/* DSI_MODE_CTRL[1:0]:0=command,1/2/3=continuous video.
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
  return;
	writel(1, y5d_mtx + 0x20);
	writel(1, y5d_rdma + 0x10);
	writel(0, y5d_dsi);
	writel(1, y5d_dsi);
}

static void y5d_fn(struct work_struct *w)
{
	unsigned long min_gap = msecs_to_jiffies(40);

 /* Do not restart a running video stream or poll it periodically. */
 if (y5d_video_mode())
  return;

	/* nie wyzwalaj czesciej niz co 40 ms: powtorzone wyzwolenie w trakcie skanowania daje migotanie i polowki klatek */
	if (time_before(jiffies, y5d_last + min_gap)) {
		schedule_delayed_work(&y5d_work, y5d_last + min_gap - jiffies);
		return;
	}
	y5d_last = jiffies;
	y5d_fire();
	if (y5d_hz)
		schedule_delayed_work(&y5d_work, max(1UL, HZ / y5d_hz));
}

/* wywolywane z simpledrm po kazdej zmianie bufora; zbiera zmiany z ostatnich 25 ms w jedno wyzwolenie */
void y5disp_kick(void)
{
	if (y5d_ready && !y5d_video_mode())
		schedule_delayed_work(&y5d_work, msecs_to_jiffies(25));
}

static ssize_t mode_show(struct kobject *k, struct kobj_attribute *a, char *buf)
{
 return sysfs_emit(buf, "%u\n", readl(y5d_dsi + 0x14) & 3);
}

static ssize_t hz_show(struct kobject *k, struct kobj_attribute *a, char *buf)
{
	return sysfs_emit(buf, "%u\n", y5d_hz);
}

static ssize_t hz_store(struct kobject *k, struct kobj_attribute *a, const char *buf, size_t n)
{
	unsigned int v;
	bool start;

	if (kstrtouint(buf, 0, &v) || v > 60)
		return -EINVAL;
	start = !y5d_hz && v;
	y5d_hz = v;
	if (start)
		schedule_delayed_work(&y5d_work, 0);
	return n;
}

static ssize_t trigger_store(struct kobject *k, struct kobj_attribute *a, const char *buf, size_t n)
{
	y5d_fire();
	return n;
}

static struct kobj_attribute y5d_mode_attr = __ATTR_RO(mode);
static struct kobj_attribute y5d_hz_attr = __ATTR_RW_MODE(hz, 0644);
static struct kobj_attribute y5d_trigger_attr = __ATTR_WO(trigger);

static int __init y5d_init(void)
{
	struct kobject *kobj;

	y5d_mtx = ioremap(0x14015000, 0x1000);
	y5d_rdma = ioremap(0x1400b000, 0x1000);
	y5d_dsi = ioremap(0x14012000, 0x1000);
	if (!y5d_mtx || !y5d_rdma || !y5d_dsi)
		return -ENOMEM;

	INIT_DELAYED_WORK(&y5d_work, y5d_fn);
	kobj = kobject_create_and_add("y5disp", kernel_kobj);
	if (kobj) {
		sysfs_create_file(kobj, &y5d_mode_attr.attr);
		sysfs_create_file(kobj, &y5d_hz_attr.attr);
		sysfs_create_file(kobj, &y5d_trigger_attr.attr);
	}
	y5d_ready = true;
	schedule_delayed_work(&y5d_work, HZ / 2);		/* pierwsze wyzwolenie: pokaz to, co fbcon juz narysowal */
	if (y5d_video_mode())
  pr_info("y5disp: continuous video scanout; manual kicks disabled\n");
 else
  pr_info("y5disp: command mode; refresh after framebuffer updates\n");
	return 0;
}
late_initcall(y5d_init);
