import serial
import time

port = "/dev/ttyUSB1"  # for the first adapter
echo = "testo inviato!"

tty = serial.Serial(port, baudrate=115200, timeout=2.0)

if tty.isOpen():
     print(tty.name + ' is open...')

for count in range(100):
    tty.write(echo)

    print "ho scritto: ", echo
    time.sleep(2)

    a = tty.read(len(echo))

    print "ho letto: ", a
    print "------------"