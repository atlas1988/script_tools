rem Please connect device to PC!
:: ����Ĭ��pull�������ļ�·��
set DEFAULT_PATH=C:\Users\atlas\Desktop\debuginfo

rem ���ݵ�ǰ���ڻ�ȡ�������մ�  
set filename=%date:~0,4%-%date:~5,2%-%date:~8,2%_%time:~0,2%_%time:~3,2%_%time:~6,2%
set filename=%filename: =0%
::echo %filename%

::echo %0 echo %1  �жϴ������
if "%1%"=="" (
rem pull sensor_def_qcomdev.conf�ļ���Ĭ��Ŀ¼
::�½�Ŀ¼
md %DEFAULT_PATH%\%filename%
set SENSOR_CONFIG=%DEFAULT_PATH%\%filename%\sensor_def_qcomdev.conf
) else echo set SENSOR_CONFIG=%1%

::echo %SENSOR_CONFIG%
adb wait-for-device
adb root
adb devices
adb remount
:: �����ֻ���sensor_def_qcomdev.conf λ��
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
