


##-##############################################################################
# Calcola l'area e ritorno area, colore e altezza
################################################################################
def area(area):
    # print area.lato1

    operazione= area.lato1*area.lato2

    return operazione # ritorna True se una condizione e' stata trovata
# fine area  #############################################################
def area2(valore):

    Dict_01 = { # nuovo dictionary che ripasso nel return
            'superfice': valore.lato1*2 + valore.lato2*2,
            'area': valore.lato1 * valore.lato2,
            'commento' : 'da area2',
            'locazione' : 'da pippo'
            }

    return Dict_01 # ritorna il dictionary Dict_01
# fine area2 #############################################################
def sfera(valore):

    Dict_02 = {
            # mi da' errore moltiplicare valore.raggio con valore.pigreco 3,1415.
            'volume': valore.varianza * 4 / 3 * valore.raggio * valore.raggio * valore.raggio * valore.pigreco / 10000,
            'commento': 'calcolato il volume',
            'proto': 'kkk',
            'varianza': valore.varianza,
            'protocollo': valore.protocollo
            }

    return Dict_02 # ritorna il dictionary Dict_02
# fine volume #############################################################

class Car(object):

    def __init__(self, model, color, company, speed_limit):		# ATTRIBUTI
        self.model = model
        self.color = color
        self.company = company
        self.speed_limit = speed_limit

    def start(self):											# METODI
        print("started")

    def stop(self):												# METODI
        print("stopped")

    def accelarate(self):										# METODI
        print("accelarating...")
        # accelarator functionality here"

    def change_gear(self, gear_type):							# METODI
        print("gear changed")
        # gear related functionality here"

