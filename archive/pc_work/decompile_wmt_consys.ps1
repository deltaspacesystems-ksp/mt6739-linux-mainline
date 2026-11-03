$env:JAVA_HOME='D:\y5_linux_work\jdk21\jdk-21.0.12.1+1'
& 'D:\y5_linux_work\ghidra\ghidra_12.1.4_PUBLIC\support\analyzeHeadless.bat' 'D:\y5_linux_work\ghidra_wmt_proj' 'wmt_proj' '-process' 'wmt_drv.ko' '-readOnly' '-noanalysis' '-scriptPath' 'D:\y5_linux_work\ghidra_scripts' '-postScript' 'Y5DecompNamed.java' 'D:\y5_linux_work\out\wmt_consys.c' 'D:\y5_linux_work\out\wmt_consys_targets.txt' *> 'D:\y5_linux_work\out\ghidra_wmt_consys.log'
exit $LASTEXITCODE
