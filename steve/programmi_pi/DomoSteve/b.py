#!/usr/bin/python

import os
import datetime

print "--------------------------------------"
print "programma b eseguito da secondo lancio"
print "--------------------------------------"
print " "
#os.system ("free")

i = datetime.datetime.now()
 
txt = "scritto dato da programma b il %d/%s/%s alle %s:%s\n" % (i.day, i.month, i.year, i.hour, i.minute)

f = open("test.dat","a") 
f.write(str(txt))
#f.write(txt)
f.close()
