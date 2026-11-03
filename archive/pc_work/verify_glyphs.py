from PIL import Image, ImageDraw, ImageFont

FONT_PATH = r"D:\y5_linux_work\fonts_extract\sfpro_final\Library\Fonts\SF-Pro-Text-Regular.otf"
pil_font = ImageFont.truetype(FONT_PATH, 60)
label_font = ImageFont.load_default()

candidates = {
    "wifi_outline": 0x100C9, "wifi_filled": 0x100CA, "wifi_slash": 0x100CB,
    "airplane_1": 0x100DEF, "airplane_2": 0x100DF0, "airplane_3": 0x100DF1,
    "camera_1": 0x100320, "camera_2": 0x100321,
    "gear_1": 0x1008CA, "gear_2": 0x1008CB, "gear_3": 0x1008CC,
    "speaker_1": 0x100955, "speaker_2": 0x100956,
    "clock_1": 0x100422, "clock_2": 0x100423,
    "cellular": 0x10218C,
    "lock_1": 0x1008AC, "lock_2": 0x1008AD, "lock_3": 0x1008AE,
    "lock_4": 0x1008AF, "lock_5": 0x1008B0,
}

COLS = 6
CELL = 110
rows = (len(candidates) + COLS - 1) // COLS
img = Image.new("RGB", (COLS*CELL, rows*CELL), "white")
d = ImageDraw.Draw(img)
for i, (name, cp) in enumerate(candidates.items()):
    col = i % COLS
    row = i // COLS
    x, y = col*CELL, row*CELL
    d.rectangle([x, y, x+CELL-1, y+CELL-1], outline="#ccc")
    try:
        d.text((x+15, y+5), chr(cp), font=pil_font, fill="black")
    except Exception:
        pass
    d.text((x+2, y+CELL-14), name, font=label_font, fill="red")

img.save(r"D:\y5_linux_work\glyph_verify.png")
print("saved")
