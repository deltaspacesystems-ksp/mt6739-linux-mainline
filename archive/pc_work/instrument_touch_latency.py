from pathlib import Path
w=Path(r'D:\y5_linux_work');p=w/'mfw_mstar_touch.c';s=p.read_text()
(w/'mfw_mstar_touch-before-latency.c').write_text(s,newline='\n')
s=s.replace('int active[10]={0},next=1,ret=0,last_count=-1;','int active[10]={0},next=1,ret=0,last_count=-1;unsigned bus_errors=0,invalid_reports=0;uint64_t longest_contact_gap=0;')
s=s.replace('if(packet(p)||decode(p,43,c)){','int bus_error=packet(p);int invalid=bus_error?0:decode(p,43,c);\n   if(bus_error||invalid){\n    if(bus_error)bus_errors++;else invalid_reports++;')
s=s.replace('good++;last_valid=ns();','''uint64_t now=ns();int was_held=0;for(unsigned i=0;i<10;i++)was_held|=active[i]!=0;
    if(was_held&&now-last_valid>longest_contact_gap)longest_contact_gap=now-last_valid;
    good++;last_valid=now;''')
s=s.replace('printf("CONTACT_COUNT=%d irq_high=%d good=%u bad=%u\\n",count,irq_high,good,bad);','printf("CONTACT_COUNT=%d irq_high=%d good=%u bad=%u monotonic_ms=%llu\\n",count,irq_high,good,bad,(unsigned long long)(last_valid/1000000));')
s=s.replace('printf("TOUCH_READER_EXIT good=', 'printf("TOUCH_TIMING bus_errors=%u invalid_reports=%u longest_contact_gap_us=%llu\\n",bus_errors,invalid_reports,(unsigned long long)(longest_contact_gap/1000));\n printf("TOUCH_READER_EXIT good=')
assert 'TOUCH_TIMING' in s and 'monotonic_ms' in s
p.write_text(s,newline='\n')
