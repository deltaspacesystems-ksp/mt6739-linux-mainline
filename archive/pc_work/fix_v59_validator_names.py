from pathlib import Path
p=Path(r'D:\y5_linux_work\verify_mfw_v59.py');s=p.read_text().replace('new={(p,n):v','new_props={(p,n):v').replace('assert new[(','assert new_props[(').replace(' in new if p==path',' in new_props if p==path').replace(' if k not in new}',' if k not in new_props}');p.write_text(s,newline='\n')
