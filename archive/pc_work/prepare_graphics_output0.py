from pathlib import Path
b=Path('/mnt/d/y5_linux_work')
s=(b/'pco_eot_probe.c').read_text().replace('0x70004000','0x70000000').replace('pco-eot20.bin','graphics-eot-output0.bin').replace('pco-eot20.ir','graphics-eot-output0.ir');(b/'pco_eot_output0_probe.c').write_text(s,newline='\n')
s=(b/'build_pco_eot20.py').read_text().replace('pco_eot_probe.c','pco_eot_output0_probe.c').replace('mfw-pco-eot20','mfw-pco-eot-output0');(b/'build_pco_eot_output0.py').write_text(s,newline='\n')
