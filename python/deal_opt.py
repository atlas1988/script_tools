import sys, getopt

####deal the options
def deal_options(parameters,opttions):
    opts, args = getopt.getopt(parameters, opttions)
    # result_opts=[]
    result_args = []
    #print opts
    #print ("args %s" % args)
    # for i in range(1,len(args)):     i = 0
    while len(args) > 0:
        #print ("lx start args= %d i= %d" % (len(args), i))
        opt, arg = getopt.getopt(args, "hi::o:")
        if not opt:
            #print ("lx deal the parameter which do't need!")
            ##remove the arg
            # print args
            if args:
                #print(args[0])      # result_args += str(args[0])
                result_args.append(str(args[0]))
                args.remove(args[0])
            continue
        else:
            ##deal the parameter remove the match
            opts += opt
            #print ("lx for 0 \n args %s" % args)
            for parm_n in range(0, len(opt)):
                # print list(opt[0])  # print len(opt)
                temp_parm = list(opt[parm_n])
                # print str(temp_parm[0])
                for parm_i in range(0, len(temp_parm)):
                    #print temp_parm[parm_i]
                    if temp_parm[parm_i] in args:
                        args.remove(temp_parm[parm_i])
                #print ("args %s" % args)
    print ("result :\n result_args= %s \n opts = %s"%(result_args,opts))
    return opts,result_args

if __name__ == '__main__':
    opts, args = deal_options(sys.argv[1:], "hi::o:")
    print("lx deal parm")
    print opts
    print args
