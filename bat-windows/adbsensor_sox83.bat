rem Please connect device to PC!

adb wait-for-device
adb root
adb devices
adb remount

adb push Z:\Code\smatron_x83x84\msm8953_8940_r00012_O\LA.UM.6.6\LINUX\android\out\target\product\rimoY\vendor\lib\sensors.ssc.so /vendor/lib/
adb push Z:\Code\smatron_x83x84\msm8953_8940_r00012_O\LA.UM.6.6\LINUX\android\out\target\product\rimoY\vendor\lib64\sensors.ssc.so /vendor/lib64/
adb reboot
::pause
