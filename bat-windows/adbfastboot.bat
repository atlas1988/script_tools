rem Please connect device to PC!

adb wait-for-device
adb reboot bootloader
:: delay 5s
@choice /t 5 /d y /n >nul  
fastboot devices

::pause
