#!/usr/bin/python

from thread import start_new_thread
import time

# ==========================================
def heron(a):

    print ("Eseguo : %s   " % a)
    print ("")
    milli_sec = int(round(time.time() * 1000))
    print(milli_sec)
    delay = a * 2
    print ("  fine lavoro :%s" % a)

    #while True:
    #    break
    #return new
# ==========================================

start_new_thread(heron,(5,))
start_new_thread(heron,(3,))
start_new_thread(heron,(1,))

delay = 2
time.sleep(delay)
print ("")
c = raw_input("Type something to quit.")