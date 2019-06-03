rem Please connect device to PC!

adb wait-for-device
adb root
adb devices
adb wait-for-device
adb shell "cat /proc/sys/kernel/printk"
adb shell "echo 8 4 1 7 > /proc/sys/kernel/printk"
adb shell "cat /proc/sys/kernel/printk"
::pause
