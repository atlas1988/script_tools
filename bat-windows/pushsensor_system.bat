::@echo off
rem Please connect device to PC!
::echo %0 echo %1 
if "%1%"=="" (
rem 输入对应的sensor_def_qcomdev.conf文件(format：UpdateSensor_conf.bat sensor_def_qcomdev.conf)
goto end
) else set SENSOR_CONFIG=%1%
::echo %SENSOR_CONFIG%

adb wait-for-device
adb root
adb devices
adb remount

:: 设置手机中sensor_def_qcomdev.conf 位置
::/system/etc/sensors/sensor_def_qcomdev.conf /system/vendor/etc/sensors/sensor_def_qcomdev.conf
set SENSOR_PATH_InPhone=/system/etc/sensors/sensor_def_qcomdev.conf
:: set SENSOR_PATH_InPhone=/vendor/etc/sensors/sensor_def_qcomdev.conf
::for /f "delims=" %%i in ( 'adb shell find /system -name sensor_def_qcomdev.conf ') do set SENSOR_PATH_InPhone=%%i 
::echo %SENSOR_PATH_InPhone%
echo %SENSOR_PATH_InPhone%

adb shell ls -l %SENSOR_PATH_InPhone%
adb shell rm /persist/sensors/sns.reg
adb shell rm %SENSOR_PATH_InPhone%
adb shell ls -l %SENSOR_PATH_InPhone%
adb push %SENSOR_CONFIG% %SENSOR_PATH_InPhone%
adb shell ls -l %SENSOR_PATH_InPhone%
::adb shell chmod 644 %SENSOR_PATH_InPhone%
adb shell sync
adb reboot
@echo 
:end
pause
