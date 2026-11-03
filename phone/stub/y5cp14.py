K='/home/y5/linux-7.3/'
def rw(p,f):
    s=open(K+p).read(); n=f(s); open(K+p,'w').write(n)
def mn(s):
    assert s.count('y5_cp_off();')==1
    return s.replace('y5_cp_off();','y5_cp(3);',1)
rw('init/main.c',mn)
def pn(s):
    a='void vpanic(const char *fmt, va_list args)\n{\n'
    assert s.count(a)==1
    return s.replace(a,a+'	if (y5_wdt_base)\n		y5_cp_regs(y5_wdt_base, 8);\n',1)
rw('kernel/panic.c',pn)
print('ok')
