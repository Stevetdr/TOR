# Distributed with a free-will license.
# Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
# PCF8574
# This code is designed to work with the PCF8574_LBAR_I2CL I2C Mini Module available from ControlEverything.com.
# https://www.controleverything.com/products

import smbus
import time
from time import sleep
#from itertools import cycle

bus = smbus.SMBus(1)                # Get I2C bus
portaOUT = 0x3C                     # PCF8574 address, 0x3F(xx)     LED
portaIN = 0x3D                      # PCF8574 address, 0x20(32)     PULSANTI

LD1 = ~ 0x24 #0xDB
LD2 = ~ 0x49 #0xB6
LD3 = ~ 0x92 #0x6D

LD4 = ~ 0x81
LD5 = ~ 0x42
LD6 = ~ 0x24
LD7 = ~ 0x18

PAT = (LD1, LD2, LD3)
PAT1 = (LD3, LD2, LD1)
PAT2 = (LD4, LD5, LD6, LD7)

print " "
print "PCF8574 portaIN  trovato su porta HEX: 0x%X (dec: %d)" %(portaIN, portaIN)
print "PCF8574 portaOUT trovato su porta HEX: 0x%X (dec: %d)" %(portaOUT, portaOUT)
print " "
bus.write_byte(portaOUT, 0x00)      # 0x00(00)  All pins configured as outputs
sleep(0.5)
bus.write_byte(portaOUT, 0xFF)      # 0xFF(255) All pins configured as inputs
sleep(0.5)

while (1):
    dataIN = bus.read_byte(portaIN)         # Read data back, 1 byte
    #print dataIN
    #------------------------------
    if (dataIN == 0x01):
        bus.write_byte(portaOUT, 0x55)
    # . . . . . . . . . . . . . . . . . . . .
    if (dataIN == 0x02):
        bus.write_byte(portaOUT, 0xAA)
    # . . . . . . . . . . . . . . . . . . . .
    if (dataIN == 0x04):
        for i in range(7):
            for XX in PAT:
                bus.write_byte(portaOUT, XX)
                sleep(0.2)
        bus.write_byte(portaOUT, 0xFF)
    # . . . . . . . . . . . . . . . . . . . .
    if (dataIN == 0x08):
        for i in range(7):
            for XX in PAT1:
                bus.write_byte(portaOUT, XX)
                sleep(0.2)
        bus.write_byte(portaOUT, 0xFF)
    # . . . . . . . . . . . . . . . . . . . .
    if (dataIN == 0x10):
        for i in range(7):
            for XX in PAT2:
                bus.write_byte(portaOUT, XX)
                sleep(0.2)
        bus.write_byte(portaOUT, 0xFF)
    # . . . . . . . . . . . . . . . . . . . .
    if (dataIN == 0x80):
        bus.write_byte(portaOUT, 0xFF)
        break

print "Uscita con tasto"




    #dataOUT = ~ dataIN
    #bus.write_byte(portaOUT,dataOUT)

