from pathlib import Path
p=Path(r'D:\y5_linux_work\export\y5-pmic-v59.c');s=p.read_text().replace('#include <linux/property.h>','#include <linux/property.h>\n#include <linux/of.h>');p.write_text(s,newline='\n')
