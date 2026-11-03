from pathlib import Path
import re,json
root=Path('/home/y5/mesa-native/src/imagination/pco');rows=[]
for p in sorted(root.rglob('*')):
 if p.suffix not in ('.c','.h') or 'uscgen' in p.parts:continue
 s=p.read_text();s=re.sub(r'/\*.*?\*/',lambda m:'\n'*m[0].count('\n'),s,flags=re.S);s=re.sub(r'//[^\n]*','',s)
 for n,line in enumerate(s.splitlines(),1):
  if re.search(r'PVR_(HAS_FEATURE|GET_FEATURE_VALUE|HAS_QUIRK|HAS_ENHANCEMENT)\(|rogue_(get_temps|max_wg_temps)\(',line):rows.append({'file':str(p.relative_to(root)),'line':n,'expression':line.strip()})
d={'scope':'Lexical C/H device-query sites in pinned PCO, excluding uscgen profile generator and comments; not whole-program reachability proof','sites':rows,'restricted_compute_analysis':{'probe':'normal compute, explicitWG1, one global store, no texture/interpolation/barriers','direct_queries':'usc_itrsmp_enhanced in interpolation helper; quirk74056 in texture lowering. These features are not general compute-capacity inputs. Must establish helper unreachability for each tested shader.','transitive_queries':'RA calls rogue_get_temps -> unified_store_depth; compute adds rogue_max_wg_temps -> usc_slots max-instance assertion, barrier arithmetic only whenWG>32 and barriers','demand':'Current offline normal store precomp metadata temps5,vtxins1,shareds0,128B at synthetic64/4; not guaranteed physical demand under unknown ISA/errata','remaining':'Prove selected instructions and target errata, safe allocation budget, exact submission packet/context program contract. Do not fill74 unknown features withfalse or declare full profile unnecessary.'}}
Path('/mnt/d/y5_linux_work/gpu-pco-query-sites.json').write_text(json.dumps(d,indent=2)+'\n');print(json.dumps(rows,indent=2))
