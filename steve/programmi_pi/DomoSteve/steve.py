#!/usr/bin/python3

import RPi.GPIO as GPIO
import os
from threading import Thread   # permette il lancio di programmi in parallelo
import datetime
#import time
from time import sleep # Import only the sleep function from the time module
#------------------------------------------------------------

i = datetime.datetime.now()
print "------------------------"
print "Ciao, Steve, tutto bene?"
print ""
print "Qui a Torre e' il %s/%s/%s e sono le %s:%s" % (i.day, i.month, i.year, i.hour, i.minute)
print "------------------------"

GPIO.setwarnings(False) # Ignore warning for now    ????
GPIO.setmode(GPIO.BOARD)      # si usano pin legati alla numerazione del connettore PI

Led16 = 16
Button18 = 18
Button22 = 22
Button24 = 24

GPIO.setup(Led16, GPIO.OUT, initial=GPIO.LOW)  # seleziona un canale per output, inizialmente a zero (LOW) GPIO23
GPIO.setup(Button18, GPIO.IN)     # seleziona un canale come input (e' il GPIO24)
GPIO.setup(Button22, GPIO.IN)     # seleziona un canale come input (e' il GPIO28) ??
GPIO.setup(Button24, GPIO.IN)     # seleziona un canale come input (e' il GPIO08) ??
#GPIO.setup(Button, GPIO.IN, pull_up_down=GPIO.PUD_UP)
#===================================================================

#===================================================================
presses = 1
contact = 0

def my_callback(Button):
    global presses
    # only count Falling edges, discard anything else
    if GPIO.input(Button) == 0:
        print "FALLING\t", presses
        presses += 1
        contact = 1
    return # not needed, just for clarity
#-------------------------------------------------------------------
def blink(nr):
    #b = 0
    #contact = 0
    #print "restart loop"
    print nr
    while (nr >= 0) : # Run forever
        print "---"
        GPIO.output(Led16, GPIO.HIGH) # Turn on
        sleep(0.3) # Sleep for 1 second        #print "++"
        GPIO.output(Led16, GPIO.LOW) # Turn off
        sleep(0.3) # Sleep for 1 second
        nr = nr - 1
#-------------------------------------------------------------------
t = 0
while (t <= 5):
    B18 = GPIO.input(Button18)
    B22 = GPIO.input(Button22)
    B24 = GPIO.input(Button24)
    print "giro nr:        ",t
    print " "
    if B18 == 0:
        print "pulsante 18 a zero"
    else:
        print "pulsante 18 a uno "
        blink(2)

    if B22 == 0:
        print "pulsante 22 a zero"
    else:
        print "pulsante 22 a uno "
        blink(3)

    if B24 == 0:
        print "pulsante 24 a zero"
    else:
        print "pulsante 24 a uno "
        blink(1)

    t = t +1
    sleep(1.5)

GPIO.cleanup()



# ## GPIO.add_event_detect(Button, GPIO.FALLING, callback=my_callback)
# ##
# ## try:
# ##     print "Waiting"
# ##     while (contact <= 0):
# ##         blink()
# ##
# ## except KeyboardInterrupt:
# ##     pass
# ## finally:
# ##     print "\nPulizia di tutti i canali GPIO usati"
# ##     GPIO.cleanup([Button])

#-------------------------------------------------------------------
def main():
    pass
#-------------------------------------------------------------------
if __name__ == '__main__':
    main()
#-------------------------------------------------------------------

