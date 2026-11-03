p = "/root/control_center.py"
s = open(p).read()
if "TORCH_LEDS" in s:
    raise SystemExit("already patched")

s = s.replace("\ndef sh(cmd):", '''
TORCH_LEDS = ("/sys/class/leds/torch-light0/brightness", "/sys/class/leds/torch-light1/brightness")


def torch_is_on():
    try:
        return int(open(TORCH_LEDS[0]).read().strip() or 0) > 0
    except Exception:
        return False


def torch_set(on):
    for f in TORCH_LEDS:
        try:
            open(f, "w").write("255" if on else "0")
        except Exception:
            pass


def sh(cmd):''', 1)

s = s.replace('''        if on_click:
            self.connect("clicked", lambda b: on_click())
''', '''        if on_click:
            self.connect("clicked", lambda b: on_click(self))
''', 1)

s = s.replace('actions.pack_start(RoundButton("torch"), False, False, 0)', '''self.torch_btn = RoundButton("torch", on_click=self.on_torch)
        if torch_is_on():
            self.torch_btn.get_style_context().add_class("on")
        actions.pack_start(self.torch_btn, False, False, 0)''', 1)

s = s.replace('''    def on_bt_toggle(self, active):
        pass
''', '''    def on_bt_toggle(self, active):
        pass

    def on_torch(self, btn):
        on = not torch_is_on()
        torch_set(on)
        ctx = btn.get_style_context()
        (ctx.add_class if on else ctx.remove_class)("on")
''', 1)

s = s.replace("button.cc-round:hover {", "button.cc-round.on, button.cc-round.on:hover { background-image: none; background-color: rgba(255,255,255,0.92); }\nbutton.cc-round:hover {", 1)
open(p, "w").write(s)
print("patched")
