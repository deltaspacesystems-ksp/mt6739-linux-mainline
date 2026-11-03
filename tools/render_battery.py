"""Generuje ikony ladowania/baterii w stylu microfw (czern, biel, ASCII art) jako pliki .raw (BGRA) o DOKLADNIE tych wymiarach,
ktore ma oryginalna partycja logo (LK wstawia je w stale miejsca). Uzycie: python render_battery.py KATALOG_WYJSCIA [ASCII_LOGO.txt]
Wynik: KATALOG/NN.raw dla indeksow: 1,2,3,4-14 (cyfry 45x64, % 57x64),15-24 (fale 163x29 -> czarne),25-35,36,37,38,39,40 oraz podglad.png.
Skladanie: tools/make_logo.py ORYGINAL.img WYNIK.img --raw 1=KATALOG/01.raw --raw 2=... (patrz tools/build_logo_all.sh)"""
import sys, os, math
from PIL import Image, ImageDraw, ImageFont
W, H, S = 720, 1440, 3
out = sys.argv[1]; os.makedirs(out, exist_ok=True)
FB = "/usr/share/fonts/truetype/jetbrains-mono/JetBrainsMono-Bold.ttf"
FM = "/usr/share/fonts/truetype/jetbrains-mono/JetBrainsMono-Medium.ttf"
def canvas(w=W, h=H): return Image.new("L", (w * S, h * S), 0)
def finish(img, w=W, h=H):
    return img.resize((w, h), Image.LANCZOS)
def save_raw(idx, img):
    g = img.tobytes(); raw = bytearray()
    for v in g: raw += bytes((v, v, v, 255))
    open(os.path.join(out, "%02d.raw" % idx), "wb").write(bytes(raw)); return img
def text_center(d, s, font_path, size, cx, cy, w=W):
    f = ImageFont.truetype(font_path, round(size * S))
    L = f.getlength(s); bb = f.getbbox(s)
    d.text((cx * S - L / 2, cy * S - (bb[1] + bb[3]) / 2), s, font=f, fill=255)
def grid(d, rows, size, lh, x_center, top, font_path=FM):
    f = ImageFont.truetype(font_path, round(size * S)); cw = f.getlength("M")
    wch = max(len(r) for r in rows); x0 = x_center * S - cw * wch / 2
    for r, line in enumerate(rows):
        for c, ch in enumerate(line):
            if ch != " ": d.text((x0 + c * cw, (top + r * lh) * S), ch, font=f, fill=255)
BW, R = 12, 9                                         # wnetrze baterii: 12 znakow x 9 wierszy
def battery_rows(frac, label_rows=None):
    full = int(frac * R); rem = frac * R - full
    dens = "#" if rem > 0.75 else "=" if rem > 0.5 else ":" if rem > 0.25 else "." if rem > 0.05 else " "
    inner = []
    for r in range(R):                                 # r=0 gora
        k = R - 1 - r                                  # numer od dolu
        if k < full: inner.append("#" * BW)
        elif k == full and dens != " ": inner.append(dens * BW)
        else: inner.append(" " * BW)
    if label_rows:
        for r, s in label_rows.items(): inner[r] = s.center(BW)
    rows = ["    .----.    ", "." + "-" * BW + "."] + ["|" + s + "|" for s in inner] + ["'" + "-" * BW + "'"]
    return rows
BAT_TOP, BAT_LH, BAT_CX = 606, 28, 360                # miejsce oryginalnej baterii: x 270-450, y 606-946
def battery_frame(frac, label=None):
    img = canvas(); d = ImageDraw.Draw(img)
    grid(d, battery_rows(frac), 26, BAT_LH, BAT_CX, BAT_TOP)
    if label: text_center(d, label, FB, 84, 360, 497)
    return finish(img)
SHADE = [(0.85, "#"), (0.62, "="), (0.42, "+"), (0.24, ":"), (0.08, ".")]
def shade_char(c):
    for t, ch in SHADE:
        if c >= t: return ch
    return " "
def ascii_shape(mask, cx, cy, cw=7.2, lh=12.0, size=12.5):
    """mask: obraz L (W x H), 255 = ksztalt. Rysuje go znakami ASCII o gestosci zaleznej od pokrycia komorki (cw x lh px)."""
    box = mask.getbbox(); x0, y0, x1, y1 = box
    cols = int((x1 - x0) / cw) + 2; rows = int((y1 - y0) / lh) + 2
    ox = cx - cols * cw / 2; oy = cy - rows * lh / 2
    img = canvas(); d = ImageDraw.Draw(img); f = ImageFont.truetype(FB, round(size * S))
    cell = Image.new("L", (1, 1))
    for r in range(rows):
        for c in range(cols):
            bx0, by0 = ox + c * cw, oy + r * lh
            reg = mask.crop((round(bx0), round(by0), round(bx0 + cw), round(by0 + lh)))
            cov = sum(reg.getdata()) / (255.0 * max(1, reg.width * reg.height))
            ch = shade_char(cov)
            if ch != " ":
                L = f.getlength(ch); d.text(((bx0 + cw / 2) * S - L / 2, by0 * S), ch, font=f, fill=255)
    return finish(img)
def bolt_circle(dashed):
    m = Image.new("L", (W * 2, H * 2), 0); d = ImageDraw.Draw(m); k = 2          # maska 2x, potem skala do 1x
    cx, cy, r, t = 359 * k, 389 * k, 138 * k, 14 * k
    if dashed:
        n = 22
        for i in range(n):
            a0 = 360 / n * i; d.arc((cx - r, cy - r, cx + r, cy + r), a0, a0 + 360 / n * 0.6, fill=255, width=t)
    else:
        d.ellipse((cx - r, cy - r, cx + r, cy + r), outline=255, width=t)
    pts = [(14, -100), (-52, 14), (-6, 14), (-24, 100), (56, -24), (8, -24), (36, -100)]
    d.polygon([(cx + x * k * 1.06, cy + y * k * 1.06) for x, y in pts], fill=255)
    return ascii_shape(m.resize((W, H), Image.LANCZOS), 359, 389)
logo_raw = sys.argv[2] if len(sys.argv) > 2 else None
previews = {}
# 1: niski poziom (pusta bateria + podpis)
img = canvas(); d = ImageDraw.Draw(img); grid(d, battery_rows(0.0), 26, BAT_LH, BAT_CX, BAT_TOP)
text_center(d, "LOW BATTERY", FB, 40, 360, 1080); previews[1] = save_raw(1, finish(img))
previews[2] = save_raw(2, bolt_circle(True))            # 2: czerwony piorun -> przerywany pierscien
previews[39] = save_raw(39, bolt_circle(False))         # 39: zielony piorun -> pelny pierscien
# 3: blad "odlacz ladowarke": bateria z wykrzyknikiem + wtyczka + tekst
img = canvas(); d = ImageDraw.Draw(img)
bang = {2: "||", 3: "||", 4: "||", 5: "||", 7: "||"}
grid(d, battery_rows(0.0, bang), 26, BAT_LH, BAT_CX, 406)
grid(d, ["  _||__||_  ", "  \\______/  ", "     ||     ", "     ||     "], 26, BAT_LH, BAT_CX, 406 + 12 * BAT_LH + 4)
text_center(d, "Error! Please plug out charger!", FM, 26, 360, 1014); previews[3] = save_raw(3, finish(img))
# 25..35: pusta bateria z mala "fala" na dnie (wysokosc fali z pomiaru oryginalu, px z 290)
wave_px = {25: 25, 26: 25, 27: 22, 28: 15, 29: 10, 30: 11, 31: 17, 32: 23, 33: 24, 34: 25, 35: 29}
for i, px in wave_px.items(): previews[i] = save_raw(i, battery_frame(px / 290.0))
previews[37] = save_raw(37, battery_frame(1.0, "100%"))
# 38: drugie logo startowe = logo microfw (jak obraz 0); 40: META
if logo_raw: open(os.path.join(out, "38.raw"), "wb").write(open(logo_raw, "rb").read())
img = canvas(); d = ImageDraw.Draw(img)
here = os.path.dirname(os.path.abspath(__file__)); ld = os.path.join(here, "..", "phone", "logo")
def art(path):
    L = [l.rstrip("\n") for l in open(path, encoding="utf-8")]; w = max(len(l) for l in L); return [l.ljust(w) for l in L]
grid(d, art(os.path.join(ld, "meta_ascii.txt")), 36, 44, 360, 480, FB)
grid(d, art(os.path.join(ld, "mode_ascii.txt")), 30, 37, 360, 740, FB)
grid(d, art(os.path.join(ld, "microfw_ascii.txt")), 9.2, 11, 360, 1338 - 30)
previews[40] = save_raw(40, finish(img))
# cyfry 4..13 (0-9) 45x64 i % (14) 57x64: biale na czarnym
for i in range(4, 15):
    w = 57 if i == 14 else 45; ch = "%" if i == 14 else str(i - 4)
    img = Image.new("L", (w * S, 64 * S), 0); d = ImageDraw.Draw(img)
    text_center(d, ch, FB, 72, w / 2, 32.5, w=w); save_raw(i, finish(img, w, 64))
for i in range(15, 25): save_raw(i, Image.new("L", (163, 29), 0))   # nakladki fali: czarne
save_raw(36, Image.new("L", (163, 1), 0))
# podglad zbiorczy
tw, th = 120, 240
ids = sorted(previews)
sheet = Image.new("L", (tw * 8, th * ((len(ids) + 7) // 8)), 25)
for k, i in enumerate(ids): sheet.paste(previews[i].resize((tw, th), Image.LANCZOS), ((k % 8) * tw, (k // 8) * th))
sheet.save(os.path.join(out, "podglad.png")); print("ok:", sorted(f for f in os.listdir(out) if f.endswith(".raw"))[:3], "...", len(ids), "pelnoekranowych")
