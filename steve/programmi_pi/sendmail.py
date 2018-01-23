#!/bin/env python3.4
import sys
import smtplib


msg = "Sono RaspBerry"



print('parametri da riga di comando')
# print (sys.argv)
# print (sys.argv[1:])
if len(sys.argv) > 1:
    msg = sys.argv[1]

print()
print('vado ad inviare il messaggio:')
print('....', msg)
sys.exit()


server = smtplib.SMTP('smtp.gmail.com', 587)
server.starttls()
server.login("stevetdruser@gmail.com", "1956fugona")


server.sendmail("stevetdruser@gmail.com", "stevetdr@gmail.com", msg)
server.sendmail("stevetdruser@gmail.com", "loreto.n@gmail.com", msg)
server.quit()
