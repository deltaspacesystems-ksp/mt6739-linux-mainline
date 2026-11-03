K='/home/y5/linux-7.3/'
def rw(p,f):
    s=open(K+p).read(); n=f(s); open(K+p,'w').write(n)
def head(s):
    for a,b in ((95,140),(80,125),(65,110)):
        assert s.count('y5cp\t%d\n'%a)==1; s=s.replace('y5cp\t%d\n'%a,'y5cp\t%d\n'%b)
    return s
rw('arch/arm64/kernel/head.S',head)
rw('arch/arm64/kernel/setup.c',lambda s:s.replace('y5_cp_regs(y5b, 120)','y5_cp_regs(y5b, 160)'))
def m(s):
    for a,b in ((220,260),(200,240),(180,220),(160,200),(140,180)):
        assert s.count('y5_cp(%d);'%a)==1; s=s.replace('y5_cp(%d);'%a,'y5_cp(%d);'%b)
    return s
rw('init/main.c',m)
def mr(s):
    macro='''#define Y5W(sec) do { volatile u32 *w = (volatile u32 *)0x10007000UL; u32 m = w[0]; w[1] = ((sec) << 11) | 8; w[2] = 0x1971; w[0] = 0x22000000 | (m & 0xff) | 1; } while (0)

'''
    a='asmlinkage phys_addr_t __init create_init_idmap('
    assert s.count(a)==1; s=s.replace(a,macro+a)
    b='	pgprot_val(text_prot) &= ~clrmask;\n'
    s=s.replace(b,'	Y5W(65);\n'+b,1)
    c1='''	map_range(&ptep, (u64)_stext, (u64)__initdata_begin,
		  (phys_addr_t)_stext, text_prot, IDMAP_ROOT_LEVEL,
		  (pte_t *)pg_dir, false, 0);
'''
    assert s.count(c1)==1; s=s.replace(c1,c1+'	Y5W(80);\n')
    c2='''	map_range(&ptep, (u64)__initdata_begin, (u64)_end,
		  (phys_addr_t)__initdata_begin, data_prot, IDMAP_ROOT_LEVEL,
		  (pte_t *)pg_dir, false, 0);
'''
    assert s.count(c2)==1; s=s.replace(c2,c2+'	Y5W(95);\n')
    return s
rw('arch/arm64/kernel/pi/map_range.c',mr)
print('ok')
