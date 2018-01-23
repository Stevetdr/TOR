#!/usr/bin/env python

# access to GPIO must be through root
import RPi.GPIO as GPIO
import time

TRIG = 25   #giallo
ECHO = 9    #verde
                                #ss add

#definiamo il sistema di riferimento dei pin. Con GPIO.BCM usiamo i numeri GPIO dei pin e non
# il numero dei pin.
# Ad esempio con GPIO.setmode(GPIO.BCM) per riferirci al pin GPIO17, usiamo 17 (e non 11).
#Per indicare che ci si riferisce al numero del pin, si usa GPIO.setmode(GPIO.BOARD)

print ("Distance Measurement In Progress...")

GPIO.setmode(GPIO.BCM)
GPIO.setup(TRIG,GPIO.OUT)
GPIO.setup(ECHO,GPIO.IN)

count = 0
average = 0
while count < 10:
    time.sleep(0.4)

    GPIO.output(TRIG, False)    # mette a zero

    GPIO.output(TRIG, True)     # picco positivo
    time.sleep(0.00001)         #attesa di 10 microsecondi
    GPIO.output(TRIG, False)    # picco a zero

    while GPIO.input(ECHO)==0:
        pulse_start = time.time()

    while GPIO.input(ECHO)==1:
        pulse_end = time.time()

    pulse_duration = pulse_end - pulse_start
    distance = pulse_duration * 17150
    distance = round(distance, 2)
    if (distance > 3400):
        print("Fuori portata o errore lettura!!!")
    else:
        print ("round nr: {}   Distance :{:6.2f} mm".format(count,distance*10))
    #print ("round nr:",count,"  Distance:",distance,"mm")
    #print ("round nr:",count,"  Distance:",distance*10,"mm")
    #print ("")
    average = average + distance
    count += 1

#print ("Average on 10 shot: ", average/10, "cm")
print ("Average on 10 shot: ", average, "mm")
GPIO.cleanup()