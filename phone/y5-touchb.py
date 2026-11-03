#!/usr/bin/env python3
"""Tlumacz dotyku: /dev/input/event3 (MT protokol A, bez slotow) -> wirtualne urzadzenie uinput
z protokolem B (ABS_MT_SLOT + TRACKING_ID), zeby libinput/wlroots/labwc dostaly prawdziwy multitouch.
Nie robi EVIOCGRAB - produkcyjny fbbridge.py dalej czyta oryginal."""
import os, fcntl, struct, select, sys, time

SRC = os.environ.get("TOUCH_DEV", "/dev/input/event3")
NAME = os.environ.get("TOUCHB_NAME", "y5-touch-b").encode()
# Zadeklarowany zakres = rozmiar wyjscia kompozytora (Xvfb 736x1440), wartosci surowe 0..720 zostaja bez skalowania,
# wiec libinput mapuje 1:1 na piksele (tak jak stary fbbridge: VISIBLE_W=720 px od lewej).
W, H, MAXSLOT = int(os.environ.get("OUT_W", "736")), int(os.environ.get("OUT_H", "1440")), 10
EV_SYN, EV_KEY, EV_ABS = 0, 1, 3
ABS_X, ABS_Y, SLOT, MTX, MTY, TRK, PRS = 0x00, 0x01, 0x2f, 0x35, 0x36, 0x39, 0x3a
BTN_TOUCH = 0x14a
UI_SET_EVBIT, UI_SET_KEYBIT, UI_SET_ABSBIT, UI_SET_PROPBIT = 0x40045564, 0x40045565, 0x40045567, 0x4004556e
UI_DEV_CREATE, UI_DEV_DESTROY = 0x5501, 0x5502

def ev(t, c, v): return struct.pack("qqHHi", 0, 0, t, c, v)

def make_uinput():
    fd = os.open("/dev/uinput", os.O_WRONLY | os.O_NONBLOCK)
    for e in (EV_SYN, EV_KEY, EV_ABS): fcntl.ioctl(fd, UI_SET_EVBIT, e)
    fcntl.ioctl(fd, UI_SET_KEYBIT, BTN_TOUCH)
    for a in (ABS_X, ABS_Y, SLOT, MTX, MTY, TRK, PRS): fcntl.ioctl(fd, UI_SET_ABSBIT, a)
    fcntl.ioctl(fd, UI_SET_PROPBIT, 1)            # INPUT_PROP_DIRECT = ekran dotykowy
    absmax, absmin = [0] * 64, [0] * 64
    for a, mx in ((ABS_X, W), (ABS_Y, H), (MTX, W), (MTY, H), (SLOT, MAXSLOT - 1), (TRK, 65535), (PRS, 255)):
        absmax[a] = mx
    dev = NAME.ljust(80, b"\0") + struct.pack("HHHH", 3, 0x1234, 0x5678, 1) + struct.pack("I", 0)
    dev += struct.pack("64i", *absmax) + struct.pack("64i", *absmin) + struct.pack("64i", *([0] * 64)) * 2
    os.write(fd, dev)
    fcntl.ioctl(fd, UI_DEV_CREATE)
    return fd

def main():
    src = os.open(SRC, os.O_RDONLY)
    ui = make_uinput()
    time.sleep(0.5)
    slots = {}          # track id -> slot
    next_tid = 1
    cur, contacts, touching = {}, [], 0
    print("y5-touchb: %s -> uinput '%s'" % (SRC, NAME.decode()), flush=True)
    try:
        while True:
            select.select([src], [], [])
            data = os.read(src, 24 * 64)
            out = b""
            for i in range(0, len(data) - 23, 24):
                _, _, t, c, v = struct.unpack("qqHHi", data[i:i + 24])
                if t == EV_ABS:
                    cur[c] = v
                elif t == EV_SYN and c == 2:                      # SYN_MT_REPORT: koniec jednego punktu
                    if MTX in cur and MTY in cur:
                        contacts.append((cur.get(TRK, 0), cur[MTX], cur[MTY], cur.get(PRS, 100)))
                    cur = {}
                elif t == EV_SYN and c == 0:                      # SYN_REPORT: koniec ramki
                    seen = set()
                    for tid, x, y, p in contacts[:MAXSLOT]:
                        seen.add(tid)
                        if tid not in slots:
                            free = [s for s in range(MAXSLOT) if s not in slots.values()]
                            if not free: continue
                            slots[tid] = free[0]
                            out += ev(EV_ABS, SLOT, slots[tid]) + ev(EV_ABS, TRK, next_tid); next_tid = next_tid % 60000 + 1
                        out += ev(EV_ABS, SLOT, slots[tid]) + ev(EV_ABS, MTX, x) + ev(EV_ABS, MTY, y) + ev(EV_ABS, PRS, p)
                    for tid in [k for k in slots if k not in seen]:
                        out += ev(EV_ABS, SLOT, slots.pop(tid)) + ev(EV_ABS, TRK, -1)
                    n = len(slots)
                    if contacts:
                        out += ev(EV_ABS, ABS_X, contacts[0][1]) + ev(EV_ABS, ABS_Y, contacts[0][2])
                    if (n > 0) != (touching > 0):
                        out += ev(EV_KEY, BTN_TOUCH, 1 if n else 0)
                    touching = n
                    out += ev(EV_SYN, 0, 0)
                    contacts = []
            if out: os.write(ui, out)
    finally:
        fcntl.ioctl(ui, UI_DEV_DESTROY)

if __name__ == "__main__":
    main()
