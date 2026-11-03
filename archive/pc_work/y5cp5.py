K='/home/y5/linux-7.3/'
def rw(p,f):
    s=open(K+p).read(); n=f(s); open(K+p,'w').write(n)
def sub(s,pairs):
    for a,b in pairs:
        assert s.count(a)==1,a
        s=s.replace(a,b)
    return s
rw('arch/arm64/kernel/head.S',lambda s:sub(s,[('mov\tw10, #0xa000','mov\tw10, #0x1000'),('y5cp\t35\n','y5cp\t4\n'),('y5cp\t50\n','y5cp\t6\n'),('y5cp\t110\n','y5cp\t14\n'),('y5cp\t125\n','y5cp\t16\n'),('y5cp\t140\n','y5cp\t18\n')]))
rw('arch/arm64/kernel/pi/map_range.c',lambda s:sub(s,[('Y5W(65)','Y5W(8)'),('Y5W(80)','Y5W(10)'),('Y5W(95)','Y5W(12)')]))
rw('arch/arm64/kernel/setup.c',lambda s:sub(s,[('y5_cp_regs(y5b, 160)','y5_cp_regs(y5b, 20)')]))
rw('init/main.c',lambda s:sub(s,[('y5_cp(180);','y5_cp(22);'),('y5_cp(200);','y5_cp(24);'),('y5_cp(220);','y5_cp(26);'),('y5_cp(240);','y5_cp(28);'),('y5_cp(260);','y5_cp(30);')]))
print('ok')
