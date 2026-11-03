K='/home/y5/linux-7.3/'
def rw(p,f):
    s=open(K+p).read(); n=f(s); open(K+p,'w').write(n)
rw('include/linux/y5cp.h',lambda s:s.replace('#endif','static inline void y5_cp_off(void)\n{\n\tvoid __iomem *b = ioremap(0x10007000, 0x100);\n\tif (b) { writel(0x22000000, b); iounmap(b); }\n}\n#endif',1))
rw('init/main.c',lambda s:s.replace('y5_cp(30);','y5_cp_off();',1))
print('ok')
