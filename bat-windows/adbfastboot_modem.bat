rem Please connect device to PC!

adb wait-for-device
adb reboot bootloader
:: delay 5s
@choice /t 5 /d y /n >nul  
fastboot devices
fastboot flash modem MSM8940.LA.3.0.1\common\build\bin\asic\NON-HLOS.bin
fastboot reboot 
::pause
