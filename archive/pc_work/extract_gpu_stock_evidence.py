"""Fixed GPU evidence from stock kernel CODE and DT, not device partitions."""
from pathlib import Path
import hashlib,json,re
work=Path(r'D:\y5_linux_work')
blob=(work/'out/stock_Image').read_bytes()
assert blob[56:60]==b'ARM\x64'
needle=b'22.49.21.16\0'
offsets=[m.start() for m in re.finditer(re.escape(needle),blob)]
assert offsets
ddk=re.search(rb'Rogue_DDK_[\x20-\x7e]{1,80}',blob)
assert ddk
text=(work/'out/stock_0.dts').read_text()
node=re.search(r'\tmfg_auckland@13000000 \{.*?\n\t\};',text,re.S)
assert node
result=dict(source_sha256=hashlib.sha256(blob).hexdigest(),compiled_bvnc_candidate='22.49.21.16',
            string_va=[hex(0xffffff8008080000+x) for x in offsets],ddk_build=ddk.group().decode(),
            stock_gpu_node=node.group().strip(),
            confidence='Compiled string/fallback evidence only; not a hardware BVNC read or proof of mainline support.',
            required_next=['Trace stock initialization and firmware selection','Confirm actual hardware BVNC with verified power/clock path','Obtain compatible mainline firmware and device data; do not override another GPU ID'])
(work/'out/gpu-stock-evidence.json').write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
print(json.dumps(result,indent=2))
