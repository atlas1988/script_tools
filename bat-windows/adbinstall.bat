rem Please connect device to PC!
set apps=%1%
set APK_DEFAULT_PATH=C:\Users\atlas\Desktop\work_doucument\APK

if "%apps%"=="" (
rem 请输入对应的格式 adbinstall [sensor/compass]
rem sensor: AndroidSensorBox.apk SensorList.apk
rem compass: compass.pc6.apk
rem GPS : TDMonitor0.10.0(GPS_BEIDOU).apk
goto end
)
adb wait-for-device
adb root
adb devices
adb remount
if "%apps%"=="sensor" (
adb install %APK_DEFAULT_PATH%\AndroidSensorBox.apk
adb install %APK_DEFAULT_PATH%\SensorList.apk
)
if "%apps%"=="compass" adb install "%APK_DEFAULT_PATH%\compass.pc6.apk"
if "%apps%"=="GPS" adb install %APK_DEFAULT_PATH%\TDMonitor0.10.0(GPS_BEIDOU).apk

:end
pause
