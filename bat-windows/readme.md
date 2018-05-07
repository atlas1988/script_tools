#	常用脚本文件
adbdisableverity.bat ----用来执行adb disable-verity 命令解决不能push的问题。  
rmsetup.bat ---- 删除手机开机向导
scriptpath.bat ---- 跳转到脚本所在的目录  
BeyondCompare.bat --- 删除验证证书 规避BeyondCompare证书问题
adbserver.bat ---- adb server重启  
adbroot.bat ---- 获取adb root 权限 &remount  
poweroff.bat ----  adb 关机手机  
adbtcp.bat ---- 设置通过wifi链接adb  
adbprintk.bat ---- 修改printk等级  
charge_poweroff.bat ---- adb 修改设置高通手机关机充电  
adblogkit.bat ---- 读取高通平台logkit保存的log 按时间保存到指定目录  
push_trx01_kl.bat ---- push 按键kl文件到指定目录 kaios系统  
adbinstall.bat ---- 根据参数 安装sensor相关apk到手机（adbinstall compass） 
 
* adbfastboot.bat ---- 将目标手机切换到fastboot 模式 期间延时5s  
	+ adbfastboot_modem.bat ----将指定目录的NON_HLOS.bin文件下载到手机
* sensor部分调试  
	+ adbsensor_sox83.bat ---- adb push so文件（sensors.ssc.so）到指定目录  
	+ adbpushadsp.bat ---- adb push adsp生成文件到设备
	+  getSensorHubInfo.bat ---- 获取sensor hub相关节点现象  
	+  adbgetsensor.bat ---- 调用getSensorHubInfo.bat获取信息  
	+  UpdateSensor_conf.bat --- 在system 目录搜索config文件文件并更新文件
	+  pushsensor_man.bat ---- 更新vendor/etc/sensors/目录下的sensor conf 文件  
	+  pushsensor_system.bat ---- 更新/system/etc/sensors目录下的sensor conf 文件  
	+  Pullsensor_conf.bat ---- pull sensor conf 文件到指定目录set DEFAULT_PATH=C:\Users\atlas\Desktop\debuginfo（自动去system目录查找文件找不到则到vendor目录查找）  
	+  genuuid.py ---- 自动生成uuid 并转化为高通需要的格式



  
## 重要语法： 
rem 根据当前日期获取，年月日串（2018-04-16_16_59_51） ---adblogkit.bat  
set filename=%date:~0,4%-%date:~5,2%-%date:~8,2%_%time:~0,2%_%time:~3,2%_%time:~6,2%  
set filename=%filename: =0%  
:: delay 5s --adbfastboot.bat  
@choice /t 5 /d y /n >nul  
：： 删除开机向导  
adb shell rm -rf /system/priv-app/SetupWizard  
：：跳转到脚本所在的目录  --scriptpath.bat
cd /d %~dp0  
：：执行命令所在的路径  
%cd%  
::删除指定文件  del d:\abc\a.txt  
::删除文件夹呢 --  BeyondCompare.bat
rmdir /q /s d:\abc  
：：当然一些特殊情况要修改属性才能删，比如指定文件，具体的说，只读文件是不能直接删除的  
attrib -r d:\abc\z.txt  
::如果目录为空目录则删除目录 ---adblogkit.bat 
dir /a /b %LOG_SAVE_PATH%|findstr .* >nul||rmdir /q /s %LOG_SAVE_PATH%  

set DEFAULT_PATH=C:\Users\atlas\Desktop\debuginfo\456  
echo %DEFAULT_PATH%  
dir /a /b %DEFAULT_PATH%|findstr .* >nul||rmdir /q /s %DEFAULT_PATH%  

：： 脚本中调用其他脚本的两种方式  ---adbgetsensor.bat  
:: getSensorHubInfo.bat --- 直接在当前终端执行  
start getSensorHubInfo.bat ----重新大可以一个终端执行  
：：adb 命令搜索对应文件将结果赋值给变量 --- UpdateSensor_conf.bat  
for /f "delims=" %%i in ( 'adb shell find /system -name sensor_def_qcomdev.conf ') do set SENSOR_PATH_InPhone=%%i   
::echo %SENSOR_PATH_InPhone% 
：：复制文件   
xcopy *.cpp *.cpptxt  
xcopy *.cpptxt *.cpp


