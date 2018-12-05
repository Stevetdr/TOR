#!/usr/bin/env python
# http://pyserial.readthedocs.io/en/latest/pyserial_api.html
# connessione di un raspberry attraverso 2 FTDI tx-rx e rx-tx
import time
import serial

ser = serial.Serial(
  port='/dev/ttyUSB2',
  baudrate = 9600,
  parity=serial.PARITY_NONE,
  stopbits=serial.STOPBITS_ONE,
  bytesize=serial.EIGHTBITS,
  timeout=None #1
)  # open the port automatically

counter=0

while 1:
  x=ser.readline()
  print x