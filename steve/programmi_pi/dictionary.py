
#!/bin/env python3.4
import sys

#studente {}

class Studente:
    def __init__(pippo, nome, cognome, corso_di_studi, eta, residenza):
        pippo.nome = nome
        pippo.cognome = cognome
        pippo.corso_di_studi = corso_di_studi
        pippo.eta_studente = eta
        pippo.residenza = residenza

    def scheda_personale(pippo):
        print ("Scheda studente:")
        print ("       Cognome: {0}".format(pippo.cognome))
        print ("          Nome: {0}".format(pippo.nome))
        print ("         Corso: {0}".format(pippo.corso_di_studi))
        print ("           Eta: {0}".format(pippo.eta_studente))
        print ("     Residenza: {0}".format(pippo.residenza))
        return (True)

#alunno[0] = Studente("Py","Mike","programmazione",20,"London")
studente_uno = Studente("Py","Mike","programmazione",20,"London")
studente_due = Studente("Marta","Stannis","scienze politiche",12, "Cina")
studente_tre = Studente("Steve","Sfregola","fuori corso",45,"Italy")

#print(alunno[0].scheda_personale())
print(studente_uno.scheda_personale())
print(studente_due.scheda_personale())
print(studente_tre.scheda_personale())
#print(Studente)