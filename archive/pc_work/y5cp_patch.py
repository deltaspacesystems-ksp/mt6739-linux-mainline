import re
K='/home/y5/linux-7.3/'
def rw(p,f):
    s=open(K+p).read(); n=f(s); open(K+p,'w').write(n)
open(K+'include/linux/y5cp.h','w').write('''#ifndef _LINUX_Y5CP_H
#define _LINUX_Y5CP_H
/* Y5 debug: punkt kontrolny = re-arm watchdog TOPRGU na `sec` sekund (restart po tym czasie, jesli kernel dalej nie dojdzie). */
#include <linux/io.h>
static inline void y5_cp_regs(void __iomem *b, u32 sec)
{
	u32 m = readl(b);
	writel((sec << 11) | 0x8, b + 4);
	writel(0x1971, b + 8);
	writel(0x22000000 | (m & 0xff) | 1, b);
}
static inline void y5_cp(u32 sec)
{
	void __iomem *b = ioremap(0x10007000, 0x100);
	if (b) { y5_cp_regs(b, sec); iounmap(b); }
}
#endif
''')
# head.S: zamiast wylaczac, uzbroj 20 s (zachowaj bity trybu z LK)
def head(s):
    old='''	movz	w10, #0x2200, lsl #16
	str	w10, [x9]
'''
    new='''	ldr	w12, [x9]
	mov	w10, #0xa000
	orr	w10, w10, #8
	str	w10, [x9, #4]
	mov	w10, #0x1971
	str	w10, [x9, #8]
	and	w12, w12, #0xff
	orr	w12, w12, #1
	movz	w10, #0x2200, lsl #16
	orr	w12, w12, w10
	str	w12, [x9]
'''
    assert s.count(old)==1; return s.replace(old,new)
rw('arch/arm64/kernel/head.S',head)
def setup(s):
    s=s.replace('#include <linux/efi.h>','#include <linux/efi.h>\n#include <linux/y5cp.h>',1) if '#include <linux/efi.h>' in s else '#include <linux/y5cp.h>\n'+s
    old='	early_ioremap_init();\n'
    assert s.count(old)==1
    return s.replace(old,old+'	{ void __iomem *y5b = early_ioremap(0x10007000, 0x100); y5_cp_regs(y5b, 40); early_iounmap(y5b, 0x100); }\n')
rw('arch/arm64/kernel/setup.c',setup)
def main(s):
    s=s.replace('#include <linux/kernel.h>','#include <linux/kernel.h>\n#include <linux/y5cp.h>',1)
    def ins(after,sec):
        nonlocal s
        assert s.count(after)==1,after
        s=s.replace(after,after+'\n	y5_cp(%d);'%sec)
    ins('	mm_core_init();',60)
    ins('	init_IRQ();',80)
    ins('	time_init();',100)
    ins('	init_userspace_fs();',120)
    ins('	kernel_init_freeable();',140)
    return s
rw('init/main.c',main)
print('ok')
