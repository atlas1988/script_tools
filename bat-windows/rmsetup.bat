rem Please connect device to PC!

adb wait-for-device
adb root
adb devices
adb remount
adb wait-for-device
adb shell rm -rf /system/priv-app/SetupWizard
adb reboot
::pause
