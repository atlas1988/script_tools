import string

aa="     \tq   "
count_s=0
for c in aa:
    if c.isspace():
        count_s +=1
    if c =='\t':
        print count_s
        break
print ("exit %d" %count_s)
print aa