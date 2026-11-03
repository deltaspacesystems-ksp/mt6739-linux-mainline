from pathlib import Path
w=Path(r'D:\y5_linux_work')
s=(w/'y5_touch_id_probe.c').read_text()
s=s.replace('--probe-id','--probe-focal-id').replace('bw(4,0x5d<<1);bw(0x14,2);bw(0x6c,6);','bw(4,0x38<<1);bw(0x14,1);bw(0x6c,1);')
s=s.replace('bw(0,0x81);bw(0,0x40);bw(0x24,1);','bw(0,0xa3);bw(0x24,1);')
s=s.replace('count==6){for(unsigned i=0;i<6;i++)','count==1){for(unsigned i=0;i<1;i++)')
start=s.index('   /* Stock: power on')
end=s.index('   ret=read_product(data);',start)
s=s[:start]+'''   /* Stock focal_chip_detect: RST/INT low, enable2.8V,100ms;
    * focal_hardware_reset(2): HIGH10ms,LOW20ms,HIGH240ms.
    * Keep INT LOW as the stock detect path does before reading0xa3.
    * Delay240 is byte2 of the verified stock table at8c39ca8. */
   pin_out(6,0);mode6(0);pin_dir(6,1);pause_ms(100);
   pin_out(119,1);pause_ms(10);pin_out(119,0);pause_ms(20);
   pin_out(119,1);pause_ms(240);
'''+s[end:]
s=s.replace('TOUCH preflight','FOCAL preflight').replace('TOUCH data=','FOCAL raw_id_and_unused=')
(w/'y5_focal_id_probe.c').write_text(s,newline='\n')
script=(w/'vtouch_snapshot.sh').read_text().replace('timeout 3 /tmp/y5_vtouch_snapshot','timeout 3 /var/cache/mfw-ui/y5_focal_id_probe --probe-focal-id')
(w/'focal_id_probe.sh').write_text(script,newline='\n')
assert '0x5d<<1' not in s and 'count==6' not in s
print('Prepared exact stock FocalTech normal-mode ID probe')
