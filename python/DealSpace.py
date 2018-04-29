import sys
import os

######trailing-space remove the space/tab in the end of line
######strip --remove front and end ;lstrip()--remove front ; rstrip()---remove old
######
def remove_training_space(strline):
    strline = strline.rstrip()
    if strline != '':
        strline += '\n'
    return strline


####space-before-tab conert the spaces to TAB
####which the space before tab begin of the line
####
def replace_space_before_tab(line_str):
    ##count the number of space before TAB
    count_s = 0
    count_t = 0
    is_need = 0
    ##Determine if the string is empty and if the first letter is a space .
    if line_str and line_str[0].isspace():
        for c in line_str:
            if c.isspace():
                count_s += 1
            if c == '\t':
                #print (count_s)
                is_need = 1
                break
            if (c == '\_') or (c.isalpha() or c.isdigit()):
                is_need = 0
                break
        if is_need :
            count_s -= 1
            count_t = count_s / 4
            # count_t += 1
            ##covert the spaces to TAB
            return (line_str.replace(' ' * count_s, '\t' * count_t))
    ###If you don't meet the conditions. return string
    return (line_str)

###process the file
def process_space(fname):
    ##1.open the file
    f_r = open(fname,'r+')
    lines = f_r.readlines()
    #print ("The file: %s context :%s"%(fname,lines))
    f_r.close()
    ##read file one by one line
    with open(fname, "w") as f_w:
        for line in lines:
            # remove space in the end of lines remove space \t \r
            line = remove_training_space(line)
            # replace space to TAB which space before TAB in the front of lines
            #print ("===lx===tab")
            line = replace_space_before_tab(line)
            #print ("===lx 3===%s " % line)
            f_w.write(line)
    f_w.close()

## process the pile
def process_file(fname):
    ##ignore the suffix follow
    f_ignore = ['.o', '.so', '.a', '.bin', '.exe', '.py', '.i']
    f_need = 1
     # if (os.path.splitext(file)[1] != '.o') and (os.path.splitext(file)[1] != '.a') and (os.path.splitext(file)[1] != '.o') and (os.path.splitext(file)[1] != '.a'):
    for f_i in f_ignore:
        if os.path.splitext(fname)[1] == f_i:
            print ("Ignore the file:%s"%fname)
            f_need = 0
            break
    # print file name & deal file
    if f_need:
        print ("Deal file:%s"%fname)
        #Determine if there is a suffix.  #if os.path.splitext(fname)[1] == '.txt':
        if os.path.splitext(fname)[1]:
            process_space(fname)

### deal the git file
###
def convert_files(fpath):
    ##judge file is exist
    if not os.path.exists(fpath):
        print ("********The file is not exist  : %s"%fpath)
        return
    ##if parametr is a file
    if os.path.isfile(fpath):
        #print ("deal with files:%s"%fpath)
        process_file(fpath)
    else:
        print("deal with directory:%s"%fpath)
        # fpath is a directory obtain all files
        files = os.listdir(fpath)
        #deal the files one by one print files
        for file_list in files:
            if not os.path.isdir(file_list):
                process_file(file_list)
            else:
                print ("This is a direcotry %s !" % file_list)


###handle the directory parameter
if __name__ == '__main__':
    file_path = "."
    if len(sys.argv) < 2:
        print 'Usage: python file1 file2 ...\n default is the directory: .'
        convert_files(file_path)
        raw_input("Please Enter Enter key to exit!")
        sys.exit(1)
    for i in range(1,len(sys.argv)):
        print file_path
        file_path = os.path.normcase(sys.argv[i])
        print file_path
        convert_files(file_path)
    raw_input("Please Enter Enter key to exit!")


'''
###process the file
def process_space(fname):
    ##1.open the file
    f = open(fname,'r+')
    ##read file one by one line
    for line in f.readlines():
        print ("===lx 1===%s "%line)
        #dos format to unix format
        line = line.replace('\r\n', '\n')
        #remove space in the end of lines
        line = remove_training_space(line)
        print ("===lx 2===%s " % line)
        #replace space to TAB which space before TAB in the front of lines
        print ("===lx===tab")
        line = replace_space_before_tab(line)
        print ("===lx 3===%s " % line)
        f.writelines(line)
    f.close()
'''
'''
    b = sys.argv[0]
    print sys.argv[0]
    print b.replace('\\\\','\\')
    process_space("1111111.txt")
'''