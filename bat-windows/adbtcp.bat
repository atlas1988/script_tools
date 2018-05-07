rem Please connect device to PC!

adb wait-for-device
adb root
adb wait-for-device
adb remount
adb devices
adb shell "setprop service.adb.tcp.port 5555"
adb shell "getprop service.adb.tcp.port"
rem Please set build.prop: adb shell "echo "service.adb.tcp.port=5555" >> /vendor/build.prop"
::adb shell "echo "service.adb.tcp.port=5555" >> /vendor/build.prop"
adb shell "cat /vendor/build.prop"
::adb reboot 
::pause
