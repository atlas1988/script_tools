rem Please connect device to PC!

adb wait-for-device
adb root
adb disable-verity
adb wait-for-device
adb reboot
pause
