import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, pull
y5sh.run("pkill -9 gltest", timeout=10)
for rem, loc in (("/ro/vendor/lib64/egl/libEGL_mtk.so", "libEGL_mtk.so"),
                 ("/ro/vendor/lib64/egl/libGLESv2_mtk.so", "libGLESv2_mtk.so"),
                 ("/ro/vendor/lib64/libIMGegl.so", "libIMGegl.so")):
    n = pull.pull(rem, r"D:\y5_linux_work\android_libs" + "\\" + loc, port=2390)
    print(n, "B ->", loc)
