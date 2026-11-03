cd /home/y5/linux-7.3
git status --short 2>&1 | head -5
ls /home/y5 | head -20
echo ---NEWER
find . -newer Makefile -type f \( -name "*.c" -o -name "*.h" -o -name "*.S" -o -name "Kconfig*" \) -not -path "./.git/*" 2>/dev/null | grep -v "^./include/generated\|^./arch/arm64/include/generated" | head -30
