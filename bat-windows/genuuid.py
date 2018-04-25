import uuid
import string
My_uuid = uuid.uuid4()
##convert UUID to string 
My_uuid1 =  str(My_uuid)
##remove "-" from string
My_uuid1 = My_uuid1.replace('-', '') 
##
##print (My_uuid1)
###################### Converts the string to an array.
result = [0]*16
i= 0
b= My_uuid1
while i<16:
	a=str(b[0:2])
	b=b[2:]
	result[i]=a
	#print (result[i])
	i=i+1
######################
#####print array
print ("The sns_reg_common.h config :")
print str(result).replace('[\'', '{0x').replace('\']', '}').replace('\'', '').replace(' ', '0x')
#print (result)

##reverse the array
resultb = result
resultb.reverse()
resultb_1 = resultb[:8]
resultb_2 = resultb[8:16]
#print (resultb)
#print resultb_1
#print resultb_2

###print sensor config
print ("\nThe sensor_def_qcomdev.conf :")
print (str(resultb_1).replace('[\'','0x').replace('\'','').replace(', ','').replace(']',''))
print (str(resultb_2).replace('[\'','0x').replace('\'','').replace(', ','').replace(']',''))

raw_input( )
