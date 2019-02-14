# Distributed with a free-will license.
# Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
# PCF8574
# This code is designed to work with the PCF8574_LBAR_I2CL I2C Mini Module available from ControlEverything.com.
# https://www.controleverything.com/products

import smbus
import time

bus = smbus.SMBus(1)                # Get I2C bus
portaOUT = 0x3C                     # PCF8574 address, 0x3F(xx)     LED
portaIN = 0x3D                      # PCF8574 address, 0x20(32)     PULSANTI

#bus.write_byte(portaIN, 0xFF)          # 0xFF(255) All pins configured as inputs
#bus.write_byte(portaOUT, 0x00)          # 0x00(00)  All pins configured as outputs

time.sleep(0.2)

dataIN = bus.read_byte(portaIN)         # Read data back, 1 byte

print " "
print "PCF8574 portaIN  trovato su porta HEX: 0x%X (dec: %d)" %(portaIN, portaIN)
print "PCF8574 portaOUT trovato su porta HEX: 0x%X (dec: %d)" %(portaOUT, portaOUT)
print " "



print "-----"
bus.write_byte(portaOUT,0xFF)
time.sleep(0.5)

bus.write_byte(portaOUT,0x00)
time.sleep(0.5)
print "-----"




dataIN = (dataIN & 0xFF)                # Convert the data
dataOUT = dataIN

data1=[9,9,9,9,9,9,9,9]             #ci vuole per riempire la lista

for i in range(0, 8) :
    if (dataIN & (2 ** i)) == 0 :
        #print "_________________________________________"
        data1[i] = 0
        #print "I/O Pin %d State is 0  (valore matrice %d)" %(i, data1[i])
    else :
        #print "_________________________________________"
        data1[i] = 1
        #print "I/O Pin %d State is 1  (valore matrice %d)" %(i, data1[i])

print "Stato bit 7  6  5  4     3  2  1  0"
print "          %d  %d  %d  %d     %d  %d  %d  %d" %(data1[7],data1[6],data1[5],data1[4],data1[3],data1[2],data1[1],data1[0])
