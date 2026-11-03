"""Mark reviewed existing fallthroughs; do not change MMU/debug behavior."""
from pathlib import Path
root = Path('D:/y5_linux_work/rogue-ddk19-port')
targets = {
 'services/server/common/mmu_common.c': {1849:'case 2:',1856:'case 1:',1863:'case 0:',2169:'case MMU_LEVEL_2:',2181:'case MMU_LEVEL_1:',3902:'case MMU_LEVEL_2:',3975:'case MMU_LEVEL_1:',4039:'case MMU_LEVEL_2:',4050:'case MMU_LEVEL_1:'},
 'services/server/devices/rgx/rgxdebug.c': {985:'case 0x6:',990:'case 0x7:'},
}
for relative, positions in targets.items():
    p = root / relative
    lines = p.read_text().splitlines(keepends=True)
    for number, expected in sorted(positions.items(), reverse=True):
        actual = lines[number - 1]
        if actual.strip() != expected:
            raise RuntimeError(f'Review source changed: {relative}:{number}')
        indentation = actual[:len(actual) - len(actual.lstrip())]
        lines.insert(number - 1, indentation + '\tfallthrough;\n')
    p.write_text(''.join(lines), newline='\n')
