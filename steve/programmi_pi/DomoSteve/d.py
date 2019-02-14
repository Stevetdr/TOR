#!/usr/bin/python

import os
import datetime
import pickle

print "--------------------------------------"
print "programma d eseguito da secondo lancio"
print "--------------------------------------"
print " "

filepath = 'test.dat'
filepath1= 'test1.dat'

# -------------------------------------------------------------
fp = open(filepath,"r")

with open(filepath) as fp:
   line = fp.readline()
   cnt = 1
   while line:
       print("Record line nr. {}: {}".format(cnt, line.strip()))
       line = fp.readline()
       cnt += 1
fp.close()
# -------------------------------------------------------------

#^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
# take user input to take the amount of data
number_of_data = 6  #int(input('Enter the number of data : '))
data = []
#dato(1) = 12.3
#dato(2) = 5,567
#dato(3) = 'pippo'

##for i in range(number_of_data):			# take input of the data
##    raw = input('Enter data '+str(i)+' : ')
##    data.append(raw)
raw = 12.3				# dati inseriti per prova ...............................
data.append(raw)
raw = 123,321
data.append(raw)
raw = 'pipo','pluto'
data.append(raw)		
raw = '1','10101010'
data.append(raw)
raw = '2','00100100'
data.append(raw)
raw = '3','10101001'
data.append(raw)		# fine dati inseriti per prova ...............................

file = open('test1.dat', 'wb')    # open a file, where you ant to store the data
pickle.dump(data, file)		# dump information to that file
file.close()				# close the file

#^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
file = open('test1.dat', 'rb')  # open the file, where you stored the pickled data
data = pickle.load(file)	# dump information to that file
file.close()				# close the file

print('Showing the pickled data:')

cnt = 0
for item in data:
    print('The data ', cnt, ' is : ', item)
    #print item
    cnt += 1

#^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^