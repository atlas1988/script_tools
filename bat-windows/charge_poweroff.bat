rem Please connect device to PC!

adb wait-for-device
adb reboot bootloader
:: delay 5s
@choice /t 5 /d y /n >nul  

:: poweroff charger  
fastboot oem enable-charger-screen

::poweroff do not charger  
::fastboot oem enable-charger-screen

rem ��power ��������!
:: ��Ҫʹ�� fastboot reboot  ����᲻��Ч
pause

adb wait-for-device
adb shell reboot -p
