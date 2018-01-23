#!/usr/bin/python3.4
#
# updated by ...: Steve prova
# Version ......: 18/12/17
# ######################################################################################.
import sys
#import os
from pathlib import Path    #ss nuova libreria che sostituisce la os
# ----------------------------------------------
# - Inserimento del path corretto della SsLib
# ----------------------------------------------
# Sotto: mette in sys.path[0], l'indirizzo di dove si trova la lib qui sotto
LnLibPath = Path(sys.argv[0]).resolve().parent / 'bin' / 'LnLib_2017-12-11.zip'
sys.path.insert(0, str(LnLibPath))   #ss inserisce in sys.path(0) LnLibPath

import  LnLib as Ln     # import librerie di Loreto provato lo spostamento in init ma non funziona

#LnLibrary = sys.path[0]
#.........................................................
# -SsLibPath = Path(sys.argv[0]).resolve().parent / 'SsLib' / 'JobLib.py'
# -sys.path.insert(0, str(SsLibPath))   #ss inserisce in sys.path(0) LnLibPath

#import  di SsLib.JobLib trasferito in __init__.py
#from SsLib.JobLib import area as AREA, area2 as AREA2
import SsLib as Ss
from SsLib.JobLib import sfera as Sfera
from SsLib.JobLib import Car as Auto

SsLibrary = sys.path[0]
#print ("1) Stampa il puntamento alla libreria: ",LnLibrary)
#print ("2) Stampa il puntamento alla libreria: ",SsLibrary)
#.........................................................

if __name__ == "__main__":  #ss il codice sotto viene eseguito solo se viene lanciato come primo programma
    area = Ln.Dict()        #ss classe Area oggetto Ln.Dict()
    area.lato1 = 50         # attributo valorizzato
    area.lato2 = 30         #   "
    area.colore = 'rosso'   #   "
    area.altezza = 1000     #   "
    area.commento = "?"     #   "

    sfera = Ln.Dict()
    sfera.raggio = 30
    sfera.pigreco = 31415   #errore??? 3,1415
    sfera.commento = "!!"
    sfera.varianza = 1

    print('--------------------------------------------')
    #x1 = AREA(area)             #ss chiamo la funzione AREA passando il dictionary area
    x2 = Ss.AREA2(area)

    print ('Superfice:',  x2['superfice'])
    print ('Area     :',  x2['area'])
    print ('Commento :',  x2['commento'])
    print ('Locazione :',  x2['locazione'])
    print('--------------------------------------------')

    print ('Calcolo del volume della sfera')

    sfera.varianza = 2
    #sfera.varianza = int(input('Inserisci la varianza (>0): '))

    sfera.protocollo = x2['locazione']

    Pt = Sfera(sfera)
    print ('Volume sfera   :', Pt['volume'])
    print ('Commento xx    :', Pt['commento'])
    print ('Varianza       :', Pt['varianza'])
    print ('Protocollo     :', Pt['protocollo'])


	#maruthi_suzuki = Car("ertiga", "black", "suzuki", 60)
    audi = Auto('A6', 'red', 'audi', 80)			#definizione classe
    fiat = Auto('tipo', 'black', 'fiat', 40)

    print(" --- 1")
    print('model       : ',audi.model)
    print('color       : ',audi.color)
    print('company     : ',audi.company)
    print('speed limit : ',audi.speed_limit)
    print(" --- 2")


    fiat.model ='Bravo'				# interessante!!!!! cambio nell'oggetto il valore

    print('model       : ',fiat.model)
    print('color       : ',fiat.color)
    print('company     : ',fiat.company)
    print('speed limit : ',fiat.speed_limit)
    print(" --- 3")




    sys.exit()







    print('')
    print('1 ++--- Inizio stampa dictionary: ')
    for key, val in Dict_02.items():
        print ('{:<12} = {}'.format(key,val))
    print('2 ++---  ----------------------***')
    print('')



    for key in x2.keys():
        print ('KEY=', key)
    print()

    for val in x2.values():
        print ('VALUE=', val)

    print(".............")

    for key, val in x2.items():
        print ('{:<12} = {}'.format(key,val))
    # print (area)








#cioè import la LnLib e poi crei una classe che si chiama xxx = Ln.Dict()



#import  Source as Prj       #ss importa da init sotto source ...vedi
#ss from Source.Process.DigitalPinSS import digitalToggle #ss as SSdigitalToggle
#ss altro modo di leggere il file toggle (come riga 41 di init
################################################################################
# - M A I N
################################################################################

        # ==========================================
        # = Preparazione del PAYLOAD  #ss parte definita dai campi fld
        # ==========================================


    #relay.printTree(header="relay object", fEXIT=True)



    fld                         = Ln.Dict() #ss altro oggetto fld
    fld.SRC_ADDR                = 0         #ss attributo
    fld.DEST_ADDR               = 1
    fld.SEQNO_H                 = 2
    fld.SEQNO_L                 = 3
    fld.RCODE                   = 4
    fld.CMD                     = 5
    fld.SUB_CMD                 = 6
    fld.COMMAND_DATA            = 7
    fld.PIN_NO                  = 7
    fld.PIN_ACTION              = 8
    # fld.printDict(header="fields names", fEXIT=True)

    rs485Prot                   = Ln.Dict() #ss altro oggetto
    rs485Prot.MasterAddress     = 1
    rs485Prot.STX               = 0x02
    rs485Prot.ETX               = 0x03
    rs485Prot.mode              = 'ascii'
    rs485Prot.CRC               = True
    rs485Prot.payloadFieldName  = fld           #ss dubbio!!!!
    # rs485Prot.printDict(header="rs485", fEXIT=True)


    mainCmd = {}    #ss altro modo di creare un dictionary
    mainCmd['RELAY_ECHO_CMD'] = 0x01    #ss questa riga e quella sotto sono = alla 73
    mainCmd['SLAVE_ECHO_CMD'] = 0x02

    mainCmd = {"RELAY_ECHO_CMD": 0x01, "SLAVE_ECHO_CMD": 0x02}

        # puntamento ai comandi e sottocomandi
    mainCmd                         = Ln.Dict() #ss
    mainCmd.RELAY_ECHO_CMD          = 0x01  #ss RELAY_ECHO_CMD = keys 0x01 = valore
    mainCmd.SLAVE_ECHO_CMD          = 0x02
    mainCmd.POLLING_CMD             = 0x03
    mainCmd.SET_PINMODE_CMD         = 0x21
    mainCmd.DIGITAL_CMD             = 0x31
    mainCmd.ANALOG_CMD              = 0x32
    mainCmd.PWM_CMD                 = 0x33

    subCmd                         = Ln.Dict() #ss
    subCmd.NO_REPLY                = 0x01     # for echo command
    subCmd.REPLY                   = 0x02     # for echo command
    subCmd.READ_PIN                = 0x04     # for analog/digital commands
    subCmd.WRITE_PIN               = 0x05     # for analog/digital commands
    subCmd.TOGGLE_PIN              = 0x06     # for digital commands
    #subCmd.printDict(header="Global vars", fEXIT=True)



    gv.rs485Prot  = rs485Prot       #ss "come si dice" collegamento al dictionary gv
    gv.mainCmd = mainCmd
    gv.subCmd  = subCmd
    #gv.printDict(header="Global vars", fEXIT=True)
                                    #ss fino a qui chiaro ... fino ad un certo punto !
    myRelay = Prj.openRs485Port(relay, rs485Prot)   #ss myRelay (oggetto) diventa port

    payload                 = bytearray(len(fld))   #ss ritorna il numero di byte di fld
    payload[fld.SRC_ADDR]   = rs485Prot.MasterAddress   #ss master pi=1 indirizzo



    Prj.digitalToggle(gv, myRelay, payload=payload) #ss richiamo la fuzione

    myRelay.Close()


    Ln.Exit(0, "completed", printStack=False, stackLevel=9, console=True)
    sys.exit()



#ss qua sotto metto i miei commenti e faccio riferimento alle righe, nel tuo programma

#ss print ("sys.argv[0] :",sys.argv[0]) #ss ->  sys.argv[0] : __main__.py
#ss ->  Path(sys.argv[0]).resolve()) : /home/pi/GIT-REPO/LnProtocol/py485_SS/__main__.py

#ss print ("Path(sys.argv[0]).resolve()) :",Path(sys.argv[0]).resolve())    #ss
#ss -> /home/pi/GIT-REPO/LnProtocol/py485_SS

#ss print ("Path(sys.argv[0]).resolve().parent) :",Path(sys.argv[0]).resolve().parent)  #ss
#ss print ("LnLibPath :",LnLibPath)   #ss
