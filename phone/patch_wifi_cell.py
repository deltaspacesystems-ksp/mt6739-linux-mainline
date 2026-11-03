import shutil
p = "/root/icons.py"
shutil.copy(p, "/root/backup_20261023/icons.py.before_wifi_cell")
s = open(p).read()
if "class WifiIcon" in s:
    raise SystemExit("already patched")

# 1) fabryka: wifi -> nasza ikona
old = '    if kind in SVG_MAP:\n        return SvgIcon(SVG_MAP[kind], size=size, color=color)\n'
assert old in s
s = s.replace(old, '    if kind == "wifi":\n        return WifiIcon(size=size, color=color)\n' + old, 1)

# 2) kreski sygnalu: 4 zaokraglone, wymiary zmierzone ze wzoru iOS
start = s.index('    def _draw(self, widget, cr):\n        w = widget.get_allocated_width()\n        h = widget.get_allocated_height()\n        handle = _get_handle("cellular.svg")')
end = s.find("\n\nclass ", start)
end = len(s) if end == -1 else end
cell = '''    # Wymiary ze wzoru iOS (jednostki wzoru): kreska 11 szeroka, rozstaw 18, wysokosci 16/23/32/41, dol wyrownany.
    BAR_W, BAR_PITCH, BAR_H = 11.0, 18.0, (16.0, 23.0, 32.0, 41.0)
    BAR_RADIUS = 0.40            # promien rogow jako ulamek szerokosci kreski (0.5 = pelna pigulka)

    def _draw(self, widget, cr):
        w = widget.get_allocated_width()
        h = widget.get_allocated_height()
        tw, th = self.BAR_PITCH * 3 + self.BAR_W, self.BAR_H[-1]
        sc = min(w / tw, h * 0.62 / th)
        ox, oy = (w - tw * sc) / 2.0, (h + th * sc) / 2.0          # oy = linia dolna kresek
        r, g, b, a = self.color
        for i, bh in enumerate(self.BAR_H):
            x = ox + i * self.BAR_PITCH * sc
            bw = self.BAR_W * sc
            hh = bh * sc
            rad = bw * self.BAR_RADIUS                                       # lekko zaokraglone rogi (4 rogi)
            y0 = oy - hh
            cr.new_sub_path()
            cr.arc(x + bw - rad, y0 + rad, rad, -math.pi / 2, 0)
            cr.arc(x + bw - rad, oy - rad, rad, 0, math.pi / 2)
            cr.arc(x + rad, oy - rad, rad, math.pi / 2, math.pi)
            cr.arc(x + rad, y0 + rad, rad, math.pi, 3 * math.pi / 2)
            cr.close_path()
            cr.set_source_rgba(r, g, b, a if i < self.level else 0.28)
            cr.fill()
        return False
'''
s = s[:start] + cell + s[end:]

# 3) ikona Wi-Fi: wycinek + 2 luki, ze wzoru iOS (czubek na dole, rozwartosc +-38 st. od pionu)
s = s.rstrip("\n") + '''


class WifiIcon(Gtk.DrawingArea):
    """Wi-Fi jak w iOS: pelny wycinek przy czubku + dwa luki z zaokraglonymi koncami. Wymiary zmierzone ze wzoru."""
    W, H = 57.0, 43.0            # bbox w jednostkach wzoru; czubek (srodek lukow) = (28.5, 43)

    def __init__(self, size=26, color=(1, 1, 1, 0.95)):
        super().__init__()
        self.color = color
        self.set_size_request(size, size)
        self.set_halign(Gtk.Align.CENTER)
        self.set_valign(Gtk.Align.CENTER)
        self.set_hexpand(False)
        self.set_vexpand(False)
        self.connect("draw", self._draw)

    def _draw(self, widget, cr):
        w = widget.get_allocated_width()
        h = widget.get_allocated_height()
        sc = min(w / self.W, h * 0.62 / self.H)
        cx = w / 2.0
        cy = (h + self.H * sc) / 2.0                    # czubek na dolnej krawedzi bbox
        r, g, b, a = self.color
        cr.set_source_rgba(r, g, b, a)
        half = math.radians(38)
        up = -math.pi / 2
        cr.set_line_cap(1)                              # ROUND
        for rad, thick in ((37.5, 10.0), (23.0, 9.0)):
            cr.set_line_width(thick * sc)
            cr.new_sub_path()
            cr.arc(cx, cy, rad * sc, up - half, up + half)
            cr.stroke()
        # wycinek przy czubku (promien 13, polowa rozwarcia 45 st.) z lekkim zaokragleniem
        cr.set_line_join(1)                             # ROUND
        cr.set_line_width(3.0 * sc)
        a0, a1 = up - math.radians(45), up + math.radians(45)
        rr = 11.0 * sc
        cr.move_to(cx, cy)
        cr.arc(cx, cy, rr, a0, a1)
        cr.close_path()
        cr.fill_preserve()
        cr.stroke()
        return False
'''
open(p, "w").write(s)
print("patched")
