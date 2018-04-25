rem Please connect device to PC!
rem 获取sensor hub 相关信息
::echo %SENSOR_CONFIG%
adb wait-for-device
adb root
adb devices
adb remount
adb shell find /system -name sensor_def_qcomdev.conf
adb shell find /vendor -name sensor_def_qcomdev.conf
adb shell getprop ro.board.platform
adb shell cat /sys/devices/soc0/hw_platform
adb shell cat /sys/devices/soc0/soc_id
adb shell cat /sys/devices/soc0/platform_subtype
adb shell cat /sys/devices/soc0/platform_version
@echo end
:end
pause
