K='/home/y5/linux-7.3/'
s=open(K+'init/main.c').read()
s=s.replace('void __iomem *y5_wdt_base;\nvoid start_kernel(void)\n','void start_kernel(void)\n',1)
a='#include <linux/y5cp.h>'
assert s.count(a)==1
s=s.replace(a,a+'\nvoid __iomem *y5_wdt_base;',1)
open(K+'init/main.c','w').write(s)
print('ok')
