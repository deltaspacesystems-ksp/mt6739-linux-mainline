"""Renderuje logo startowe microfw: 720x1440, czarne tlo, biale napisy, supersampling 4x (wygladzone krawedzie).
Uklad: duzy ASCII art na srodku-gorze, na samym dole "POWERED BY" + maly ASCII art.
python render_logo.py ASCII.txt WYNIK.raw [PODGLAD.png] [--font PLIK_TTF]   (raw = BGRA 4147200 B, pod make_logo.py --raw)"""
import sys
from PIL import Image, ImageDraw, ImageFont
W, H, S = 720, 1440, 4
args = list(sys.argv[1:])
font_bold = "/usr/share/fonts/truetype/jetbrains-mono/JetBrainsMono-Bold.ttf"
font_reg = "/usr/share/fonts/truetype/jetbrains-mono/JetBrainsMono-Medium.ttf"
if "--font" in args:
    i = args.index("--font"); font_bold = font_reg = args[i + 1]; del args[i:i + 2]
art = [l.rstrip("\n") for l in open(args[0], encoding="utf-8")]
while art and not art[-1].strip(): art.pop()
w = max(len(l) for l in art); art = [l.ljust(w) for l in art]
img = Image.new("L", (W * S, H * S), 0)
d = ImageDraw.Draw(img)
def block(lines, size_px, lh_px, cy):
    """rysuje blok ASCII: znak po znaku na rownej siatce, wysrodkowany w poziomie, srodek pionowy w cy (piksele docelowe)"""
    fnt = ImageFont.truetype(font_reg, round(size_px * S))
    cw = fnt.getlength("M")
    x0 = (W * S - cw * len(lines[0])) / 2
    top = cy * S - lh_px * S * len(lines) / 2
    for r, line in enumerate(lines):
        for c, ch in enumerate(line):
            if ch != " ": d.text((x0 + c * cw, top + r * lh_px * S), ch, font=fnt, fill=255)
    return cw / S * len(lines[0])
bw = block(art, 21.5, 26, 610)                    # duzy logotyp: ok. 80% szerokosci, srodek pionowy ok. 42%
cap = ImageFont.truetype(font_bold, round(19 * S))
t = "POWERED BY"
d.text(((W * S - cap.getlength(t)) / 2, 1268 * S), t, font=cap, fill=255)
sw = block(art, 9.2, 11, 1338)                    # maly logotyp tuz nad dolna krawedzia
img = img.resize((W, H), Image.LANCZOS)
raw = bytearray()
g = img.tobytes()
for v in g: raw += bytes((v, v, v, 255))
open(args[1], "wb").write(bytes(raw))
if len(args) > 2: img.convert("RGB").save(args[2])
print("ok %d B; duzy logo %.0f px, maly %.0f px szerokosci" % (len(raw), bw, sw))
