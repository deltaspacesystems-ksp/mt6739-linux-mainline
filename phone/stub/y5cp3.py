K='/home/y5/linux-7.3/'
def rw(p,f):
    s=open(K+p).read(); n=f(s); open(K+p,'w').write(n)
def head(s):
    s=s.replace('y5cp\t50','y5cp\t95').replace('y5cp\t40','y5cp\t80').replace('y5cp\t30','y5cp\t65')
    a='	bl	record_mmu_state\n'; b='	bl	preserve_boot_args\n'
    assert s.count(a)==1 and s.count(b)==1
    s=s.replace(a,a+'	y5cp	35\n').replace(b,b+'	y5cp	50\n')
    return s
rw('arch/arm64/kernel/head.S',head)
rw('arch/arm64/kernel/setup.c',lambda s:s.replace('y5_cp_regs(y5b, 70)','y5_cp_regs(y5b, 120)'))
def m(s):
    for a,b in ((170,220),(150,200),(130,180),(110,160),(90,140)):
        s=s.replace('y5_cp(%d);'%a,'y5_cp(%d);'%b)
    return s
rw('init/main.c',m)
