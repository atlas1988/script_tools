rem Please connect device to PC!
:: ����Ĭ��pull�������ļ�·��
set DEFAULT_PATH=C:\Users\atlas\Desktop\debuginfo\log
adb wait-for-device
adb root
adb devices
adb remount
rem ���ݵ�ǰ���ڻ�ȡ�������մ�  
set filename=%date:~0,4%-%date:~5,2%-%date:~8,2%_%time:~0,2%_%time:~3,2%_%time:~6,2%
set filename=%filename: =0%
::echo %filename%
::����data�ļ�˵��·��
set LOG_PATH=/data/user/0/com.qualcomm.qti.logkit/files/
::echo %0 echo %1  �жϴ������
if "%1%"=="" (
rem pull sensor_def_qcomdev.conf�ļ���Ĭ��Ŀ¼
::�½�Ŀ¼
md %DEFAULT_PATH%\%filename%
set LOG_SAVE_PATH=%DEFAULT_PATH%\%filename%\
) else echo set SENSOR_CONFIG=%1%

::echo %LOG_SAVE_PATH%
echo %LOG_PATH%
echo %LOG_SAVE_PATH%
adb shell ls -l %LOG_PATH%
adb pull  %LOG_PATH% %LOG_SAVE_PATH%
::���Ŀ¼Ϊ��Ŀ¼��ɾ��Ŀ¼
dir /a /b %LOG_SAVE_PATH%|findstr .* >nul||rmdir /q /s %LOG_SAVE_PATH%
@echo 
:end
pause
