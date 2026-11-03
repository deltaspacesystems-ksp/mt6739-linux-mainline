from pathlib import Path
import hashlib
import json
import re
base=Path('D:/y5_linux_work')
reference=base/'sources/rogue-ddk19'
config=reference/'hwdefs/km/configs/rgxconfig_km_22.V.21.16.h'
core=reference/'hwdefs/km/cores/rgxcore_km_22.49.21.16.h'
features={}
for match in re.finditer(r'^#define RGX_FEATURE_([A-Z0-9_]+)([^\n]*)',config.read_text(),re.M):
    name,value=match.groups(); value=value.strip()
    if not value: features[name]=True
    elif re.fullmatch(r'\(\d+\)',value): features[name]=int(value[1:-1])
    else: features[name]={'unparsed':value}
mesa=(base/'sources/mesa-device-reference/pvr_device_info.h').read_text()
known={}
unmapped=[]
for name,value in features.items():
    key=name.lower()
    if re.search(r'\bhas_'+re.escape(key)+r'\b',mesa):
        known['has_'+key]=True
        if not isinstance(value,bool): known[key]=value
    else: unmapped.append(name)
errata=[int(n) for n in re.findall(r'^#define FIX_HW_BRN_(\d+)',core.read_text(),re.M)]
enhancements=[int(n) for n in re.findall(r'^#define HW_ERN_(\d+)',core.read_text(),re.M)]
result={
 'target_bvnc':'22.49.21.16', 'bvnc_confidence':'Measured on v59 hardware, see gpu-hwid-v59.json',
 'soc_marketing_gpu':'PowerVR GE8100 (MediaTek MT6739 specification, not GE8300)',
 'soc_primary_url':'https://www.mediatek.com/press-room/full-featured-mediatek-smartphone-chipset-mt6739-debuts-in-india',
 'feature_confidence':'Pinned DDK generated config/core reference; features are not independently hardware-probed',
 'reference_commit':'096323c2d5db8a05e84df31608fc0c9d22e47b31',
 'config_sha256':hashlib.sha256(config.read_bytes()).hexdigest(), 'core_sha256':hashlib.sha256(core.read_bytes()).hexdigest(),
 'ddk_kernel_features':features, 'ddk_errata':errata, 'ddk_enhancements':enhancements,
 'mesa_fields_with_direct_name_and_value_evidence':known, 'ddk_features_without_direct_mesa_field':unmapped,
 'warning':'Not a complete Mesa device profile. Missing usermode/compiler/tile/USC parameters and unknown features must not be defaulted to false or borrowed from GE8300.',
 'native_mesa_bvnc_present_in_pinned_reference':False,
}
assert '22_49_21_16' not in (base/'sources/mesa-device-reference/pvr_device_info.c').read_text()
(Path('X:/y5linux/phone/hardware/gpu-ge8100-feature-reference.json')).write_text(json.dumps(result,indent=2)+'\n',newline='\n')
print('Recorded',len(features),'DDK features,',len(known),'direct Mesa fields,',len(errata),'errata; incomplete profile stays non-executable')
