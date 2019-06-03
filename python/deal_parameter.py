import sys, getopt
print sys.argv[1:]
print len(sys.argv)
parms=sys.argv[1:]
parms_n=len(sys.argv)
opts, args = getopt.getopt(sys.argv[1:], "hi::o:")
#result_opts=[]
result_args=[]
print opts
print ("args %s"%args)
#print args.remove('-o')
print ("args %s"%args)
#for i in range(1,len(args)):
i=0
while len(args) > 0:
    print ("lx start args= %d i= %d"%(len(args),i))
    i+=1
    opt, arg = getopt.getopt(args, "hi::o:")
    if not opt:
        print ("paramrter is empty!")
        ##remove the arg
        #print args
        if args:
            print(args[0])
            #result_args += str(args[0])
            result_args.append(str(args[0]))
            args.remove(args[0])
        continue
    else:
        ##deal the parameter remove the match
        opts +=opt
        print ("lx for 0")
        print ("args %s" % args)
        for parm_n in range(0,len(opt)):
            #print list(opt[0])
            #print len(opt)
            temp_parm = list(opt[parm_n])
            # print str(temp_parm[0])
            for parm_i in range(0,len(temp_parm)):
                print temp_parm[parm_i]
                if temp_parm[parm_i] in args:
                    args.remove(temp_parm[parm_i])
            print ("args %s" % args)
    print opt
    print arg

print("result 1")
print ("result_args %s"%result_args)
print opts

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