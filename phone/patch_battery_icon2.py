import shutil
p = "/root/icons.py"
shutil.copy(p, "/root/backup_20261023/icons.py.before_battery2")
s = open(p).read()
start = s.index("    def _draw(self, widget, cr):\n        # iOS 26/27: pelna")
end = s.index("    def _rr(self, cr, x, y, w, h, r):", start)
new = '''    def _draw(self, widget, cr):
        # Proporcje zmierzone ze wzoru iOS (jednostka u = wysokosc korpusu):
        # korpus 1.98u x 1u, promien rogow 0.27u, szczelina 0.07u, wypustka 0.116u x 0.33u (zaokraglona).
        w = widget.get_allocated_width()
        h = widget.get_allocated_height()
        u = min(h * 0.62, w / 2.166)
        bw, bh = 1.98 * u, u
        gap, nw, nh = 0.07 * u, 0.116 * u, 0.33 * u
        x = (w - (bw + gap + nw)) / 2.0
        y = (h - bh) / 2.0
        r = 0.27 * u
        cr_, cg_, cb_, ca_ = self.color
        self._rr(cr, x, y, bw, bh, r)
        cr.set_source_rgba(cr_, cg_, cb_, 0.50)
        cr.fill()
        self._rr(cr, x + bw + gap, y + (bh - nh) / 2.0, nw, nh, nw * 0.9)
        cr.set_source_rgba(cr_, cg_, cb_, 0.46)
        cr.fill()
        fw = bw * self.level / 100.0
        if fw > 0:
            cr.save()
            self._rr(cr, x, y, bw, bh, r)
            cr.clip()
            cr.rectangle(x, y, fw, bh)
            cr.set_source_rgba(cr_, cg_, cb_, 1.0)
            cr.fill()
            cr.restore()
        return False

'''
open(p, "w").write(s[:start] + new + s[end:])
print("patched")
