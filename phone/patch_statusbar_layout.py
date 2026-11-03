import shutil, math
p = "/root/status_bar.py"
shutil.copy(p, "/root/backup_20261023/status_bar.py.before_layout")
s = open(p).read()
if "ICON_H" in s:
    raise SystemExit("already patched")

# zegar: wysokosc cyfr ~ wysokosc ikon (iOS), polgrube
old_css = ".status-time { color: white; font-size: 17px; font-weight: 700; }"
assert old_css in s
s = s.replace(old_css, ".status-time { color: white; font-size: 21px; font-weight: 600; }", 1)

old = '''        right = Gtk.Box(spacing=7)
        right.set_halign(Gtk.Align.END)
        self.cell_icon = CellularIcon(level=4, size=26)
        self.wifi_icon = Icon("wifi", size=26)
        self.batt_icon = BatteryIcon(level=battery_level(), size=24)
'''
assert old in s
new = '''        # Wspolna skala ikon (jednostka wzoru iOS: wysokosc ikony = 43): kazda ikona ma szerokosc dokladnie swojego ksztaltu,
        # a odstep miedzy ikonami to ~0.55 wysokosci (wzor: 23 i 25 przy wysokosci 43) - jeden, taki sam miedzy wszystkimi.
        ICON_H = 24
        sc = ICON_H * 0.62 / 43.0
        gap = max(1, int(round(24 * sc)))
        right = Gtk.Box(spacing=gap)
        right.set_halign(Gtk.Align.END)
        self.cell_icon = CellularIcon(level=4, size=ICON_H)
        self.wifi_icon = Icon("wifi", size=ICON_H)
        self.batt_icon = BatteryIcon(level=battery_level(), size=ICON_H)
        self.cell_icon.set_size_request(int(math.ceil(65 * sc)), ICON_H)
        self.wifi_icon.set_size_request(int(math.ceil(57 * sc)), ICON_H)
        self.batt_icon.set_size_request(int(math.ceil(2.166 * ICON_H * 0.62)), ICON_H)
'''
s = s.replace(old, new, 1)
if "\nimport math" not in s:
    s = s.replace("\nimport subprocess", "\nimport math\nimport subprocess", 1)
open(p, "w").write(s)
print("patched")
