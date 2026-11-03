from pathlib import Path
w=Path(r'D:\y5_linux_work');p=w/'microfw-gpu/clk-mfw-mfg-probe.c';s=p.read_text().replace('#define to_mfw_clock(h)','#ifdef MODULE\n#error "MFG probe clock must remain built-in"\n#endif\n#define to_mfw_clock(h)').replace('.of_match_table = mfw_mfg_clock_match },','.of_match_table = mfw_mfg_clock_match, .suppress_bind_attrs = true },');p.write_text(s,newline='\n')
p=w/'microfw-gpu/test_mfg_power.c';s=p.read_text().replace('puts("PASS:', '''for(unsigned fail=1;fail<=7;fail++){init(&m,&io);assert(!mfw_mfg_power_up(&io));m.fail_poll=m.polls+fail;assert(mfw_mfg_power_down(&io)==-ETIMEDOUT);assert(io.stage>=5&&io.stage<8);}
 puts("PASS:''');p.write_text(s,newline='\n')
