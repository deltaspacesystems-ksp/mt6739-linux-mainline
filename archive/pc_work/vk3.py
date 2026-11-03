import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
ls -l /aroot/vendor/lib64/hw/vulkan.mt6739.so /aroot/vendor/lib64/libvulkan* /aroot/system/lib64/libvulkan.so 2>&1
strings -a /aroot/vendor/lib64/hw/vulkan.mt6739.so | grep -i -E 'vk|vulkan|\.so|not support|fail|error' | head -n 25
""", timeout=40))
