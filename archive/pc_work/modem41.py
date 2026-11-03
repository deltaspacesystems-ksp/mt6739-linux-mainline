import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- cmdline'; cat /proc/cmdline | tr ' ' '\n' | grep -i -E 'androidboot|opt|rat|md'
echo '--- build.prop'; grep -h -i -E 'mtk_protocol|rat_config|md_sbp|ps1_rat|mtk_ril_mode|telephony|multisim|mtk_md|ro.mediatek.platform|ro.board|ro.hardware' /aroot/vendor/build.prop /aroot/system/build.prop /aroot/vendor/default.prop /aroot/default.prop 2>/dev/null | sort -u | head -n 40
echo '--- MD_SBP'; ls -l /aroot/vendor/nvdata/APCFG/APRDCL/ ; od -An -tx1 /aroot/vendor/nvdata/APCFG/APRDCL/MD_SBP | head -n 3
""", timeout=40))
