import re
s=open('/home/y5/linux-7.3/lib/fonts/font_8x16.c').read()
i=s.index('fontdata_8x16')
body=s[s.index('{',i):s.index('};',i)]
body=re.sub(r'/\*.*?\*/','',body,flags=re.S)
vals=re.findall(r'0x[0-9a-fA-F]{2}',body)
assert len(vals)==4096,len(vals)
print(len(vals))
open('/mnt/d/y5_linux_work/font8x16.h','w').write('static const unsigned char font8x16[%d] = {%s};\n'%(len(vals),','.join(vals)))
