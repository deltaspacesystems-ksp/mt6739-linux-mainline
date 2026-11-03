@echo off
set JAVA_HOME=D:\y5_linux_work\jdk21\jdk-21.0.12.1+1
set PATH=%JAVA_HOME%\bin;%PATH%
cd /d D:\y5_linux_work
rmdir /s /q D:\y5_linux_work\ghidra_k44 2>nul
mkdir D:\y5_linux_work\ghidra_k44
D:\y5_linux_work\ghidra\ghidra_12.1.4_PUBLIC\support\analyzeHeadless.bat D:\y5_linux_work\ghidra_k44 k44 -import D:\y5_linux_work\out\stock_Image -loader BinaryLoader -loader-baseAddr ffffff8008080000 -processor AARCH64:LE:64:v8A -noanalysis -scriptPath D:\y5_linux_work\ghidra_scripts -postScript Y5Decomp.java D:\y5_linux_work\kallsyms_44.txt D:\y5_linux_work\k44_dec1.c usb_phy_poweron,usb_phy_recover,usb_enable_clock,usb_prepare_enable_clock,mt_usb_enable_clk,mt_usb_prepare_clk,mt_usb_probe,musb_platform_reset,usb_phy_savecurrent_internal,ddp_dsi_trigger,DSI_Start,ddp_dsi_start,_trigger_display_interface,primary_display_trigger_nolock.isra.46,msdc_ungate_clock,msdc_init_hw
echo KONIEC
