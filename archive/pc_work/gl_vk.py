import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
ls /aroot/vendor/lib64/hw /aroot/vendor/lib64 /aroot/system/lib64 2>/dev/null | grep -i -E 'vulkan|vk|opencl|OpenCL|gles|libEGL'
grep -i vulkan /aroot/vendor/build.prop /aroot/system/build.prop 2>/dev/null
""", timeout=30))
