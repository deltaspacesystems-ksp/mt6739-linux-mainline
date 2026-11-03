import sys, os, base64
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, y5put
# 1) dopilnuj, ze rootfs ma sshd + klucz (haslo root tymczasowe nie jest ustawiane; wchodzimy kluczem lub shellem RNDIS)
print(y5sh.run(r"""
chroot /mnt/root /bin/sh -c '
mkdir -p /root/.ssh /etc/ssh; chmod 700 /root/.ssh
ssh-keygen -A >/dev/null 2>&1
grep -q "^PermitRootLogin" /etc/ssh/sshd_config || echo "PermitRootLogin prohibit-password" >> /etc/ssh/sshd_config
echo "sshd: $(ls /etc/ssh/ssh_host_*key | wc -l) kluczy hosta"
'
""", timeout=60))
