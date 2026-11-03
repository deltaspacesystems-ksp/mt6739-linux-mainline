b = open(r"D:\y5_linux_work\log_dump.bin",'rb').read()
n = int(b[7:17]); txt = b[18:18+n].decode(errors='replace')
open(r"D:\y5_linux_work\report.txt",'w',encoding='utf-8').write(txt)
print("report chars", len(txt), "lines", txt.count("\n"))
