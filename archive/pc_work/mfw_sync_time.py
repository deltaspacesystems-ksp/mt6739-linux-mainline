"""Read/check phone UTC against the PC; --set explicitly sets volatile wall time.

Does not reboot, write RTC/partitions, or change monotonic time/charging timers.
"""
import argparse
import datetime as dt
import re
import time
from phone_shell import run

def epoch():
    out,rc,ok=run("printf 'MFW_EPOCH='; date -u +%s",15)
    m=re.search(r'^MFW_EPOCH=(\d+)\s*$',out,re.M)
    if not ok or str(rc)!='0' or not m:raise RuntimeError('Cannot read phone UTC')
    return int(m.group(1))

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--set',action='store_true',help='Set phone wall clock from current PC UTC')
    args=ap.parse_args()
    before=epoch();pc=int(time.time())
    print('Phone UTC:',dt.datetime.fromtimestamp(before,dt.timezone.utc).isoformat())
    print('PC UTC:',dt.datetime.fromtimestamp(pc,dt.timezone.utc).isoformat())
    if args.set:
        out,rc,ok=run(f'date -u -s @{pc}',15)
        if not ok or str(rc)!='0':raise RuntimeError('date -s did not succeed')
        after=epoch();offset=after-int(time.time())
        if abs(offset)>5:raise RuntimeError('Clock readback differs by more than 5 seconds')
        print('PASS: phone UTC set from PC; readback offset_s=',offset)
    else:print('Read-only check: offset_s=',before-pc)

if __name__=='__main__':main()
