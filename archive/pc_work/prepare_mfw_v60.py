from pathlib import Path
base = Path('D:/y5_linux_work')
s = (base / 'bm_mfw_v59.py').read_text()
s = s.replace('Image_mfw_v59', 'Image_mfw_v60').replace('v18.dtb', 'v19.dtb').replace('mfw_v59.img', 'mfw_v60.img')
(base / 'bm_mfw_v60.py').write_text(s, newline='\n')
source = (base / 'verify_mfw_v59.py').read_text()
parser = source[source.index('def props('):source.index('import json\nop=props(')]
(base / 'out/fdt_props_v60.py').write_text('import struct\n' + parser, newline='\n')
print('Boot builder derived; all initramfs content retained for byte comparison')
