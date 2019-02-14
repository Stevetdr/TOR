#!/usr/bin/python3.4
# -*- coding: utf-8 -*-
import sys
import time

from sht_sensor import Sht
sht = Sht(21, 17)

while (1):
    Temperature = sht.read_t()
    Humidity    = sht.read_rh()
    dew_point = sht.read_dew_point(Temperature, Humidity)
#print ('Temperature........:', Temperature)
#print ('Relative Humidity..:', Humidity)
#print()
    #print ('Temperature........: {0:0.2f}'.format(Temperature))
    #print ('Relative Humidity..: {0:0.2f}'.format(Humidity))

    print ('Gradi  {0:0.2f} °C      Umidita {1:0.2f} %       dew Point {2:0.2f}'.format(Temperature, Humidity, dew_point))
    time.sleep(1)
# https://github.com/kizniche/sht-sensor/blob/master/README.rst
# si deve lanciare con python3.4

