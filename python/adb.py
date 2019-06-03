import os
def getstatusoutput(cmd):
    '''return (status output) of executing cmd in the shell'''
    import os
    pipe = os.popen(cmd,'r')
    text = pipe.read()
    sts = pipe.close()
    if sts is None: sts = 0
    if text[-1:] == '\n': text = text[:-1]
    return sts, text
b,c =getstatusoutput("adb devices")
print b
print c
a = os.system("adb shell ls")
print a
#os.system("adb shell")
print os.system("pwd")
