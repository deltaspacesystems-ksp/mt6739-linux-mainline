import re, struct
d = open(r"D:\y5_linux_work\stock.dtb","rb").read()
magic,total,off_struct,off_str,off_rsv,ver,lastcomp,bootcpu,sz_str,sz_struct = struct.unpack_from('>10I', d, 0)
strs = d[off_str:off_str+sz_str]
def cs(b,o):
    e=b.index(b'\0',o); return b[o:e].decode(errors='replace'), e+1
def val(v):
    if v and v.endswith(b'\0') and all(32<=c<127 or c==0 for c in v): return ", ".join('"%s"'%s.decode() for s in v[:-1].split(b'\0'))
    if len(v)%4==0 and v: return "<"+" ".join("0x%x"%x for x in struct.unpack('>%dI'%(len(v)//4),v))+">"
    return "[%s]"%v.hex()
o=off_struct; path=[]; rows=[]
while True:
    tag,=struct.unpack_from('>I',d,o); o+=4
    if tag==1: n,o=cs(d,o); o=(o+3)&~3; path.append(n or "/")
    elif tag==2: path.pop()
    elif tag==3:
        ln,no=struct.unpack_from('>II',d,o); o+=8; v=d[o:o+ln]; o=(o+ln+3)&~3; pn,_=cs(strs,no)
        rows.append(("/"+"/".join(path[1:]),pn,val(v)))
    elif tag==9: break
def show(prefix, props=None, limit=60):
    n=0
    for p,k,v in rows:
        if p.startswith(prefix) and (props is None or k in props) and n<limit:
            print(f"{p} :: {k} = {v}"); n+=1
print("=== top-level #cells / compatible / model"); show("/", {"compatible","model","#address-cells","#size-cells","interrupt-parent"}, 8)
print("\n=== psci"); show("//psci" if False else "/psci")
print("\n=== timer"); show("/timer")
print("\n=== gic"); show("/interrupt-controller@0c000000", None, 20)
print("\n=== cpus"); show("/cpus", {"compatible","reg","device_type","enable-method","clock-frequency","cpu-release-addr","capacity-dmips-mhz"}, 40)
print("\n=== uart0"); show("/apuart0@11002000", None, 12)
print("\n=== uart1"); show("/apuart1@11003000", None, 8)
