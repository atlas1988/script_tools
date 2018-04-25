rem Please connect device to PC!
rem please cd adsp_proc\obj\8953\LA\system\etc\firmware
set ORIGIN_PATH=%cd%
:::%~dp0--执行脚本代码所在路径
echo %ORIGIN_PATH%
rem 输入对应的adsp.文件路径(format：adbpushadsp.bat adsp_proc\obj\8953\LA\system\etc\firmware)
if "%1%"=="" (
set ADSP_FRIME_PATH=E:\x80_cuizi\SDM636_LA21_0120\ADSP.VT.4.0\adsp_proc\obj\qdsp6v5_ReleaseG\660.adsp.prod\LA\system\etc\firmware
) else set ADSP_FRIME_PATH=%1%

::echo ADSP_FRIME_PATH=%
:: 进入到指定路径
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
::退回到原路径 
cd  %ORIGIN_PATH%
::pause
