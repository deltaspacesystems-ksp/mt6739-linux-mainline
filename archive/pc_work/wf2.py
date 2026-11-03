import sys, datetime
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, y5put
now = datetime.datetime.now(datetime.timezone.utc).strftime("%Y-%m-%d %H:%M:%S")
y5put.put("/usr/local/bin/wcn-up", open(r"D:\y5_linux_work\guisrc\wcn-up.sh", encoding="utf-8").read(), 0o755)
print(y5sh.run(r"""
cat /proc/uptime; date -u -s "%s" >/dev/null
( /usr/local/bin/wcn-up >/tmp/wcn-up.log 2>&1 & )
echo started
""" % now, timeout=30))
