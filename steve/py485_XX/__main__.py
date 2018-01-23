#!/usr/bin/python3.4
# -*- coding: iso-8859-1 -*-
# -*- coding: utf-8 -*-
#
# updated by ...: Loreto Notarantonio prova
# Version ......: 11-12-2017 09.09.38
#                                               by Loreto Notarantonio
# ######################################################################################
import sys; sys.dont_write_bytecode = True  #ss https://docs.python.org/2/library/sys.html
import os
from pathlib import Path    #ss lettura libreria. Imposta class   ?? dove lo cerca e dove lo trova
# ----------------------------------------------
# - Inserimento del path corretto della LnLib
# - Le path per LnLib vanno impostate prima di fare gli import
# ----------------------------------------------
#ss questa parte permette di identificare sempre dove si trovano le librerie LnLib...
#print ("sys.argv[0]      : ",sys.argv[0])                                           # risultato    __main__.py
#print ("Path(sys.argv[0]): ",Path(sys.argv[0]))                                     # risultato    __main__.py
#print ("Path(sys.argv[0]).resolve().parent: ",Path(sys.argv[0]).resolve().parent)   # /home/pi/GIT-REPO/LnProtocol/py485_XX

LnLibPath = Path(sys.argv[0]).resolve().parent / 'bin' / 'LnLib_2017-12-11.zip'     # aggiunge il puntamento al file zippato

#print ("LnLibPath : ",LnLibPath)    # risultato     /home/pi/GIT-REPO/LnProtocol/py485_XX/bin/LnLib_2017-12-11.zip

sys.path.insert(0, str(LnLibPath))   #ss poi inserisce in sys.path (nella posizione 0, la prima) la nuova path
# si potrebbe anche fare sys.path.append(str(LnLibPath)) ma lo metterebbe alla fine

#print ("sys.path = ",sys.path)      # solo per visualizzare che il path e' stato inserito, li visualizza tutti

import  LnLib as Ln         # a questo punto si puo' importare la libreria LnLib
import  Source as Prj       #ss importa da init sotto source ...vedi
#ss from Source.Process.DigitalPinSS import digitalToggle #ss as SSdigitalToggle
#ss altro modo di leggere il file toggle (come riga 41 di init
################################################################################
# - M A I N
################################################################################
if __name__ == "__main__":  #ss il codice sotto viene eseguito solo se viene lanciato come primo programma
    gv      = Ln.Dict() #ss gv oggetto dictionary passabile, serve per la riga 95

        # ==========================================
        # = Preparazione del PAYLOAD  #ss parte definita dai campi fld
        # ==========================================

    relay      = Ln.Dict()      #ss classe - nuovo oggetto relay
    relay.port = '/dev/ttyUSB0' #ss attributo
    relay.mode = 'ascii'        #ss attributo
    relay.baudrate = 9600       #ss attributo

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
    rs485Prot.payloadFieldName  = fld           #ss inserisce il dictionary fld gia' costruito
    #rs485Prot.printDict(header="rs485", fEXIT=True)     # asteriscare

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
    #print (myRelay)
    #sys.exit()
    payload                 = bytearray(len(fld))   #ss ritorna il numero di byte di fld
    payload[fld.SRC_ADDR]   = rs485Prot.MasterAddress   #ss master pi=1 indirizzo

    #ss programma attualmente che viene eseguito
    Prj.digitalToggle(gv, myRelay, payload=payload) #ss richiamo la funzione

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
