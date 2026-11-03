from fontTools.ttLib import TTFont
from PIL import Image, ImageDraw, ImageFont
import sys

FONT_PATH = r"D:\y5_linux_work\fonts_extract\sfpro_final\Library\Fonts\SF-Pro-Text-Regular.otf"

f = TTFont(FONT_PATH)
cmap = f.getBestCmap()
pua = sorted([cp for cp in cmap if 0x100000 <= cp <= 0x10FFFD])
print("total PUA glyphs:", len(pua))

pil_font = ImageFont.truetype(FONT_PATH, 34)
label_font = ImageFont.load_default()

COLS = 25
ROWS = 40
PER_PAGE = COLS * ROWS
CELL_W, CELL_H = 50, 58

pages = (len(pua) + PER_PAGE - 1) // PER_PAGE
print("pages:", pages)

for page in range(pages):
    chunk = pua[page*PER_PAGE:(page+1)*PER_PAGE]
    img = Image.new("RGB", (COLS*CELL_W, ROWS*CELL_H), "white")
    d = ImageDraw.Draw(img)
    for i, cp in enumerate(chunk):
        col = i % COLS
        row = i // COLS
        x = col * CELL_W
        y = row * CELL_H
        d.rectangle([x, y, x+CELL_W-1, y+CELL_H-1], outline="#ddd")
        try:
            d.text((x+8, y+2), chr(cp), font=pil_font, fill="black")
        except Exception as e:
            pass
        d.text((x+2, y+CELL_H-12), "%X" % cp, font=label_font, fill="red")
    out = r"D:\y5_linux_work\sfsym_sheet_%02d.png" % page
    img.save(out)
    print("saved", out)
