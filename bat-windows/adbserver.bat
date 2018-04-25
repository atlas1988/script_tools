rem Please connect device to PC!

adb wait-for-device
adb kill-server
:: delay 3s
@choice /t 3 /d y /n >nul  
adb start-server

::pause
