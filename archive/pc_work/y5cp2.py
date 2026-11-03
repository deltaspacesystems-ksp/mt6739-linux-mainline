K='/home/y5/linux-7.3/'
def rw(p,f):
    s=open(K+p).read(); n=f(s); open(K+p,'w').write(n)
mac='''.macro	y5cp, sec
	mov	x9, #0x7000
	movk	x9, #0x1000, lsl #16
	ldr	w12, [x9]
	mov	w10, #((\\sec << 11) & 0xffff)
	movk	w10, #((\\sec << 11) >> 16), lsl #16
	orr	w10, w10, #8
	str	w10, [x9, #4]
	mov	w10, #0x1971
	str	w10, [x9, #8]
	and	w12, w12, #0xff
	orr	w12, w12, #1
	movz	w10, #0x2200, lsl #16
	orr	w12, w12, w10
	str	w12, [x9]
.endm
'''
def head(s):
    a='SYM_CODE_START(primary_entry)\n'
    assert s.count(a)==1
    s=s.replace(a,mac+a)
    s=s.replace('	bl	__pi_create_init_idmap\n','	bl	__pi_create_init_idmap\n	y5cp	30\n',1)
    s=s.replace('	bl	init_kernel_el			// w0=cpu_boot_mode\n	mov	x20, x0\n','	bl	init_kernel_el			// w0=cpu_boot_mode\n	mov	x20, x0\n	y5cp	40\n',1)
    s=s.replace('	bl	__cpu_setup			// initialise processor\n	b	__primary_switch\n','	bl	__cpu_setup			// initialise processor\n	y5cp	50\n	b	__primary_switch\n',1)
    assert s.count('y5cp\t')==3 or s.count('y5cp\t')>=3
    return s
rw('arch/arm64/kernel/head.S',head)
rw('arch/arm64/kernel/setup.c',lambda s:s.replace('y5_cp_regs(y5b, 40)','y5_cp_regs(y5b, 70)'))
def m(s):
    for a,b in ((60,90),(80,110),(100,130),(120,150),(140,170)):
        s=s.replace('y5_cp(%d);'%a,'y5_cp(%d);'%b)
    return s
rw('init/main.c',m)
print('ok')
