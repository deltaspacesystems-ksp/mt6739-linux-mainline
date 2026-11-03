from pathlib import Path
import shutil
base=Path('D:/y5_linux_work')
s=(base/'bm_mfw_v60.py').read_text().replace('Image_mfw_v60','Image_mfw_v61').replace('mfw_v60.img','mfw_v61.img')
(base/'bm_mfw_v61.py').write_text(s,newline='\n')
shutil.copyfile(base/'export/config-7.3-mfw-v60',base/'export/config-7.3-mfw-v61')
s=(base/'verify_mfw_v60.py').read_text()
s=s.replace("old = (base / 'mfw_v59.img')", "old = (base / 'mfw_v60.img')").replace("new = (base / 'mfw_v60.img')", "new = (base / 'mfw_v61.img')")
s=s.replace('Image_mfw_v60','Image_mfw_v61')
start=s.index('op, np = props(od), props(nd)')
end=s.index('for marker in ',start)
s=s[:start]+'''assert nd == od, 'DT must be byte-identical to v60'
assert (base / 'export/config-7.3-mfw-v60').read_bytes() == (base / 'export/config-7.3-mfw-v61').read_bytes()
'''+s[end:]
s=s.replace("'dt_delta':'new GPU SPI226+low and ID-probe disabled only'", "'dt_delta':'none, identical v60'")
s=s.replace("'config_delta':'DRM_MFW_ROGUE_DDK19=y only'", "'config_delta':'none, identical v60'")
s=s.replace("'initramfs_addresses_cmdline':'identical v59'", "'initramfs_addresses_cmdline':'identical v60'")
s=s.replace('mfw-v60-validation.json','mfw-v61-validation.json')
(base/'verify_mfw_v61.py').write_text(s,newline='\n')
s=(base/'run_v60_native_firmware_trial.sh').read_text().replace("'#45 '*", "'#46 '*")
(base/'run_v61_native_firmware_trial.sh').write_text(s,newline='\n')
print('V61 builder/validator/trial derived, exact v60 DT/config/initramfs retained')
