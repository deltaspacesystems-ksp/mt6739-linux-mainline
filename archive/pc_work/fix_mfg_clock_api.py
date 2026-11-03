from pathlib import Path
p=Path(r'D:\y5_linux_work\microfw-gpu\clk-mfw-mfg-probe.c');s=p.read_text().replace('static long mfw_mfg_round_rate(struct clk_hw *hw, unsigned long rate, unsigned long *parent_rate)\n{\n return rate == 26000000 ? 26000000 : -EINVAL;\n}','''static int mfw_mfg_determine_rate(struct clk_hw *hw, struct clk_rate_request *req)
{
 if (req->rate != 26000000 || req->min_rate > 26000000 || req->max_rate < 26000000)
  return -EINVAL;
 return 0;
}''').replace('.round_rate = mfw_mfg_round_rate','.determine_rate = mfw_mfg_determine_rate');p.write_text(s,newline='\n')
