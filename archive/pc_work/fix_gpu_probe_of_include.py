from pathlib import Path
p=Path(r'D:\y5_linux_work\microfw-gpu\mfw-gpu-id-probe.c');s=p.read_text().replace('#include <linux/of_platform.h>','#include <linux/of.h>\n#include <linux/of_platform.h>');p.write_text(s,newline='\n')
