from pathlib import Path
w=Path(r'D:\y5_linux_work');s=(w/'inspect_stock_gpu_firmware.sh').read_text();a=s.index('  echo "GPU_FIRMWARE_FILE=');b=s.index('\n done',a)
s=s[:a]+'''  echo "GPU_FIRMWARE_FILE=$p"
  python3 /var/cache/mfw-ui/check_pvr_fw_header.py "$p"'''+s[b:];(w/'validate_stock_gpu_firmware.sh').write_text(s,newline='\n')
