K='/home/y5/linux-7.3/'
s=open(K+'init/main.c').read()
s=s.replace('#include <linux/y5cp.h>\n#include <linux/delay.h>','#include <linux/y5cp.h>',1)
a='#include <linux/kernel.h>'
assert s.count(a)==1
s=s.replace(a,'#include <linux/delay.h>\n'+a,1)
open(K+'init/main.c','w').write(s)
print('ok')
