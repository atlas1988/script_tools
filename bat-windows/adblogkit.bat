rem Please connect device to PC!
:: 设置默认pull下来的文件路径
set DEFAULT_PATH=C:\Users\atlas\Desktop\debuginfo\log
adb wait-for-device
adb root
adb devices
adb remount
rem 根据当前日期获取，年月日串  
set filename=%date:~0,4%-%date:~5,2%-%date:~8,2%_%time:~0,2%_%time:~3,2%_%time:~6,2%
set filename=%filename: =0%
::echo %filename%
::设置data文件说在路径
set LOG_PATH=/data/user/0/com.qualcomm.qti.logkit/files/
::echo %0 echo %1  判断传入参数
if "%1%"=="" (
rem pull sensor_def_qcomdev.conf文件到默认目录
::新建目录
md %DEFAULT_PATH%\%filename%
set LOG_SAVE_PATH=%DEFAULT_PATH%\%filename%\
) else echo set SENSOR_CONFIG=%1%

::echo %LOG_SAVE_PATH%
echo %LOG_PATH%
echo %LOG_SAVE_PATH%
adb shell ls -l %LOG_PATH%
adb pull  %LOG_PATH% %LOG_SAVE_PATH%
::如果目录为空目录则删除目录
dir /a /b %LOG_SAVE_PATH%|findstr .* >nul||rmdir /q /s %LOG_SAVE_PATH%
@echo 
:end
pause
