#!/usr/bin/python

import os
import datetime

print "--------------------------------------"
print "programma c eseguito da secondo lancio"
print "--------------------------------------"
print " "
os.system ("ls -la *.py")

i = datetime.datetime.now()

txt = "scritto dato da programma c il %s/%s/%s alle %s:%s\n" % (i.day, i.month, i.year, i.hour, i.minute)

f = open("test.dat","a") 
f.write(str(txt))
f.close()