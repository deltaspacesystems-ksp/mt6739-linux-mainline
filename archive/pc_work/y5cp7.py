K='/home/y5/linux-7.3/'
def rw(p,f):
    s=open(K+p).read(); n=f(s); open(K+p,'w').write(n)
def mn(s):
    a='void start_kernel(void)\n'
    assert s.count(a)==1
    s=s.replace(a,'void __iomem *y5_wdt_base;\n'+a)
    b='	y5_cp(22);'
    assert s.count(b)==1
    return s.replace(b,b+'\n	y5_wdt_base = ioremap(0x10007000, 0x100);')
rw('init/main.c',mn)
def pn(s):
    a='void vpanic(const char *fmt, va_list args)\n{\n'
    assert s.count(a)==1
    s=s.replace(a,'extern void __iomem *y5_wdt_base;\n'+a+'	if (y5_wdt_base)\n		y5_cp_regs(y5_wdt_base, 5);\n')
    return s.replace('#include <linux/interrupt.h>','#include <linux/interrupt.h>\n#include <linux/y5cp.h>',1)
rw('kernel/panic.c',pn)
print('ok')
