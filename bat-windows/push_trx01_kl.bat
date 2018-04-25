::@echo off
rem Please connect device to PC!


adb wait-for-device
adb root
adb devices
adb remount
adb wait-for-device

adb shell ls -l system/usr/keylayout/
adb shell rm system/usr/keylayout/matrix_keypad.kl
adb shell rm system/usr/keylayout/gpio-keys.kl
adb shell rm system/usr/keylayout/qpnp_pon.kl
adb shell ls -l system/usr/keylayout/

adb push matrix_keypad.kl system/usr/keylayout/matrix_keypad.kl
adb push gpio-keys.kl system/usr/keylayout/gpio-keys.kl
adb push qpnp_pon.kl system/usr/keylayout/qpnp_pon.kl


adb shell sync
adb reboot
@echo 
:end
pause
