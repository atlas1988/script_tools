# python tools
1. 怎么将python 转换为可执行文件（How do you convert python（py） to executable?） 
	1.安装环境工具  
	* ubuntu/linux 环境下安装转换工具  
		+ sudo apt-get install python-pip  
		+ pip install pyinstaller -i -i https://pypi.douban.com/simple  
	* windows下安装环境 
		+ 安装pip 解压 pip-9.0.1.tar.gz 	解压之后进入目录安装 python setup.py install
		+ 如果上面步骤报错 先解压 setuptools-38.5.1.zip 解压之后进入目录安装 python setup.py 安装完成之后再重新上面的步骤安装pip

	2.转换文件  
			pyinstaller -F -w gitDealSpace.py   
		如果有多个.py文件希望打包成一个可执行文件，则指定多个.py文件完整路径。eg:  
		pyinstaller -F ./smsweb.py ./tool/*.py ./db/db.py  
	*****注意：在直接运行py文件的时候可以使用exit（0）返回，在转换为exe文件后不能直接使用，否则会报错，需要使用sys.exit(0).
	
