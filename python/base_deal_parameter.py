import sys, getopt
print sys.argv[1:]
print len(sys.argv)
opts, args = getopt.getopt(sys.argv[2:], "hi:o:")
print opts
print ("args %s"%args)
input_file=""
output_file=""
for op, value in opts:
    if op == "-i":
        input_file = value
        print ("input_file:%s"%input_file)
    elif op == "-o":
        output_file = value
        print ("output_file:%s" % output_file)
    elif op == "-h":
        print "help"
        sys.exit()