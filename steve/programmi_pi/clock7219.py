#!/usr/bin/env python

# access to GPIO must be through root
import RPi.GPIO as GPIO
import time
import sys
import datetime

LATCH = 11          # CS      GP11   pin 23
CLK = 12            # CLOCK   GP12   pin 32
dataBit = 7         # DIN     GP07   pin 26

#definiamo il sistema di riferimento dei pin. Con GPIO.BCM usiamo i numeri GPIO dei pin e non
# il numero dei pin.
# Ad esempio con GPIO.setmode(GPIO.BCM) per riferirci al pin GPIO17, usiamo 17 (e non 11).
#Per indicare che ci si riferisce al numero del pin, si usa GPIO.setmode(GPIO.BOARD)
GPIO.setmode(GPIO.BCM)    #usiamo la numerazione BCM
GPIO.setwarnings(False)

GPIO.setup(LATCH, GPIO.OUT)      # P0    11
GPIO.setup(CLK, GPIO.OUT)        # P1    12
GPIO.setup(dataBit, GPIO.OUT)    # P7     7
#-------------------------------------------------------------------------------
# Setup IO
GPIO.output(LATCH, 0)
GPIO.output(CLK, 0)

#-------------------------------------------------------------------------------
def pulseCLK():
    GPIO.output(CLK, 1)
    # time.sleep(.001)
    GPIO.output(CLK, 0)
#-------------------------------------------------------------------------------
def pulseCS():
    GPIO.output(LATCH, 1)
    # time.sleep(.001)
    GPIO.output(LATCH, 0)
#-------------------------------------------------------------------------------
# shift byte into MAX7219               MSB out first!                  ?????????????
def DatoOut(value):         # era ssrOut convertito in DataOut
    for  x in range(0,8):   #il range va' da 0 a 7 serve per l'invio dei singoli bit
        temp = value & 0x80
        if temp == 0x80:            # testa se il bit 7 e' ON
           GPIO.output(dataBit, 1) # data bit HIGH  non carica i dati
        else:
           GPIO.output(dataBit, 0) # data bit LOW   carica i dati
        pulseCLK()                  # oppure OFF
        value = value << 0x01 # shift left serve per mandare i singoli bit (8)
#-------------------------------------------------------------------------------
def initMAX7219():           # initialize MAX7219 4 digits BCD
    # set decode mode
    writeMAX7219(0x09,0xFF)   # (CMD TAB 2, 4-bit BCD decode 8 digit TAB 4)

    # set intensity
    writeMAX7219(0x0A,0x00)   # (CMD TAB 2, 9/32 intensity TAB 5)

    # set scan limit 0-7 per 8 display=07 per 4 display=03
    writeMAX7219(0x0B,0x07)   # (CMD TAB 2, 8 digits di visualizz TAB 8)

    # set for normal operation
    writeMAX7219(0x0C,0x01)   # (CMD TAB 2, 01 normal operation, 00 shutdown)

    for x in range(1,9):    # conta 1, 2, 3, 4, 5, 6, 7, 8
        writeMAX7219(x,0x0F)    #(CMD TAB 2, 0x0F = blank) o altro carattere per riempire

#-------------------------------------------------------------------------------
def writeMAX7219(registro, dato):
    DatoOut(registro)           # invia prima il registro e
    DatoOut(dato)               #  poi il dato per arrivare ai 16 bit
    pulseCS()                   # colpo di data entry/load
#-------------------------------------------------------------------------------
def displayOff():           # da usare solo in uscita x shutdown
    writeMAX7219(0x0C,0x00)
#-------------------------------------------------------------------------------
def displayOn():            # set for normal operation
    writeMAX7219(0x0C,0x01)     # provare per vedere se funziona DOVREBBE!
#-------------------------------------------------------------------------------
def esempioOra():
    tempo = datetime.datetime.now()
    print ("Data e ora corrente = %s" % tempo)
    #print ("Data e ora ISO format = %s" % tempo.isoformat())
    print ("Anno corrente = %s" % tempo.year)
    print ("Mese corrente = %s" % tempo.month)
    print ("Data corrente (giorno) =  %s" % tempo.day)
    print ("Formato dd/mm/yyyy =  %s/%s/%s" % (tempo.day, tempo.month, tempo.year))
    print ("Ora corrente = %s" % tempo.hour)
    print ("Minuto corrente = %s" % tempo.minute)
    print ("Secondo corrente =  %s" % tempo.second)
    print ("Formato hh:mm:ss = %s:%s:%s" % (tempo.hour, tempo.minute, tempo.second))
    print ("Numero del giorno nella settimana (0=dom,1=lun): ", tempo.today().strftime("%w"))
#-------------------------------------------------------------------------------
def ore():
    writeMAX7219(0x08,OraD)
    writeMAX7219(0x07,OraU)
    #writeMA    X7219(0x06,10)
    writeMAX7219(0x05,MinutiD)
    writeMAX7219(0x04,MinutiU)
    #writeMAX7219(0x03,10)
    writeMAX7219(0x02,SecondiD)   # (data, position)
    writeMAX7219(0x01,SecondiU)
#-------------------------------------------------------------------------------
def giorni():
    writeMAX7219(0x08,GiornoD)
    writeMAX7219(0x07,GiornoU)
    writeMAX7219(0x06,10)
    writeMAX7219(0x05,MeseD)
    writeMAX7219(0x04,MeseU)
    writeMAX7219(0x03,10)
    writeMAX7219(0x02,AnnoD)   # (data, position)
    writeMAX7219(0x01,AnnoU)
#-------------------------------------------------------------------------------
#inizio esecuzione <<<<<<<<<<<<<<<<<--------------------------------------
initMAX7219()

# Then writes digits to MAX7219.
#esempioOra()
tempo = datetime.datetime.now()
OraD = int(tempo.hour / 10)
OraU = tempo.hour - (OraD * 10)
MinutiD = int(tempo.minute / 10)
MinutiU = tempo.minute - (MinutiD * 10)
SecondiD = int(tempo.second / 10)
SecondiU = tempo.second - (SecondiD * 10)

for x in range(0,15):
    writeMAX7219(0x0A,x)
    ore()
    time.sleep(0.10)
for x in range(15,0,-1):
    writeMAX7219(0x0A,x)
    ore()
    time.sleep(0.10)

time.sleep(1)

GiornoD = int(tempo.day / 10)
GiornoU = tempo.day - (GiornoD * 10)
MeseD = int(tempo.month / 10)
MeseU = tempo.month - (MeseD * 10)
AnnoT = tempo.year - 2000
AnnoD = int((AnnoT) / 10)
AnnoU = AnnoT - (AnnoD * 10)

for x in range(0,15):
    writeMAX7219(0x0A,x)
    giorni()
    time.sleep(0.10)
for x in range(15,0,-1):
    writeMAX7219(0x0A,x)
    giorni()
    time.sleep(0.10)

#sys.exit()

displayOff()        # da usare solo in uscita x shutdown
print ("Good by!")


# Two bytes are shifted in first being address, second being data.
# Works the same as two 74165 SSRs in series or 16-bits.
# LD "pulseCS()" clocks 16-bit address/data into working registers.

#  http://www.bristolwatch.com/ele2/rpi_count_max7219.htm

# The program presented below will allow Raspberry in Python and setup a MAX7219 display driver
#  to act as a four digit counter. The program was ported over from Arduino C to illustrate how
#  coding can be reused.
#
# The MAX7219 display driver can drive 8 digit multiplexed LED display or a 8X8 LED matrix.
# This is setup in software. It includes selectable internal binary-coded-decimal (BCD) decoding
#  which is used here.
#
# It can also be cut on/off by a single command, the number of digits displayed, and intensity
# are all selectable in software. See "def initMAX7219()" below.
#
# Refer to the block diagram above. 10-bit data input is broken into 2 8-bit data bytes the first
#  being an address and the second being data. Using the function writeMAX7219(digit, k) digit is
#   the digit value and k is position pointer.
#
# For example assuming BCD mode and digit = 1 and k = 1 a one will be displayed on the far right
#  of the display. writeMAX7219(digit, k) uses DatoOut() twice.
#
# BCD code is limited to 0-9. Here we take a for loop where i counts from 0-999. Each iteration
#  the value of i is sent to j which the MOD function j with 10 returns remainder value of 0-9.
#
# The position and value are output to the display. The j is then divided by 10.
# This done 4 times to cover all four digits. Then i will increment and the process will begin again.
#
# In the next section we will use the Python time and datetime function to create a LED real time clock.
# scrittura sul display : write (data,posizione)
# data=10 -> -      data=11 -> E   data=12 -> H    data=13 -> L    data=14 -> P   data=15 -> blank
#    {}