from pathlib import Path
w=Path(r'D:\y5_linux_work')
s=(w/'y5_touch_id_probe.c').read_text()
s=s.replace('--probe-id','--probe-mstar-detect')
s=s.replace('read_product(unsigned char data[6])','read_product(unsigned char data[6])')
s=s.replace(' const unsigned offsets[]={4,8,0x10,0x14,0x18,0x6c};',' (void)data;\n const unsigned offsets[]={4,8,0x10,0x14,0x18,0x6c};')
s=s.replace('bw(0x10,0x3a);','bw(0x10,0x28);')
s=s.replace('bw(4,0x5d<<1);bw(0x14,2);bw(0x6c,6);bw(0x18,2);','bw(4,0x62<<1);bw(0x14,1);bw(0x6c,0);bw(0x18,1);')
s=s.replace('bw(0,0x81);bw(0,0x40);bw(0x24,1);','bw(0,0x37);bw(0x24,1);')
s=s.replace('if((status&7)==1 && count==6){for(unsigned i=0;i<6;i++)data[i]=bus[0]&255;ret=0;}','if((status&7)==1 && count==0)ret=0;')
start=s.index('   /* Stock: power on')
end=s.index('   ret=read_product(data);',start)
s=s[:start]+'''   /* Stock mstar_chip_detect: INT/RST low,2.8V,50ms;
    * mstar_dev_hw_reset: HIGH10ms,LOW10ms,HIGH25ms.
    * Only stock detection command0x37 to debug address0x62.
    * No SERDB/MCU-stop/flash-unlock/chip-type routine is executed. */
   pin_out(6,0);mode6(0);pin_dir(6,1);pause_ms(50);
   pin_out(119,1);pause_ms(10);pin_out(119,0);pause_ms(10);
   pin_out(119,1);pause_ms(25);
   printf("RST119 during high: dir=%u out=%u in=%u\\n",!!(gpio[0x30/4]&(1U<<23)),!!(gpio[0x130/4]&(1U<<23)),!!(gpio[0x230/4]&(1U<<23)));
'''+s[end:]
s=s.replace('TOUCH preflight','MSTAR preflight')
old='printf("TOUCH data=%02x %02x %02x %02x %02x %02x result=%d restore=%d enable_after=%u\\n",data[0],data[1],data[2],data[3],data[4],data[5],ret,restore,after);'
new='printf("MSTAR stock_detect_tx37 addr=0x62 result=%d restore=%d enable_after=%u (ACK is not chip ID)\\n",ret,restore,after);'
assert old in s;s=s.replace(old,new)
(w/'y5_mstar_detect_probe.c').write_text(s,newline='\n')
script=(w/'vtouch_snapshot.sh').read_text().replace('timeout 3 /tmp/y5_vtouch_snapshot','timeout 3 /var/cache/mfw-ui/y5_mstar_detect_probe --probe-mstar-detect')
(w/'mstar_detect_probe.sh').write_text(script,newline='\n')
assert '0x5d<<1' not in s and 'count==6' not in s
print('Prepared stock MStar ACK-detect only; no debug session or chip-type flash commands')
