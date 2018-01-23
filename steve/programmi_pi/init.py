
#!/bin/env python3.4
import sys

#------------------------------------------------------------------
class tempo:
    def __ini__(self, Ore=0, Minuti=0, Secondi=0):
        self.Ore = Ore
        self.Minuti = Minuti
        self.Secondi = Secondi
#------------------------------------------------------------------
#print (" per prova mettere gli argomenti tipo init.py 10 12 24")
try:
    tempo.Ore = sys.argv[1]
    tempo.Minuti = sys.argv[2]
    tempo.Secondi = sys.argv[3]

    print ("argomento 0: ",sys.argv[0])
    print ("Sono le {0}:{1}:{2}".format(tempo.Ore,tempo.Minuti,tempo.Secondi))

except IndexError:
    print("Errore: mancano {0} parametri !!!!".format(4-len(sys.argv))) #
    print(" ho letto solo ",sys.argv[1])
    print(" ho letto solo ",sys.argv[2])

