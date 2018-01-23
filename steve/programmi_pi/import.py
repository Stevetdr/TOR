
#!/bin/env python3.4
import sys
from stevetestlib import *
#from stevetestlib import printa
#from stevetestlib import printb
#from stevetestlib import printss

#print ("__name__: ", __name__)
#print ("__doc__: ", __doc__)
#print ("__file__: ", __file__)
#print ("__package__: ", __package__)
#print ("__spec__: ", __spec__)
#------------------------------------------------------------------

#print ("Questo 1: ", __name__)
printss("crosta")
#print ("Questo 2: ", __name__)
stampa.printa("pollo")
#print ("Questo : ", sys.argv[0])
#printb("gallo")
#print ("Questo : ", sys.argv[0])

sys.exit()

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

