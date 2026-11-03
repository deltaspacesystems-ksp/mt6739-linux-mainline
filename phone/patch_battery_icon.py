import shutil
p = "/root/icons.py"
shutil.copy(p, "/root/backup_20261023/icons.py.before_battery")
s = open(p).read()
start_marker = '    def _draw(self, widget, cr):\n        w = widget.get_allocated_width()\n        h = widget.get_allocated_height()\n        handle = _get_handle("battery0.svg")'
start = s.index(start_marker)
end = s.index("    def _rr(self, cr, x, y, w, h, r):", start)
new = '''    def _draw(self, widget, cr):
        # iOS 26/27: pelna "pigulka" bez konturu i wypustki; jasna czesc z lewej = poziom, reszta szara/polprzezroczysta.
        w = widget.get_allocated_width()
        h = widget.get_allocated_height()
        bh = h * 0.60
        bw = min(w - 2.0, bh * 2.4)
        x, y = (w - bw) / 2.0, (h - bh) / 2.0
        r = bh * 0.34
        cr_, cg_, cb_, ca_ = self.color
        self._rr(cr, x, y, bw, bh, r)
        cr.set_source_rgba(cr_, cg_, cb_, 0.38)
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
