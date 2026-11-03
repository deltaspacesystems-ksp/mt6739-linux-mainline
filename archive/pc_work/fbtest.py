import sys, socket, time, threading
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
W, H, STRIDE_PX = 720, 1440, 736          # stride 2944 B = 736 px * 4
bands = [(0xFF,0x00,0x00,0xFF), (0x00,0xFF,0x00,0xFF), (0x00,0x00,0xFF,0xFF), (0xFF,0xFF,0xFF,0xFF)]  # bajty 0..3 kolejnych pikseli
rows = bytearray()
for y in range(H):
    px = bands[min(3, y*4//H)]
    row = bytearray()
    for x in range(STRIDE_PX):
        if x >= W: row += bytes((0xFF,0x00,0xFF,0xFF))        # padding: magenta (gdy stride zly -> widac skos)
        elif x < 8: row += bytes((0,0,0,0xFF))                  # czarny pasek po lewej
        else: row += bytes(px)
    rows += row
open(r"D:\y5_linux_work\fb_pattern.raw","wb").write(rows)
print("pattern", len(rows), "B (", STRIDE_PX*4, "B/row )")
# wyslij na telefon: nc -l na 2324 -> /tmp/pat.raw
def listener(): y5sh.run("nc -l -p 2324 > /tmp/pat.raw; echo recv_done", timeout=60)
t = threading.Thread(target=listener); t.start(); time.sleep(2)
s = socket.create_connection(("172.16.42.1", 2324), timeout=5); s.sendall(bytes(rows)); s.close(); t.join(30)
print(y5sh.run("ls -l /tmp/pat.raw; cat /sys/class/graphics/fb0/virtual_size /sys/class/graphics/fb0/stride; dd if=/tmp/pat.raw of=/dev/fb0 bs=4096 2>&1 | tail -n 1", timeout=30))
