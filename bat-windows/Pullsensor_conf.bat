rem Please connect device to PC!
:: 设置默认pull下来的文件路径
set DEFAULT_PATH=C:\Users\atlas\Desktop\debuginfo

rem 根据当前日期获取，年月日串  
set filename=%date:~0,4%-%date:~5,2%-%date:~8,2%_%time:~0,2%_%time:~3,2%_%time:~6,2%
set filename=%filename: =0%
::echo %filename%

::echo %0 echo %1  判断传入参数
if "%1%"=="" (
rem pull sensor_def_qcomdev.conf文件到默认目录
::新建目录
md %DEFAULT_PATH%\%filename%
set SENSOR_CONFIG=%DEFAULT_PATH%\%filename%\sensor_def_qcomdev.conf
) else echo set SENSOR_CONFIG=%1%

::echo %SENSOR_CONFIG%
adb wait-for-device
adb root
adb devices
adb remount
:: 设置手机中sensor_def_qcomdev.conf 位置
::/system/etc/sensors/sensor_def_qcomdev.conf /system/vendor/etc/sensors/sensor_def_qcomdev.conf
:: set SENSOR_PATH_InPhone=/system/etc/sensors/sensor_def_qcomdev.conf
for /f "delims=" %%i in ( 'adb shell find /system -name sensor_def_qcomdev.conf ') do set SENSOR_PATH_InPhone=%%i 
::echo %SENSOR_PATH_InPhone%

if "%SENSOR_PATH_InPhone%"==""(for /f "delims %%j in ( 'adb shell find /system -name sensor_def_qcomdev.conf ') do set SENSOR_PATH_InPhone=%%j
	) else(
		echo wwwwww
		for /f "delims=" %%i in ( 'adb shell find /vendor -name sensor_def_qcomdev.conf ') do set SENSOR_PATH_InPhone=%%i 
	)
	
echo %SENSOR_PATH_InPhone%
adb shell ls -l %SENSOR_PATH_InPhone%
adb pull  %SENSOR_PATH_InPhone% %SENSOR_CONFIG%
@echo 
:end
pause
