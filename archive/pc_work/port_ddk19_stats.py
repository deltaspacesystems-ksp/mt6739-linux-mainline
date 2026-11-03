from pathlib import Path
root = Path('D:/y5_linux_work/rogue-ddk19-port')
p = root / 'services/server/common/process_stats.c'
s = p.read_text().replace('\tmtk_get_gpu_memory_usage_fp = MTKGetMemStat;\n', '').replace('\tmtk_dump_gpu_memory_usage_fp = MTKGetMemStatDump;\n', '')
p.write_text(s, newline='\n')
p = root / 'services/server/include/process_stats.h'
s = p.read_text().replace('extern unsigned int (*mtk_get_gpu_memory_usage_fp)(void);\n', '').replace('extern bool (*mtk_dump_gpu_memory_usage_fp)(void);\n', '')
p.write_text(s, newline='\n')
