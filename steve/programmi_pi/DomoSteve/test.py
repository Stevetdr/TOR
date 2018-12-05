#!/usr/bin/python

import thread
import time


#-----------------------------------------------------------
# Define a function for the thread
def print_time( threadName, delay):
    count = 0
    while count < 5:
        time.sleep(delay)
        count += 1
        print "%s: %s" % ( threadName, time.ctime(time.time()) )
#-----------------------------------------------------------
def printSS():
    print "stampa il pollo"
#-----------------------------------------------------------
print "versione 27-00"
print ""
# Create two threads as follows
try:
    #thread.start_new_thread( print_time, ("Thread SS-1", 2, ) )
    #thread.start_new_thread( print_time, ("Thread-2", 4, ) )
    thread.start_new_thread( printSS() )
except:
    print "Error: unable to start thread"
#-----------------------------------------------------------

while 1:
    pass
    thread.join()
