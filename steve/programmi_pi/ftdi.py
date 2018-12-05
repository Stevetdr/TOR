import serial
import time

port = "/dev/ttyUSB1"  # for the first adapter
echo = "testo inviato!"

for count in range(500):
    wait = float(count) / 100

    print("valore di wait: {:.2f} ".format(wait))

    time.sleep(wait)
    #---------------------------------------------------------------------------
    with serial.Serial(port, baudrate=115200, timeout=2.0) as tty:
        t0 = time.time()        #fissa il tempo inizio in t0
       #b = tty.write(echo)
        assert tty.write(echo) == len(echo)  # errore se
        #        print echo
        #a = tty.read(len(echo))
        assert tty.read(len(echo)) == echo #,"print ("errore nella lunghezza letta!!!")"
        t1 = time.time()        #fissa il tempo fine in t1

    print("{:.2f} sec close to open, {:6.1f} ms to echo".format(wait, (t1-t0)*1000))