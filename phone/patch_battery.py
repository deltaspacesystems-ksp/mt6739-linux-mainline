p = "/root/status_bar.py"
s = open(p).read()
old = '''def battery_level():
    v = sh("cat /sys/class/power_supply/battery/capacity 2>/dev/null")
    try:
        return int(v)
    except Exception:
        return 100
'''
new = '''# Miernik poziomu (fuel gauge) w tym kernelu zwraca capacity=-1, wiec poziom szacujemy z napiecia ogniwa (Li-ion 4.4 V).
_VCURVE = [(3.40, 0), (3.60, 5), (3.70, 15), (3.75, 25), (3.80, 35), (3.85, 45),
           (3.90, 55), (3.98, 65), (4.05, 75), (4.15, 85), (4.25, 93), (4.35, 100)]


def _estimate_from_voltage(volts, charging):
    v = volts - (0.08 if charging else 0.0)       # pod ladowaniem napiecie jest zawyzone
    if v <= _VCURVE[0][0]:
        return 0
    for (v0, p0), (v1, p1) in zip(_VCURVE, _VCURVE[1:]):
        if v <= v1:
            return int(round(p0 + (p1 - p0) * (v - v0) / (v1 - v0)))
    return 100


def battery_level():
    try:
        cap = int(sh("cat /sys/class/power_supply/battery/capacity 2>/dev/null"))
        if 0 <= cap <= 100:
            return cap
    except Exception:
        pass
    try:
        uv = int(sh("cat /sys/class/power_supply/battery/batt_vol 2>/dev/null"))
        charging = sh("cat /sys/class/power_supply/battery/status 2>/dev/null") == "Charging"
        return _estimate_from_voltage(uv / 1e6, charging)
    except Exception:
        return 100
'''
assert old in s
open(p, "w").write(s.replace(old, new, 1))
print("patched")
