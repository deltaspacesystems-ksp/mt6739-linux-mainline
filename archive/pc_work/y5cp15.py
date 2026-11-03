K='/home/y5/linux-7.3/'
def rw(p,f):
    s=open(K+p).read(); n=f(s); open(K+p,'w').write(n)
rw('init/main.c',lambda s:s.replace('y5_cp(3);','y5_cp(1);',1) if s.count('y5_cp(3);')==1 else (_ for _ in ()).throw(Exception('main')))
rw('kernel/panic.c',lambda s:s.replace('y5_cp_regs(y5_wdt_base, 8)','y5_cp_regs(y5_wdt_base, 3)',1))
print('ok')
