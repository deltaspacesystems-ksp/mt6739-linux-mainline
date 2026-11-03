#!/usr/bin/env python3
"""Most Xvfb -> /dev/fb0 (ekran) oraz /dev/input/event3 (dotyk) -> XTest. Dziala na stockowym kernelu 4.4 bez VT/DRM."""
import mmap, os, struct, sys, time, zlib, fcntl, threading, select

FBIOGET_VSCREENINFO = 0x4600
FBIOPAN_DISPLAY = 0x4606
FB_ACTIVATE_NOW = 0
FB_ACTIVATE_FORCE = 0x80

def make_pan_kicker(fd):
    """Zwraca funkcje, ktora wymusza odswiezenie panelu (DISP/CMDQ nie podejmuje
    samych zmian w mmap bez jawnego FBIOPAN_DISPLAY na tym kernelu)."""
    var = bytearray(fcntl.ioctl(fd, FBIOGET_VSCREENINFO, bytes(160), True))
    struct.pack_into("<II", var, 16, 0, 0)          # xoffset=0, yoffset=0
    struct.pack_into("<I", var, 84, FB_ACTIVATE_NOW | FB_ACTIVATE_FORCE)  # activate
    def kick():
        try:
            fcntl.ioctl(fd, FBIOPAN_DISPLAY, var)
        except OSError:
            pass
    return kick

W, H, STRIDE = 736, 1440, 2944          # Xvfb: -screen 0 736x1440x24 => 2944 B/wiersz (zgodne z mtkfb)
VISIBLE_W = 720
XFB = "/tmp/xfb/Xvfb_screen0"
FB = "/dev/fb0"
TOUCH = os.environ.get("TOUCH_DEV", "/dev/input/event3")
FPS = float(os.environ.get("FPS", "24"))

def wait_for(path, secs=30):
    t = time.time() + secs
    while not os.path.exists(path):
        if time.time() > t: sys.exit("brak " + path)
        time.sleep(0.3)

def display_loop():
    wait_for(XFB)
    f = open(XFB, "rb")
    hdr = f.read(100)
    header_size = struct.unpack(">I", hdr[0:4])[0]
    ncolors = struct.unpack(">I", hdr[76:80])[0]
    off = header_size + ncolors * 12
    size = STRIDE * H
    xm = mmap.mmap(f.fileno(), off + size, access=mmap.ACCESS_READ)
    fd = os.open(FB, os.O_RDWR)
    fb = mmap.mmap(fd, size, mmap.MAP_SHARED, mmap.PROT_READ | mmap.PROT_WRITE)
    kick = make_pan_kicker(fd)
    out = bytearray(size)
    out[3::4] = b"\xff" * (size // 4)
    last = None; frames = 0; t0 = time.time()
    print("fbbridge: xwd off=%d ncolors=%d, ekran %dx%d" % (off, ncolors, W, H), flush=True)
    while True:
        src = xm[off:off + size]               # BGRX
        crc = zlib.crc32(src)
        if crc != last:
            last = crc
            out[0::4] = src[2::4]               # R
            out[1::4] = src[1::4]               # G
            out[2::4] = src[0::4]               # B
            fb[:] = out
            kick()                              # wymus odswiezenie panelu (DISP/CMDQ)
            frames += 1
            if frames % 100 == 0:
                print("fbbridge: %d klatek, %.1f fps" % (frames, frames / (time.time() - t0)), flush=True)
        time.sleep(1.0 / FPS)

EVIOCGABS = lambda a: 0x80184540 + a
def absinfo(fd, code):
    b = fcntl.ioctl(fd, EVIOCGABS(code), b"\0" * 24)
    v, mn, mx, fz, fl, rs = struct.unpack("6i", b)
    return mn, mx

def touch_loop():
    from Xlib import X, display
    from Xlib.ext import xtest
    d = None
    for _ in range(60):
        try:
            d = display.Display(":0"); break
        except Exception:
            time.sleep(0.5)
    if d is None: sys.exit("touch: brak X")
    fd = os.open(TOUCH, os.O_RDONLY)
    try: xmn, xmx = absinfo(fd, 0x35); ymn, ymx = absinfo(fd, 0x36)
    except OSError: xmn, xmx = absinfo(fd, 0); ymn, ymx = absinfo(fd, 1)
    print("touch: zakres X %d..%d Y %d..%d" % (xmn, xmx, ymn, ymx), flush=True)
    x = y = 0; down = False; pend = False
    ev = struct.Struct("llHHi")                 # aarch64: timeval(2*long) + type + code + value
    buf = b""
    while True:
        select.select([fd], [], [])
        buf += os.read(fd, 24 * 64)
        while len(buf) >= ev.size:
            sec, usec, t, c, v = ev.unpack(buf[:ev.size]); buf = buf[ev.size:]
            if t == 3 and c in (0x35, 0):   x = int((v - xmn) * VISIBLE_W / max(1, xmx - xmn)); pend = True
            elif t == 3 and c in (0x36, 1): y = int((v - ymn) * H / max(1, ymx - ymn)); pend = True
            elif t == 1 and c == 0x14a:
                if v and not down:
                    xtest.fake_input(d, X.MotionNotify, x=x, y=y); xtest.fake_input(d, X.ButtonPress, 1); down = True
                elif not v and down:
                    xtest.fake_input(d, X.ButtonRelease, 1); down = False
                d.sync()
            elif t == 0 and pend:
                xtest.fake_input(d, X.MotionNotify, x=x, y=y); d.sync(); pend = False

if __name__ == "__main__":
    if os.environ.get("GPUPRESENT") == "1":
        touch_loop()                      # wyswietlaniem zajmuje sie gpupresent (GPU), tu tylko dotyk
    else:
        th = threading.Thread(target=touch_loop, daemon=True); th.start()
        display_loop()
