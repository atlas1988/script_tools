import sys
import string

��һ���Ƕ������У�ֱ���滻
import re

import os



f=open(r"c:\a.txt", "r+")

for line in f.readlines():

	if line.find("world") >= 0:

		s = line.replace("world", "python")

		f.writelines(s)
	http://blog.csdn.net/gj19890923/article/details/51417396
http://www.360doc.com/content/10/1123/09/2036337_71633414.shtml	
ʹ��git�ύ�����ʱ�򣬾�������������trailing whitespace���ľ��棬����git�Դ���淶��У�飬�������в������Կո��β���Ա����ύ��Ч�Ĵ��롣
����������»������������:
crlf�ࣺ����Ҫ�ǲ�ͬ����ϵͳƽ̨�Ļ��б��벻ͬ����;������ȷʵ���Կո��β;
���ڵ�һ�����������ʹ��������������رմ�����:
git config core.autocrlf true
git config core.safecrlf true
���������ύ����ʱ��ֹ�����飺
git commit --no-verify-a
Ҳ������������
git config core.whitespace "trailing-space,space-before-tab"
git config apply.whitespace "trailing-space,space-before-tab"

��������һ���취��������pre-commit���������䣺
if(/\s$/){#bad_line("trailing whitespace", $_);}		
#####convert fileformat from dos to unix
#####git config --global core.autocrlf true/input/false 
#####eg:  filename = "test2.txt"   
#####     dos_to_unix(filename)  
def dos_to_unix(fname):  
    with open(fname,'rb+') as fobj:  
        data = fobj.read()  
        data = data.replace('\r\n', '\n')  
        fobj.seek(0, 0)  
        fobj.truncate()  
        fobj.write(data)  
    fobj.close()
  
######trailing-space remove the space/tab in the end of line
######strip --remove front and end �� lstrip()--remove front�� rstrip()---remove old
######
def remove_training_space(strline):
	strline.rstrip()


####space-before-tab conert the spaces to TAB 
####which the space before tab begin of the line
####
def replace_space_before_tab(line_str):
	##count the number of space before TAB
	count_s = 0
	count_t = 0
	for c in line_str :
		if c.isspace():
			count_s += 1
		if c == '\t':
			print (count_s)
			break
	count_t = count_s/4
	##covert the spaces to TAB
	return (line_str.replace(' '*count_s,'\t'*count_t))

###process the file
def process_file(fname):
	##1.
	dos_to_unix(fname)
	##2.remove_training_space
	
	##3.replace_space_before_tab
	


###handle the directory parameter

    
if __name__ == '__main__':
    if len(sys.argv) < 3:
        print 'Usage: python file1 file2 ...'
        exit(1)
    

