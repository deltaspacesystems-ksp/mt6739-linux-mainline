from pathlib import Path
import json
b=Path('D:/y5_linux_work');s=(b/'render19_freelist_pair_trial.c').read_text();a=s.index('int main(void){');s=s[:a]+s[a:]
s=s.replace('struct arena arenas[3]={0};struct page pages[12]={0};for(unsigned i=0;i<12;i++)','struct arena arenas[5]={0};struct page pages[32]={0};for(unsigned i=0;i<32;i++)')
s=s.replace('int slot=!strcmp(name,"General")?0:!strcmp(name,"PDS Code and Data")?1:!strcmp(name,"USC Code")?2:-1;','int slot=!strcmp(name,"General")?0:!strcmp(name,"PDS Code and Data")?1:!strcmp(name,"USC Code")?2:!strcmp(name,"MMU INIA BRN65273")?3:!strcmp(name,"MMU INIB BRN65273")?4:-1;')
s=s.replace('for(unsigned i=0;i<3;i++){','for(unsigned i=0;i<5;i++){').replace('for(unsigned i=0;i<12;i++){','for(unsigned i=0;i<32;i++){')
plan=[]
for i in range(15):plan.append((0,i*4096,i in [13,14]))
for off in [0x10000,0x12000,0x20000]+list(range(0x30000,0x3a000,4096)):plan.append((1,off,False))
plan += [(2,0x10000,False),(2,0x11000,False),(3,0,False),(4,0,False)]
assert len(plan)==32
old='unsigned a=i<8||i>=10?0:i==8?1:2;pages[i].arena=a;pages[i].va=arenas[a].base+(i<8||i>=10?i*4096:65536);'
assert old in s
rows=','.join('{%d,%d,%d}'%(ar,off,prot) for ar,off,prot in plan)
s=s.replace('int main(void){','struct plan_entry {unsigned arena,offset,protected_table;};\nstatic const struct plan_entry plan[32]={'+rows+'};\nint main(void){')
s=s.replace(old,'unsigned a=plan[i].arena;pages[i].arena=a;pages[i].va=arenas[a].base+plan[i].offset;')
s=s.replace('if(i>=10)','if(plan[i].protected_table)').replace('if(i<10)','if(!plan[i].protected_table)')
a=s.index(' uint8_t framework[16]');z=s.index('done:\n',a);s=s[:a]+' puts("GRAPHICS_MEMORY32_PASS actual5heaps contiguousVA full render candidate footprint; no shader/job");rc=0;\n'+s[z:]
s=s.replace('for(int i=11;i>=0;i--)','for(int i=31;i>=0;i--)').replace('for(int i=2;i>=0;i--)','for(int i=4;i>=0;i--)').replace('FREELIST_PAIR_RC','GRAPHICS_MEMORY32_RC')
(b/'graphics19_memory_preflight.c').write_text(s,newline='\n')
(b/'out/graphics-memory32-plan.json').write_text(json.dumps({'pages':len(plan),'entries':[{'arena':ar,'offset':hex(off),'protected_table':prot} for ar,off,prot in plan]},indent=2)+'\n')
