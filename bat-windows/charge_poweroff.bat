rem Please connect device to PC!

adb wait-for-device
adb reboot bootloader
:: delay 5s
@choice /t 5 /d y /n >nul  

:: poweroff charger  
fastboot oem enable-charger-screen

::poweroff do not charger  
::fastboot oem enable-charger-screen

rem 按power 按键重启!
:: 不要使用 fastboot reboot  否则会不生效
pause

adb wait-for-device
adb shell reboot -p
