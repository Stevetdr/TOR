#!/usr/bin/env python
# connessione di un raspberry attraverso 2 FTDI tx-rx e rx-tx

import time
import serial

ser = serial.Serial(
  port='/dev/ttyUSB1',
  baudrate = 9600,
  parity=serial.PARITY_NONE,
  stopbits=serial.STOPBITS_ONE,
  bytesize=serial.EIGHTBITS,
  timeout=1
)  # open the port automatically

counter=0

while 1:
  ser.write('Contatore calcolato ed inviato: %d \n'%(counter))
  print('Contatore calcolato ed inviato: %d \n'%(counter))
  time.sleep(3)
  counter += 1