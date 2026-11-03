import sys
sys.path.insert(0, r"D:\y5_linux_work")
import pull
for n in ["wmt_chrdev_wifi.ko"]:
    print(pull.pull("/aroot/vendor/lib/modules/"+n, r"D:\y5_linux_work\tmpl.ko", port=2390))
