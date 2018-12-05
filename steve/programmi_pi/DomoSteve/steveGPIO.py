#!/usr/bin/env python3

import RPi.GPIO as GPIO
import threading    # permette il lancio di programmi in parallelo
import time

sem = threading.Semaphore(0)

time23 = 0

# ------------------------------------------------------
def cb23(channel):
        global time23
        time23 = time.time()
        sem.release()
# ------------------------------------------------------
# ATTENZIONE, lavorare con la tensione di 3,3V e non con quella da 5V. Si rischiano danni !!!

GPIO.setmode(GPIO.BCM)      # si usano pin legati alla numerazione del connettore PI
#GPIO.setmode(GPIO.BOARD)    #  si usano pin legati alla numerazione del processore GPIOxx

GPIO.setup(23, GPIO.IN)     # seleziona un canale come input (e' il GPIO11)

GPIO.setup(22, GPIO.OUT, initial=GPIO.LOW)  # seleziona un canale per output, inizialmente a zero (LOW) GPIO25
#..................................................................................
GPIO.add_event_detect(23, GPIO.BOTH, callback=cb23)

value23 = 0
timeout = None
value22 = 0

while True:
        try:
                new=sem.acquire(timeout=timeout)    # La try esegue per prima questa istruzione
        except:
                break                               # Se durante l'esecuzione ci sono errori, break

        now = time.time()                           #  altrimenti .... prosegue sotto
        if now - time23 > 0.05:
                tmpvalue23 = GPIO.input(23)
                if tmpvalue23 != value23:
                        value23 = tmpvalue23
                        if value23 == 1:
                                value22 = 1 - value22
                                if value22 == 1:
                                        GPIO.output(22, GPIO.HIGH)
                                else:
                                        GPIO.output(22, GPIO.LOW)

        if new:
                timeout = 0.05
        else:
                timeout = None
#..................................................................................
print("cleanup canali alla chiusura del programma")
#GPIO.cleanup()     # chiudendo pulisce situazione pregressa di tutto
GPIO.cleanup(22)    # per esempio ho chiuso i channel separatamente
GPIO.cleanup(23)