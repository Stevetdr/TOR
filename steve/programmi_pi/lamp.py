
#!/bin/env python3.4
import RPi.GPIO as GPIO
import time


GPIO.setmode(GPIO.BCM)    #usiamo la numerazione BCM
GPIO.setwarnings(False)

GPIO.setup(17,GPIO.OUT)     #imposto Pin 17 ad out,
GPIO.setup(18,GPIO.OUT)     #imposto Pin 18 ad out,
#print ("LED on")

for count in (range(1,11)):
    GPIO.output(17,True)        #accendo il led,
    GPIO.output(18,False)       #spengo il led,
    time.sleep(0.5)               #attendo
    #print ("LED off")
    GPIO.output(17,False)       #spengo il led,
    GPIO.output(18,True)        #accendo il led,
    time.sleep(0.5)               #attendo




GPIO.output(18,False)       #accendo il led, corrispondente al comando: gpio write 0 1
GPIO.output(17,False)       #accendo il led, corrispondente al comando: gpio write 0 1
