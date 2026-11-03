from pathlib import Path
import json,shlex,subprocess
r=Path('/home/y5/mesa-native'); b=r/'build-mfw'
s=(r/'src/imagination/vulkan/pds/pvr_pds.c').read_text()
def function(name):
    pos=s.index(name+'('); start=s.rfind('static ALWAYS_INLINE',0,pos); begin=s.index('{',pos); depth=1; end=begin+1
    while depth:
        depth+=(s[end]=='{')-(s[end]=='}'); end+=1
    return s[start:end]
license=s[:s.index('*/')+2]
code=license+'\n#include <assert.h>\n#include <stdio.h>\n#include "pds/pvr_rogue_pds_encode.h"\n#define __pvr_address_type uint64_t\n#define __pvr_get_address(a) (a)\n#define __pvr_make_address(a) (a)\n#include "csbgen/rogue/pds.h"\n'+function('pvr_pds_inst_encode_doutc')+'\n'+function('pvr_pds_encode_bra')+'\n'+function('pvr_pds_encode_doutu')+'''
int main(void) {
uint32_t got[]={
pvr_pds_encode_bra(PVR_ROGUE_PDSINST_PREDICATE_IF0,PVR_ROGUE_PDSINST_BRA_NEG_DISABLE,PVR_ROGUE_PDSINST_PREDICATE_IF1,1),
pvr_pds_inst_encode_halt(1),
pvr_pds_encode_bra(PVR_ROGUE_PDSINST_PREDICATE_IF0,PVR_ROGUE_PDSINST_BRA_NEG_DISABLE,PVR_ROGUE_PDSINST_PREDICATE_IF0,1),
pvr_pds_inst_encode_doutc(0,0),pvr_pds_inst_encode_halt(0)};
/* Independently observed legacy instruction words; no vendor code executed. */
const uint32_t expected[]={0xc1100001,0xdb000000,0xc1080001,0xf0000005,0xd3000000};
for(unsigned i=0;i<5;i++){printf("%u: %08x expected %08x\\n",i,got[i],expected[i]);assert(got[i]==expected[i]);}
for(unsigned cc=0;cc<2;cc++) for(unsigned offset=0;offset<=254;offset+=2) {
uint32_t modern=pvr_pds_encode_doutu(cc,1,offset>>1);
uint32_t legacy=((offset&0xfe)<<7)|(cc<<27)|0xf4000002;
assert(modern==legacy);
}
for(unsigned units=0;units<64;units++) for(unsigned rate=0;rate<3;rate++) for(unsigned dual=0;dual<2;dual++) {
struct ROGUE_PDSINST_DOUTU_SRC0 v={.temps=units,.sample_rate=rate,.dual_phase=dual,.exe_off=0xfffffffc}, out;
uint32_t words[2];ROGUE_PDSINST_DOUTU_SRC0_pack(words,&v);
ROGUE_PDSINST_DOUTU_SRC0_unpack(words,&out);
assert(out.temps==units && out.sample_rate==rate && out.dual_phase==dual && out.exe_off==0xfffffffc);
assert(words[0]==0xfffffffc && words[1]==((units<<3)|(rate<<1)|(dual<<9)));
}
puts("PASS: generated XML pack/unpack preserves 384 encoded-unit/rate/phase combinations without rescaling");
const uint64_t task_base[]={UINT64_C(0x1000000000),UINT64_C(0x2800000000)};
const unsigned units[]={2,5};
for(unsigned i=0;i<2;i++) {
assert(((task_base[i]&~PVR_ROGUE_PDSINST_DOUT_FIELDS_DOUTU_SRC0_TEMPS_CLRMSK)>>PVR_ROGUE_PDSINST_DOUT_FIELDS_DOUTU_SRC0_TEMPS_SHIFT)==units[i]);
assert((task_base[i]&~PVR_ROGUE_PDSINST_DOUT_FIELDS_DOUTU_SRC0_SAMPLE_RATE_CLRMSK)==0);
assert((task_base[i]&PVR_ROGUE_PDSINST_DOUT_FIELDS_DOUTU_SRC0_DUAL_PHASE_EN)==0);
assert((task_base[i]&~PVR_ROGUE_PDSINST_DOUT_FIELDS_DOUTU_SRC0_EXE_OFF_CLRMSK)==0);
}
puts("PASS: two legacy USC task bases decode to temp units 2/5, instance rate, no dual phase");
puts("PASS: five fence words and 256 aligned DOUTU offset/predicate pairs; no GPU dispatch");
}
'''
Path('/tmp/mfw-pds-compare.c').write_text(code)
entries=json.loads((b/'compile_commands.json').read_text()); e=next(x for x in entries if x['file'].endswith('/pvr_pds.c'))
a=shlex.split(e['command']);a[a.index('-o')+1]='/tmp/mfw-pds-compare.o';a[a.index('-c')+1]='/tmp/mfw-pds-compare.c'
subprocess.run(a,cwd=b,check=True)
subprocess.run(['cc','/tmp/mfw-pds-compare.o','-o','/tmp/mfw-pds-compare'],check=True)
subprocess.run(['/tmp/mfw-pds-compare'],check=True)
