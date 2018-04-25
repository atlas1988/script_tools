rem del C:\Users\atlas\AppData\Roaming\Scooter Software\Beyond Compare 3!
:: del d:\abc\a.txt 
::删除文件夹呢，就是：rmdir /q /s d:\abc当然一些特殊情况要修改属性才能删，比如指定文件，又需要的话再说 
::具体的说，只读文件是不能直接删除的，你要用
::attrib -r d:\abc\z.txt
::去掉只读属性后才能删除。

rmdir /q /s "C:\Users\%USERNAME%\AppData\Roaming\Scooter Software\Beyond Compare 3"

::pause
