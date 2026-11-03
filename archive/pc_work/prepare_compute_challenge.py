from pathlib import Path
import hashlib
b=Path('D:/y5_linux_work');old=Path('//wsl.localhost/archlinux/tmp/mfw-pco-restricted.bin').read_bytes();assert hashlib.sha256(old).hexdigest()=='3c15b86129d126c263fbd831312216f468ba4897068c95d7b8ea23fdeddba646';(b/'out/compute-store-initial-budget8.bin').write_bytes(old)
s=(b/'pco_restricted_probe.c').read_text();assert s.count('0x12345678')==1 and s.count('0xc0000000u')==1;s=s.replace('0x12345678','0xa1b2c3d4').replace('0xc0000000u','0xc0000040u');(b/'pco_restricted_probe_challenge.c').write_text(s)
s=(b/'build_pco_budget8.py').read_text().replace('/pco_restricted_probe.c','/pco_restricted_probe_challenge.c').replace('mfw-pco-budget8','mfw-pco-challenge').replace('pco-budget8-result.txt','pco-challenge-result.txt');(b/'build_pco_challenge.py').write_text(s)
