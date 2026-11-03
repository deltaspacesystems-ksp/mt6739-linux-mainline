@echo off
set JAVA_HOME=D:\y5_linux_work\jdk21\jdk-21.0.12.1+1
set PATH=%JAVA_HOME%\bin;%PATH%
cd /d D:\y5_linux_work
D:\y5_linux_work\ghidra\ghidra_12.1.4_PUBLIC\support\analyzeHeadless.bat D:\y5_linux_work\ghidra_k44 k44 -process stock_Image -noanalysis -scriptPath D:\y5_linux_work\ghidra_scripts -postScript Y5Decomp.java D:\y5_linux_work\kallsyms_44.txt %1 %2
echo KONIEC
