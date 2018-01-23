
import sys

class printss:
    def __init__(self, variabile):
        print(" prova con classe strana, scrivo : ", variabile)
    #    print ("__name__: ", __name__)
        #print ("__builtins__: ", __builtins__)
        #print ("__cached__: ", __cached__)
    #    print ("__doc__: ", __doc__)
    #    print ("__file__: ", __file__)
        #print ("__loader__: ", __loader__)
    #    print ("__package__: ", __package__)
    #    print ("__spec__: ", __spec__)


class stampa:
    def printa(variabile):
        print (" STAMPA FUNZ printa ",variabile)
        print ("Questo e' il nome di chi chiama: ", sys.argv[0])

    def printb(variabile1):
        print (" stampa la funzione printb ",variabile1)
        print ("Questo e' il nome di chi chiama: ", sys.argv[0])


class tempo:
    def __ini__(self, Ore=0, Minuti=0, Secondi=0):
        self.Ore = Ore
        self.Minuti = Minuti
        self.Secondi = Secondi