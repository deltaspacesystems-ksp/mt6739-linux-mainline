$env:JAVA_HOME='D:\y5_linux_work\jdk21\jdk-21.0.12.1+1'
& 'D:\y5_linux_work\ghidra\ghidra_12.1.4_PUBLIC\support\analyzeHeadless.bat' 'D:\y5_linux_work\ghidra_k44' 'k44' '-process' 'stock_Image' '-readOnly' '-noanalysis' '-scriptPath' 'D:\y5_linux_work\ghidra_scripts' '-postScript' 'Y5Decomp.java' 'X:\y5linux\phone\kallsyms_44.txt' 'D:\y5_linux_work\out\k44_touch_reset.c' 'D:\y5_linux_work\out\touch_reset_targets.txt' *> 'D:\y5_linux_work\out\ghidra_touch_reset.log'
exit $LASTEXITCODE
