# updated by ...: Loreto Notarantonio
# Version ......: 23-11-2017 16.15.59

from    pathlib import Path

import  LnLib as Ln; C=Ln.Color()

#######################################################
# LOG options
#######################################################
def logOptions(myParser, defaultLogFile):

    logGroup = myParser.add_mutually_exclusive_group(required=False)  # True indica obbligatorietà di uno del gruppo

        # ---------------------------------------
        # - devo mettere un carattere prima
        # - altrimenti da errore a causa
        # - dei char speciali del colore.
        # ---------------------------------------
    mySeparatorText = '-' + C.getColored(color=C.magentaH, text='---------------log options ----')
    myParser.add_argument(mySeparatorText,
                                required=False,
                                action='store_true',
                                help=Ln.coloredHelp('', default=None))

        # log debug su console
    logGroup.add_argument( "--log-console",
                                metavar='',
                                required=False,
                                default=False,
                                nargs='*',
                                help=Ln.coloredHelp("""attivazione log sulla console.
    E' possibile indicare una o più stringhe
    per identificare le funzioni che si vogliono inserire nel log.
    Possono essere anche porzioni di funcName separate da ' ' Es: pippo pluto ciao""", default=False))

        # log debug su file
    logGroup.add_argument('--log',
                                metavar='',
                                required=False,
                                default=False,
                                nargs='*',
                                help=Ln.coloredHelp("""attivazione log sul file.
    E' possibile indicare una o più stringhe
    per identificare le funzioni che si vogliono inserire nel log.
    Possono essere anche porzioni di funcName separate da ' ' Es: pippo pluto ciao
    verra' utilizzao il file di log definito tramite --log-filename.""", default=False))


        # definizione file di log
    myParser.add_argument('--log-filename',
                                metavar='',
                                required=False,
                                default=defaultLogFile,
                                help=Ln.coloredHelp('log fileName... (valid only with --log option specified)', default=defaultLogFile))




