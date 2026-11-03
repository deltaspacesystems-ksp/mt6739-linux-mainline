cd /mnt/d/y5_linux_work
python3 mkfont.py
sed -n 1,20p chk_cal2.sh >/dev/null
bash chk_cal2.sh | head -3
