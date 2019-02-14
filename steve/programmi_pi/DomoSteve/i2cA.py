from smbus import SMBus
from itertools import cycle
from time import sleep

LED1 = 0x01
LED2 = 0x02
LED3 = 0x04
LED4 = 0x08
LED5 = 0x10 
LED6 = 0x20
LED7 = 0x40
LED8 = 0x80

LD1 = ~ 0x24 #0xDB
LD2 = ~ 0x49 #0xB6
LD3 = ~ 0x92 #0x6D

PAT = (LD1, LD2, LD3)

PATTERN = (LED1, LED2, LED3, LED4, LED5, LED6, LED7, LED8,
           LED7, LED6, LED5, LED4, LED3, LED2)

bus = SMBus(1) # Port 1 used on REV2

#for LED in cycle(PATTERN):
#    bus.write_byte(0x3C, LED)
#    sleep(0.1)

for XX in cycle(PAT):
    bus.write_byte(0x3C, XX)
    sleep(0.2)