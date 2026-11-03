from pathlib import Path
import hashlib,re
w=Path(r'D:\y5_linux_work')
patch=(w/'export/mfw-kernel-7.3-v53.patch').read_text()
source=(w/'export/y5disp-v56.c').read_text()
header='--- /dev/null\n+++ b/drivers/video/y5disp.c\n'
start=patch.index(header)
next_header=re.search(r'^--- (?:a/[^\n]+|/dev/null)\n\+\+\+ b/',patch[start+len(header):],re.M)
end=start+len(header)+next_header.start() if next_header else len(patch)
old_source=''.join(line[1:]+'\n' for line in patch[start:end].splitlines() if line.startswith('+') and not line.startswith('+++'))
assert old_source==(w/'export/y5disp.c').read_text()
blob=source.encode();oid=hashlib.sha1(b'blob '+str(len(blob)).encode()+b'\0'+blob).hexdigest()
lines=source.splitlines()
section=header+'@@ -0,0 +1,'+str(len(lines))+' @@\n'+''.join('+'+line+'\n' for line in lines)
result=patch[:start]+section+patch[end:]
assert len(re.findall(r'^\+\+\+ b/',result,re.M))==len(re.findall(r'^\+\+\+ b/',patch,re.M))
(w/'export/mfw-kernel-7.3-v56.patch').write_text(result,newline='\n')
image=(w/'out/Image_mfw_v56').read_bytes()
assert b'continuous video scanout; manual kicks disabled' in image
assert (w/'export/config-7.3-mfw-v56').read_bytes()==(w/'export/config-7.3-mfw-v54').read_bytes()
print('PASS: full patch retains all original files; sole source delta y5disp; new message linked in Image; same config')
