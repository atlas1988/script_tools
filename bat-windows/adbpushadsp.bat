rem Please connect device to PC!
rem please cd adsp_proc\obj\8953\LA\system\etc\firmware
set ORIGIN_PATH=%cd%
:::%~dp0--ִ�нű���������·��
echo %ORIGIN_PATH%
rem �����Ӧ��adsp.�ļ�·��(format��adbpushadsp.bat adsp_proc\obj\8953\LA\system\etc\firmware)
if "%1%"=="" (
set ADSP_FRIME_PATH=E:\x80_cuizi\SDM636_LA21_0120\ADSP.VT.4.0\adsp_proc\obj\qdsp6v5_ReleaseG\660.adsp.prod\LA\system\etc\firmware
) else set ADSP_FRIME_PATH=%1%

::echo ADSP_FRIME_PATH=%
:: ���뵽ָ��·��
cd  %ADSP_FRIME_PATH%
dir 
adb wait-for-device
adb root
adb devices
adb remount
adb shell mount -o rw,remount /firmware
adb shell ls -l /firmware/image/adsp.*
adb shell rm /firmware/image/adsp.*
adb shell ls -l /firmware/image/adsp.*
adb push . /firmware/image/
adb shell ls -l /firmware/image/adsp.*
adb reboot
::�˻ص�ԭ·�� 
cd  %ORIGIN_PATH%
::pause
