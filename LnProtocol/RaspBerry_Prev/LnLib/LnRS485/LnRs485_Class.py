#!/usr/bin/env python3
#
# modified by Loreto:           2017-03-10_14.50.01
# #####################################################

__author__   = 'Loreto Notarantonio'
__email__    = 'nloreto@gmail.com'

__version__  = 'LnVer_2017-08-14_13.49.11'
__status__   = 'Beta'

import os
import serial       # sudo pip3.4 install pyserial
import sys
import inspect
import string
import time

def LnClass(): pass

####################
## Default values ##
####################
# Several instrument instances can share the same serialport
_SERIALPORTS = {}


BYTESIZE = 8
"""Default value for the bytesize (int)."""

PARITY   = serial.PARITY_NONE
"""Default value for the parity. See the pySerial module for documentation. Defaults to serial.PARITY_NONE"""

STOPBITS = 1
"""Default value for the number of stopbits (int)."""

read_TIMEOUT  = 0.05
"""Default value for the timeout value in seconds (float)."""

# CLOSE_PORT_AFTER_EACH_CALL = False
# CLOSE_PORT_AFTER_EACH_CALL = True
"""Default value for port closure setting."""

#####################
## Named constants ##
#####################

MODE_RTU   = 'rtu'
MODE_ASCII = 'ascii'
##############################
## Modbus instrument object ##
##############################


# Ln_SOURCE_ADDR = 0
# Ln_DEST_ADDR   = 1
# Ln_SEQNO_HIGH  = 2
# Ln_SEQNO_LOW   = 3
# Ln_COMMAND     = 4

# DATALEN             = 0     # - lunghezza dei dati escluso STX ed ETX
# SENDER_ADDR         = 0     # - Dest Address      (FF = Broadcast)
# DESTINATION_ADDR    = 1     # - source Address    (00 = Master)
# SEQNO_HIGH          = 2     # - numero del messaggio utile per associare la risposta
# SEQNO_LOW           = 3     # -
# COMMAND             = 4     # - comando da eseguire
# USER_RCODE          = 5     # - esito del comando
# USER_DATA           = 6     # - dati necessari per l'esecuzione del comando oppure i dati di risposta

# DATALEN           = 0    # - lunghezza dei dati escluso STX ed ETX
SENDER_ADDR       = 0    # - Dest Address      (FF = Broadcast)
DESTINATION_ADDR  = 1    # - source Address    (00 = Master)
SEQNO_HIGH        = 2    # - numero del messaggio utile per associare la risposta
SEQNO_LOW         = 3    # -
CMD_RCODE         = 4    # rCode di ritorno per il comando eseguito (in TX è ignorato)
COMMAND           = 5    # comando da eseguire
SUBCOMMAND        = 6    # eventuale dettaglio per il comando
COMMAND_DATA      = 7    # TX - dati necessari al comando per la sua corretta esecuzione/RX - dati di risposta



# -----------------------------------------------------------------------
# - con il gioco del complementedByte, gli unici byte che dovrebbero
# - circolare sono i seguenti al di là dello STX ed ETX
# -----------------------------------------------------------------------
LnRs485_validBytesHex = [
                '0x0F',
                '0x1E',
                '0x2D',
                '0x3C',
                '0x4B',
                '0x5A',
                '0x69',
                '0x78',
                '0x87',
                '0x96',
                '0xA5',
                '0xB4',
                '0xC3',
                '0xD2',
                '0xE1',
                '0xF0'
              ]



rs485Map = {            'DATALEN'           : 0,    # - lunghezza dei dati escluso STX ed ETX
                        'SENDER_ADDR'       : 1,    # - Dest Address      (FF = Broadcast)
                        'DESTINATION_ADDR'  : 2,    # - source Address    (00 = Master)
                        'SEQNO_HIGH'        : 3,    # - numero del messaggio utile per associare la risposta
                        'SEQNO_LOW'         : 4,    # -
                        'CMD_RCODE'         : 5,    # rCode di ritorno per il comando eseguito (in TX è ignorato)
                        'COMMAND'           : 6,    # comando da eseguire
                        'SUBCOMMAND'        : 7,    # eventuale dettaglio per il comando
                        'COMMAND_DATA'      : 8,    # TX - dati necessari al comando per la sua corretta esecuzione/RX - dati di risposta
                    };



LnRs485_writePin   = {  'COMANDO'           : 5,   # READ 1 | WRITE-2
                        'PIN_NO'            : 6,   # numero del pin su cui operare
                        'PIN_VALUE'         : 6,   # 0, 1
                    };



# ln485Map = LnClass()
# ln485Map.DATALEN = 0
# ln485Map.COMMAND_DATA = 8


class LnRs485_Instrument():
    def __init__(self, port, mode='ascii', baudrate=9600, logger=None):

        if port not in _SERIALPORTS or not _SERIALPORTS[port]:
            try:
                self.serial = _SERIALPORTS[port] = serial.Serial(
                                                    port     = port,
                                                    baudrate = baudrate,
                                                    parity   = serial.PARITY_NONE,
                                                    bytesize = serial.EIGHTBITS,
                                                    stopbits = serial.STOPBITS_ONE,
                                                    rtscts   = False,
                                                    xonxoff  = False,
                                                    dsrdtr   = False,
                                                    timeout  = read_TIMEOUT)




            except (Exception) as why:
                print ('ERROR:  ', str(why))
                sys.exit()

        else:
            self.serial = _SERIALPORTS[port]
            if self.serial.port is None:
                self.serial.open()
                # - chissà se sono importanti....
                self.serial.reset_input_buffer()        # clear input  buffer
                self.serial.reset_output_buffer()       # clear output buffer

        if logger:
            self._setLogger = logger
        else:
            self._setLogger = self._internaLogger

        self._validBytes=bytearray([int(i, 16) for i in LnRs485_validBytesHex]) # creiamo un array di integer

        self.mode = mode
        """Slave mode (str), can be MODE_RTU or MODE_ASCII.  Most often set by the constructor (see the class documentation).

        New in version 0.6.
        """

        # ----   LnRs485_payLoadMap
        # self._payLoadMap = LnClass()
        # self._payLoadMap.SENDER_ADDR       = 0
        # self._payLoadMap.DESTINATION_ADDR  = 1
        # self._payLoadMap.SEQNO_HIGH        = 2
        # self._payLoadMap.SEQNO_LOW         = 3
        # self._payLoadMap.PAYLOAD           = 4
        self._printableChars                = list(range(31,126))

        self.debug = True
        """Set this to :const:'True' to print the communication details. Defaults to :const:'False'."""

        self.sendCounter = 0
        """Set this to :const:'True' to print the communication details. Defaults to :const:'False'."""

        self.STX = int('0x02', 16) # integer
        self.ETX = int('0x03', 16) # integer
        # self.CRC = True

        self.close_port_after_each_call = False
        """If this is :const:'True', the serial port will be closed after each call. """
        if  self.close_port_after_each_call: self.serial.close()

        self.precalculate_read_size = True
        """If this is :const:'False', the serial port reads until timeout
        instead of just reading a specific number of bytes. Defaults to :const:'True'.

        New in version 0.5.
        """


    def ClosePortAfterEachCall(self, openPortAEC):
        logger = self._setLogger(package=__name__)
        self.close_port_after_each_call = openPortAEC

        if openPortAEC:
            if self.serial.isOpen():
                logger.info('closing port...')
                self.serial.close()
        else:
            if not self.serial.isOpen():
                logger.info('opening port...')
                self.serial.open()



    def _internaLogger(self, package=None):
        ##############################################################################
        # - classe che mi permette di lavorare nel caso il logger non sia richiesto
        ##############################################################################
        class nullLogger():
                def __init__(self, package=None, stackNum=1):
                    pass
                def info(self, data):
                    self._print(data)
                def debug(self, data):
                    self._print(data)
                def error(self, data):  pass
                def warning(self, data):  pass

                def _print(self, data):
                    pass
                def _print_(self, data):
                    caller = inspect.stack()[3]
                    dummy, programFile, lineNumber, funcName, lineCode, rest = caller
                    if funcName == '<module>': funcName = '__main__'
                    str = "[{FUNC:<20}:{LINENO}] - {DATA}".format(FUNC=funcName, LINENO=lineNumber, DATA=data)
                    print (str)

        return nullLogger()


    def __repr__(self):
        """String representation of the :class:'.Instrument' object."""
            # address                    = {ADDRESS},
        return """{MOD}.{CLASS}
            <class-id                  = 0x{ID:x},
            mode                       = {MODE},
            close_port_after_each_call = {CPAEC},
            precalculate_read_size     = {PRS},
            debug                      = {DEBUG},
            STX                        = 0x{STX:02x},
            ETX                        = 0x{ETX:02x},
            serial-id                  = {SERIAL},>
                """.format(
                        MOD=self.__module__,
                        CLASS=self.__class__.__name__,
                        ID=id(self),
                        MODE=self.mode,
                        CPAEC=self.close_port_after_each_call,
                        PRS=self.precalculate_read_size,
                        DEBUG=self.debug,
                        SERIAL=self.serial,
                        STX=self.STX,
                        ETX=self.ETX
            )
                        # ADDRESS=self.address,

    def _getCRC8(self, byteArray_data):
        logger = self._setLogger(package=__name__)
        crcValue = 0
        for byte in byteArray_data:
            # if isinstance(byte, str): byte = ord(byte)            # onverte nel valore ascii
            logger.debug('byte: int:{0} hex: {0:02x} - crcValue int:{1} hex: {1:02x}'.format(byte, crcValue))
            b2 = byte
            if (byte < 0):
                b2 = byte + 256
            for i in range(8):
                odd = ((b2^crcValue) & 1) == 1
                crcValue >>= 1
                b2 >>= 1
                if (odd):
                    crcValue ^= 0x8C # this means crc ^= 140

        return crcValue


    # ---------------------------------------------
    # - aaaa bbbb
    # -     byte1 = aaaa !aaaa
    # -     byte2 = bbbb !bbbb
    # -     byte = byte1_HNibble * 16 + byte2_HNibble
    # ---------------------------------------------
    def _splitComplementedByte(self, byte):
        logger = self._setLogger(package=__name__)
        logger.debug ("byte to be converted: {0} - type: {1}".format(byte, type(byte)))
        # if isinstance(byte, str): byte = ord(byte)            # onverte nel valore ascii

        # print ('....', type(byte), byte)
        # logger.debug ("converting: x{0:02X}".format(byte))

            # first nibble
        c = byte >> 4;
        byteValue = (c << 4) | (c ^ 0x0F)
        highNibble = byteValue
        logger.debug  ("    x{0:02X}".format( highNibble))

            # second nibble
        c = byte & 0x0F;
        byteValue = (c << 4) | (c ^ 0x0F)
        lowNibble = byteValue
        logger.debug  ("    x{0:02X}".format(lowNibble))


            # second two bytes
        return highNibble, lowNibble




    # ---------------------------------------------
    # -     byte1 = aaaa !aaaa
    # -     byte2 = bbbb !bbbb
    # -     byte = byte1_HNibble * 16 + byte2_HNibble
    # ---------------------------------------------
    def _combineComplementedByte(self, byte1, byte2):
        logger = self._setLogger(package=__name__)
        # if isinstance(byte1, str): byte1 = ord(byte1)            # onverte nel valore ascii
        # if isinstance(byte2, str): byte2 = ord(byte2)            # onverte nel valore ascii

        logger.debug("complementedData: x{0:02X} + x{1:02X}".format(byte1, byte2))

            # - check first byte
        byte1_HighNibble = (byte1 >> 4) & 0x0F
        byte1_LowNibble = ~byte1 & 0x0F
        if byte1_LowNibble != byte1_HighNibble:
            logger.error("byte1 nibbles corrupted: x{0:02X} + x{1:02X}".format(byte1_LowNibble, byte1_HighNibble))
            return None

            # - check second byte
        byte2_HighNibble = (byte2 >> 4) & 0x0F
        byte2_LowNibble = ~byte2 & 0x0F
        if byte2_LowNibble != byte2_HighNibble:
            logger.error("byte2 nibbles corrupted: x{0:02X} + x{1:02X}".format(byte2_LowNibble, byte2_HighNibble))
            return None

            # re-build real byte
        realByte = byte1_HighNibble*16 + byte2_HighNibble
        logger.debug("  resulting data BYTE: x{0:02X} char:{1}".format(realByte, chr(realByte)))

        return realByte


    #######################################################################
    # - Lettura dati fino a:
    # -     SOD = StartOfData  carattere che identifica l'inizio dei dati.
    # -             quelli arrivati prima saranno ignorati
    # -     EOD = None ... avanti fino al primo null char
    # -     EOD = xxx ... fino al char xxx
    # - Ritorna una bytearray di integer
    #######################################################################
    def _readSerialBuffer(self, SOD=[], EOD=[], timeoutValue=5000, fDEBUG=False):
        logger = self._setLogger(package=__name__)

        if self.close_port_after_each_call:
            logger.debug('opening port...')
            self.serial.open()

        if isinstance(SOD, int): SOD=[SOD]
        if isinstance(EOD, int): EOD=[EOD]

        RAW = True if (not SOD and not EOD) else False

            # -------------------
            # - solo per DEBUG
            # -------------------
        sodString=''
        for val in SOD: sodString += " x{0:02x}".format(val)
        logger.debug( "SOD:     {0}".format(sodString))

        eodString=''
        for val in EOD: eodString += " x{0:02x}".format(val)
        logger.debug( "EOD:     {0}".format(eodString))

        logger.debug( "RAW:     {}".format(RAW))
        logger.debug( "TIMEOUT: {}".format(timeoutValue))
        logger.debug( "reading buffer")


        # facciamo partire il timer
        startRun = time.time()
        elapsed = 0

        buffer = bytearray()
        while elapsed < timeoutValue:
            # time.sleep(.01)
            elapsed = time.time()-startRun
            logger.debug( "elapsed {}/{}".format(elapsed, timeoutValue))
                # - in attesa di un byte
            ch = self.serial.read(1)       # ch e' un type->bytes
            chInt = int.from_bytes(ch, 'little')
            if ch == b'':
                if RAW:
                    if len(buffer) > 0:
                        logger.debug( "Received byte: {0:02x} - breaking".format(chInt) )
                        break
                    else:
                        logger.debug( "Received byte: {0:02x} - skipping".format(chInt) )
                        continue
                else:
                    if len(buffer) > 0:
                        logger.debug( "Received byte: {0:02x} - breaking".format(chInt) )
                        break
                    else:
                        logger.debug( "Received byte: {0:02x} - skipping".format(chInt) )
                        continue

            logger.debug( "Received byte: {0:02x}".format(chInt) )
                # Start of Data
            if SOD and chInt in SOD:
                logger.debug( "found SOD")
                buffer = bytearray()    # reinizializza il buffer
                buffer.append(chInt)

                # se siamo in cerca di EOD
            elif EOD:
                logger.debug( "inside EOD")
                buffer.append(chInt)
                if chInt in EOD:
                    logger.debug( "found SOD")
                    break

                # - andiamo liberi senza delimiters
            else:
                logger.debug( "inside RAW")
                buffer.append(chInt)





        # print (elapsed, TIMEOUT)
        if self.close_port_after_each_call:
            logger.debug('closing port...')
            self.serial.close()

        return buffer



    #######################################################################
    # - Lettura dati fino a:
    # -     SOD = StartOfData  carattere che identifica l'inizio dei dati.
    # -             quelli arrivati prima saranno ignorati
    # -     EOD = None ... avanti fino al primo null char
    # -     EOD = xxx ... fino al char xxx
    # - Ritorna una bytearray di integer
    #######################################################################
    def _readSerialBuffer_ERROR(self, SOD=[], EOD=[], timeoutValue=5000, fDEBUG=False):
        logger = self._setLogger(package=__name__)

        if self.close_port_after_each_call:
            logger.debug('opening port...')
            self.serial.open()

            # creiamo LIST nel caso sia un intero
        if isinstance(SOD, int): SOD=[SOD]
        if isinstance(EOD, int): EOD=[EOD]

            # se non abbiamo SOD nè EOD
        RAW = True if (not SOD and not EOD) else False

            # -------------------
            # - solo per DEBUG
            # -------------------
        sodString=''
        for val in SOD: sodString += " x{0:02x}".format(val)
        logger.debug( "SOD:          {0}".format(sodString))

        eodString=''
        for val in EOD: eodString += " x{0:02x}".format(val)
        logger.debug( "EOD:          {0}".format(eodString))
        logger.debug( "RAW:          {}".format(RAW))
        logger.debug( "TIMEOUT (ms): {}".format(timeoutValue))
        logger.debug( "reading buffer")


        # facciamo partire il timer
        startRun = time.time()*1000
        elapsed = 0
        TIMEOUT = True

        buffer = bytearray()
        while elapsed < timeoutValue:
            # time.sleep(.01)
            elapsed = int((time.time()*1000)-startRun)
            if elapsed%100 == 0:
                logger.debug( "elapsed {0}/{1}".format(elapsed, timeoutValue))

                # ---------------------------
                # - in attesa di un byte
                # ---------------------------
            ch = self.serial.read(1)       # ch e' un type->bytes
            chInt = int.from_bytes(ch, 'little')
            if ch == b'':
                if RAW:
                    if len(buffer) > 0:
                        logger.debug( "Received byte: {0:02x} - breaking".format(chInt) )
                        break
                    else:
                        logger.debug( "Received byte: {0:02x} - skipping".format(chInt) )
                        continue
                else:
                    if len(buffer) > 0:
                        logger.debug( "Received byte: {0:02x} - breaking".format(chInt) )
                        break
                    else:
                        logger.debug( "Received byte: {0:02x} - skipping".format(chInt) )
                        continue

            logger.debug( "Received byte: {0:02x}".format(chInt) )
                # Start of Data
            if SOD and chInt in SOD:
                logger.debug( "found SOD")
                buffer = bytearray()    # reinizializza il buffer
                buffer.append(chInt)

                # se siamo in cerca di EOD
            elif EOD:
                logger.debug( "inside EOD")
                buffer.append(chInt)
                if chInt in EOD:
                    logger.debug( "found SOD")
                    break

                # - andiamo liberi senza delimiters
            else:
                logger.debug( "inside RAW")
                buffer.append(chInt)





        # print (elapsed, timeoutValue)
        if self.close_port_after_each_call:
            logger.debug('closing port...')
            self.serial.close()

        return buffer


    #######################################################################
    # - by Loreto
    # - EOD = int('0x0A', 16) # integer
    # - EOD = None   ... legge fino al primo byte null
    #######################################################################

    def readRawData(self, SOD=[], EOD=[], hex=False, text=False, char=False, timeoutValue=1000):
        logger = self._setLogger(package=__name__)

        bufferData = self._readSerialBuffer(SOD=[], EOD=EOD, timeoutValue=timeoutValue)

        if bufferData:
            # validChars = list(range(31,126))
            validChars = self._printableChars
            validChars.append(10) # aggiungiamo il newline in modo che venga displayato

            if isinstance(bufferData, bytes):
                bufferData = bufferData.decode('utf-8')


            lineToPrint = []
            for i in bufferData:
                if i in validChars:                    # Handle only printable ASCII
                    lineToPrint.append(chr(i))
                else:
                    lineToPrint.append(" ")


            if hex:
                hexData         = ' '.join('{0:02x}'.format(x) for x in bufferData)
                print ('readRawDataLib: {DESCR:^10}:  {DATA}'.format(DESCR="raw", DATA=hexData))

            if char:
                print ('readRawDataLib: {DESCR:^10}:  {DATA}'.format(DESCR="chr", DATA='  '.join(lineToPrint)))

            if text:
                print ('readRawDataLib: {DESCR:^10}:  {DATA}'.format(DESCR="text", DATA=''.join(lineToPrint)))


        return bufferData








    #######################################################################
    # - by Loreto
    # - Lettura dati bsato sul protocollo:
    # -     RS485 protocol library by Nick Gammon
    # - STX - data - CRC - ETX
    # - A parte STX e ETX tutti gli altri byte sono inviati come due nibble
    # -  byte complemented (incluso il CRC)
    # -  only values sent would be (in hex):
    # -    0F, 1E, 2D, 3C, 4B, 5A, 69, 78, 87, 96, A5, B4, C3, D2, E1, F0
    # -  Con il fatto che solo i byte di sopra possono essere inviati,
    # -  il controllo su di essi forse fa venire meno il CRC e quindi
    # -  l'ho inserito come flag
    #
    # - credo che questa funzione venga chiamata non direttamente da me
    # - in quanto ho notato che non prende ulteriori parametri.
    #######################################################################
    def readData(self, timeoutValue=1000, fDEBUG=False):
        logger = self._setLogger(package=__name__)

        bufferData = self._readSerialBuffer(SOD=self.STX, EOD=self.ETX, timeoutValue=timeoutValue, fDEBUG=fDEBUG)
        hexData    = ' '.join('{0:02X}'.format(x) for x in bufferData)
        msg = '{TITLE:<15}: ({LEN}) {DATA}'.format(TITLE='readDataLib: full data', LEN=len(bufferData), DATA=hexData)
        # print(msg)
        logger.debug(msg)

        # if not bufferData:
        #     msg = 'ERROR: no valid data found'
        #     print(msg)
        #     logger.error(msg)
        #     return bytearray(), bufferData

            # Prendiamo i dati fissi
        if not bufferData[0] == self.STX:
            msg = 'ERROR: STX missed'
            print(msg)
            logger.error(msg)
            return bytearray(), bufferData

        if not bufferData[-1] == self.ETX:
            msg = 'ERROR: ETX missed'
            print(msg)
            logger.error(msg)
            return bytearray(), bufferData


            # ---------------------------------------------
            # - ricostruzione dei bytes originari
            # - byte = byte1_HighNibble*16 + byte2_HighNibble
            # - si potrebbe usare la funzione _combineComplementedByte
            # -    che provvede anche a fare la verifica che
            # -    il secondo nibble sia il complemento del primo ma
            # -    avendo verificato che il byte si trova nei self._validBytes
            # ---------------------------------------------

            # il trick che segue ci permette di prelevare due bytes alla volta
        rcvData = bytearray()
        payLoadNibbled  = bufferData[1:-1] # skip STX and ETX - include nibbled_data+nibbled_CRC
        xy = iter(payLoadNibbled)
        for byte1, byte2 in zip(xy, xy):
                # re-build real byte
            if byte1 in self._validBytes and byte2 in self._validBytes:
                byte1_HighNibble = (byte1 >> 4) & 0x0F
                byte2_HighNibble = (byte2 >> 4) & 0x0F
                realByte = byte1_HighNibble*16 + byte2_HighNibble


            else:
                msg = 'ERROR: some byte corrupted byte1:{0:02x} byte2:{1:02x}'.format(byte1, byte2)
                print(msg)
                logger.error(msg)
                return bytearray(), bufferData

            rcvData.append(realByte)


            # -----------------------------------------------------------------------
            # - Una volta ricostruidi i bytes origilali,
            # - calcoliamo il CRC sui dati (ovviamento escluso il byte di CRC stesso)
            # -----------------------------------------------------------------------
        CRC_calculated  = self._getCRC8(rcvData[:-1])
            # ---------------------------------
            # - check CRC (drop STX and ETX)
            # ---------------------------------
        CRC_received    = rcvData[-1]
        rcvData         = rcvData[:-1]

        logger.debug("    CRC received  : x{0:02X}".format(CRC_received))
        logger.debug("    CRC calculated: x{0:02X}".format(CRC_calculated))
        xMitCode = CRC_received - CRC_calculated

        if xMitCode:
            logger.error('Il valore di CRC non coincide')
            print ('ERROR: Il valore di CRC non coincide')
            print ()
            print ("    CRC received  : x{0:02X}".format(CRC_received))
            print ("    CRC calculated: x{0:02X}".format(CRC_calculated))
            print ()
            return bytearray(), bufferData

        if fDEBUG and len(rcvData) > 0:
                # sommario
            print ()
            print ('[Pi-RX] - ', end="")
            print (' [{0:02X}/{0:03}]'.format(rcvData[SENDER_ADDR]), end="")
            print (' --> [{0:02X}/{0:03}]'.format(rcvData[DESTINATION_ADDR]), end="")
            print (' - SeqNo: {0:05}'.format(rcvData[SEQNO_HIGH]*256+rcvData[SEQNO_LOW]), end="")
            print (' - [rcvdCode: {0:03}]'.format(xMitCode))
            print ()


                # hezadecimal
            print ('    full data - len: [{0:03}] - '.format(len(rcvData)), end="")
            for byte in rcvData: print ('{0:02X} '.format(byte), end="")
            print ()

                # hezadecimal solo la parte comando
            userData = rcvData[COMMAND_DATA:]
            print ('    user data - len: [{0:03}] - '.format(len(userData)), end="")
            print ('   '*COMMAND_DATA, end="")
            for byte in userData: print ('{0:02X} '.format(byte), end="")

                # ascii solo la parte comando
            print ('')
            print ('    user data - len: [{0:03}] - '.format(len(userData)), end="")
            print ('   '*COMMAND_DATA, end="")
            print ('[', end="")
            for byte in userData:
                if byte in self._printableChars:   # Handle only printable ASCII
                    print(chr(byte), end="")
                else:
                    print(' ', end="")
            print (']')
            print ('')
            print ('    CRC Rec/Cal 0x : {0:02X} {1:02X}'.format(CRC_received, CRC_calculated))
            print ('    SEQNO       0x : {0:02X} {1:02X}'.format(rcvData[SEQNO_HIGH], rcvData[SEQNO_LOW]))
            print ('    CMD_RCode   0x : {0:02X}'.format(rcvData[CMD_RCODE]))
            print ('    CMD/subCMD  0x : {0:02X} {1:02X}'.format(rcvData[COMMAND], rcvData[SUBCOMMAND]))


        return rcvData, bufferData



    def _getSendCounter(self):
        self.sendCounter += 1
        yy = self.sendCounter.to_bytes(2, byteorder='big')
        return yy

    #######################################################################
    # - sendDataSDD - con parametri di input diversi
    # -    richiama comunque sendData
    #######################################################################
    def sendDataSDD(self, sourceAddress, destAddress, dataStr, fDEBUG=False):
        ''' formato esplicito dei parametri '''
        dataToSend = bytearray()
        dataToSend.append(sourceAddress)
        dataToSend.append(destAddress)

        yy = self._getSendCounter()
        dataToSend.append(yy[0])  # high byte
        dataToSend.append(yy[1])  # Low byte

        for x in dataStr:
            dataToSend.append(ord(x))

        dataSent = self.sendData(dataToSend, fDEBUG=fDEBUG)
        return dataSent


    #######################################################################
    # - sendDataCMD - con parametri di input diversi
    # -    richiama comunque sendData
    # -    ordine:
    # -      SENDER_ADDR       = 0    # - Dest Address      (FF = Broadcast)
    # -      DESTINATION_ADDR  = 1    # - source Address    (00 = Master)
    # -      SEQNO_HIGH        = 2    # - numero del messaggio utile per associare la risposta
    # -      SEQNO_LOW         = 3    # -
    # -      CMD_RCODE         = 4    # rCode di ritorno per il comando eseguito (in TX è ignorato)
    # -      COMMAND           = 5    # comando da eseguire
    # -      SUBCOMMAND        = 6    # eventuale dettaglio per il comando
    # -      COMMAND_DATA      = 7    # TX - dati necessari al comando per la sua corretta esecuzione/RX - dati di risposta
    #######################################################################
    def sendDataCMD(self, CMD, fDEBUG=False):
        ''' formato CLASS dei parametri '''
        dataToSend = bytearray()
        dataToSend.append(CMD.sourceAddr)
        dataToSend.append(CMD.destAddr)

        yy = self._getSendCounter()
        dataToSend.append(yy[0])  # high byte
        dataToSend.append(yy[1])  # Low byte

        dataToSend.append(CMD.xmitRcode)  # 0 per una TX
        dataToSend.append(CMD.command)
        dataToSend.append(CMD.subCommand)

        for x in CMD.dataStr:
            dataToSend.append(ord(x))

        dataSent = self.sendData(dataToSend, fDEBUG=fDEBUG)
        return dataSent


    #######################################################################
    # - by Loreto
    # - Scrittura dati basato sul protocollo:
    # -     RS485 protocol library by Nick Gammon
    # - STX - data - CRC - ETX
    # - A parte STX e ETX tutti gli altri byte sono inviati come due nibble
    # -  byte complemented (incluso il CRC)
    # -  only values sent would be (in hex):
    # -    0F, 1E, 2D, 3C, 4B, 5A, 69, 78, 87, 96, A5, B4, C3, D2, E1, F0
    #######################################################################
    def sendData(self, txData, fDEBUG=False):
        ''' formato in bytearray dei parametri '''
        logger = self._setLogger(package=__name__)
        if fDEBUG:
            # print()
            # seqNO = data[SEQNO_LOW]+data[SEQNO_HIGH]*256
            # print ('sendDataLib: dataLen=', len(data))
            # print ('    source:{sADDR:03}  dest:{dADDR:03} CMD:{CMD:03} seqNo:{SEQ:05}'.format(sADDR=data[SENDER_ADDR], dADDR=data[DESTINATION_ADDR], SEQ=seqNO, CMD=data[COMMAND]))
            # print ('    {DESCR:<10}: {DATA}'.format(DESCR="payload", DATA=' '.join('{0:02x}'.format(x) for x in data)))

            '''

            print ('sendDataLib:')
            print ('    seqNo    : {0:05}'.format(payLoad[SEQNO_HIGH]*256+payLoad[SEQNO_LOW]))
            print ('    xMitCode : {0:03} (0 for TX)'.format(0))
            print ()

            print ('    full data - len: [{0:03}] - '.format(len(payLoad)), end="")
            for byte in payLoad: print ('{0:02X} '.format(byte), end="")
            print ()

            userData = payLoad[USER_DATA:]
            print ('    user data - len: [{0:03}] - '.format(len(userData)), end="")
            print ('   '*USER_DATA, end="")
            for byte in userData: print ('{0:02X} '.format(byte), end="")

            print ('')
            print ('    user data - len: [{0:03}] - '.format(len(userData)), end="")
            print ('   '*USER_DATA, end="")
            print ('[', end="")
            for byte in userData:
                if byte in self._printableChars:   # Handle only printable ASCII
                    print(chr(byte), end="")
                else:
                    print(' ', end="")
            print (']')
            print ('')
            print ('    SourceAddr 0x : {0:02X}'.format(payLoad[SENDER_ADDR]))
            print ('    DestAddr   0x : {0:02X}'.format(payLoad[DESTINATION_ADDR]))
            print ('    SEQNO      0x : {0:02X} {1:02X}'.format(payLoad[SEQNO_HIGH], payLoad[SEQNO_LOW]))
            print ('    Command    0x : {0:02X}'.format(payLoad[COMMAND]))
            print ('    userRCode  0x : {0:02X}'.format(payLoad[USER_RCODE]))

            '''


            # - preparaiamo il bytearray con i dati da inviare
        dataToSend=bytearray()

            # - STX nell'array
        dataToSend.append(self.STX)

            # - Data nell'array
        for thisByte in txData:
            byte1, byte2 = self._splitComplementedByte(thisByte)
            dataToSend.append(byte1)
            dataToSend.append(byte2)

            # - CRC nell'array
        if self.CRC:
            CRC_value    = self._getCRC8(txData)
            byte1, byte2 = self._splitComplementedByte(CRC_value)
            dataToSend.append(byte1)
            dataToSend.append(byte2)

            # - ETX
        dataToSend.append(self.ETX)

        if self.close_port_after_each_call:
            self.serial.open()

            # INVIO dati
        self.serial.write(dataToSend)


        if self.close_port_after_each_call:
            self.serial.close()



        if fDEBUG:
            print ('[Pi-TX] - ', end="")
            print (' [{0:02X}/{0:03}]'.format(txData[SENDER_ADDR]), end="")
            print (' --> [{0:02X}/{0:03}]'.format(txData[DESTINATION_ADDR]), end="")
            print (' - SeqNo: {0:05}'.format(txData[SEQNO_HIGH]*256+txData[SEQNO_LOW]), end="")
            print ()
            print ()



            print ('    full data - len: [{0:03}] - '.format(len(txData)), end="")
            for byte in txData: print ('{0:02X} '.format(byte), end="")
            print ()

            userData = txData[COMMAND_DATA:]
            print ('    user data - len: [{0:03}] - '.format(len(userData)), end="")
            print ('   '*COMMAND_DATA, end="")
            for byte in userData: print ('{0:02X} '.format(byte), end="")

            print ('')
            print ('    user data - len: [{0:03}] - '.format(len(userData)), end="")
            print ('   '*COMMAND_DATA, end="")
            print ('[', end="")
            for byte in userData:
                if byte in self._printableChars:   # Handle only printable ASCII
                    print(chr(byte), end="")
                else:
                    print(' ', end="")
            print (']')
            print ('')
            print ('    xMitted CRC 0x : {0:02X}'.format(CRC_value))
            print ('    SEQNO       0x : {0:02X} {1:02X}'.format(txData[SEQNO_HIGH], txData[SEQNO_LOW]))
            print ('    CMD_RCode   0x : {0:02X}'.format(txData[CMD_RCODE]))
            print ('    CMD/subCMD  0x : {0:02X} {1:02X}'.format(txData[COMMAND], txData[SUBCOMMAND]))

            print ('    {DESCR:<10}: {DATA}'.format(DESCR="raw data", DATA=' '.join('{0:02x}'.format(x) for x in dataToSend)))

        return dataToSend




class LnClass(): pass
################################################################################
# - M A I N
# - Prevede:
# -  2 - Controllo parametri di input
# -  5 - Chiamata al programma principale del progetto
################################################################################
if __name__ == '__main__':
    import time
    Sintax = """
        Immettere:
            action.usbPortNO.EoD [EOD=endOfData, default=10='\n']

        es.:
            monitor.0   : per monitorare la porta /dev/ttyUSB0
            send.0      : per inviare messaggi sulla porta /dev/ttyUSB0
    """



    if len(sys.argv) > 1:
        token = sys.argv[1].split('.')

        if len(token) == 2:
            action, portNO = sys.argv[1].split('.')
            EOD = b'\x03'

        elif len(token) == 3:
            action, portNO, EOD = sys.argv[1].split('.')
            iEOD = int(EOD)
            EOD = bytes([iEOD])

        else:
            print (Sintax)
            sys.exit()
    else:
        print (Sintax)
        sys.exit()




    LnRs485                = LnRs485_Instrument   # short pointer alla classe
    rs485                  = LnClass()
    rs485.MASTER_ADDRESS   = 0
    rs485.bSTX             = b'\x02'
    rs485.bETX             = b'\x03'
    rs485.usbDevPath       = '/dev/ttyUSB{0}'.format(portNO)
    # rs485.baudRate         = 9600
    rs485.mode             = 'ascii'

    print('     .....action:', action)
    print('     .....portNO:', portNO)
    print('     .....EOD:   ', EOD)


        # ===================================================
        # = RS-485 port monitor
        # ===================================================
    if action == 'monitor':
        print('Monitoring port: {0}'.format(rs485.usbDevPath))
            # ------------------------------
            # - Inizializzazione
            # ------------------------------
        try:
            address = 5
            print('setting port {0} to address {1}'.format(rs485.usbDevPath, address))
            monPort = LnRs485(rs485.usbDevPath, address, rs485.mode, logger=None)  # port name, slave address (in decimal)
            monPort.CRC = True

            print ('... press ctrl-c to stop the process.')
            while True:
                payLoad, rawData = monPort.readData()
                print ('rawData (Hex):  {0}'.format(' '.join('{0:02x}'.format(x) for x in rawData)))
                if payLoad:
                    print ('payLoad (Hex):      {0}'.format(' '.join('{0:02x}'.format(x) for x in payLoad)))
                    print ('payLoad (chr):      {0}'.format(' '.join('{0:>2}'.format(chr(x)) for x in payLoad)))
                else:
                    print ('payLoad ERROR....')
                print()


        except (KeyboardInterrupt) as key:
            print ("Keybord interrupt has been pressed")
            sys.exit()


    elif action == 'send':
        print('Sending data to port: {0}'.format(rs485.usbDevPath))
            # ------------------------------
            # - Inizializzazione
            # ------------------------------
        try:
            address = 5
            print('setting port {0} to address {1}'.format(rs485.usbDevPath, address))
            wrPort = LnRs485(rs485.usbDevPath, address, rs485.mode, logger=None)  # port name, slave address (in decimal)
            # - setting di alcuni parametri delle funzioni
            # - me li ritrovo come self.PARAM
            wrport.CRC       = False
            print ('... press ctrl-c to stop the process.')
            index = 0
            basedata = 'Loreto.'
            while True:
                index += 1
                dataToSend  = '[{0}.{1:04}]'.format(basedata, index)
                line        = '[{0}:{1:04}] - {2}'.format(rs485.usbDevPath, index, dataToSend)
                print (line)
                dataSent = wrPort.sendData(dataToSend, CRC=True)
                print ('sent (Hex): {0}'.format(' '.join('{0:02x}'.format(x) for x in dataSent)))
                print()
                time.sleep(5)


        except (KeyboardInterrupt) as key:
            print ("Keybord interrupt has been pressed")
            sys.exit()


    else:
        print(gv.INPUT_PARAM.actionCommand, 'not available')







''' OLD functions
    #######################################################################
    # - Lettura dati da StartOfData fino a EndOfData
    # - Ritorna una bytearray di integer
    #######################################################################
    def _read_SOD_EOD_Buffer(self, SOD=None, EOD=None, TIMEOUT=1000):
        logger = self._setLogger(package=__name__)

        if self.close_port_after_each_call:
            logger.debug('opening port...')
            self.serial.open()

        if not SOD: SOD = self.STX
        if not EOD: EOD = self.ETX
        buffer = bytearray()

        logger.debug( "reading buffer")

            # facciamo partire il timer
        startRun = time.time()
        elapsed = 0

        while TIMEOUT:
            TIMEOUT -= (time.time()-startRun)
            ch = self.serial.read(1)       # ch e' un bytes
            if ch==b'': continue
            chInt = int.from_bytes(ch, 'little')
            buffer.append(chInt)
            break

        logger.debug( "Received: SOD {0:02x}, waiting for EOD {1:02x}".format(SOD, EOD))

        while TIMEOUT:
            TIMEOUT -= (time.time()-startRun)
            ch = self.serial.read(1)       # ch e' un bytes
            if ch == b'': continue
            chInt = int.from_bytes(ch, 'little')
            buffer.append(chInt)
            logger.debug( "Received: byte hex: {0:02x}... waiting for {1:02x}".format(chInt, EOD) )
            if chInt==EOD: break


        if self.close_port_after_each_call:
            logger.debug('closing port...')
            self.serial.close()

        return buffer





    #######################################################################
    # - Lettura dati da StartOfData fino a EndOfData
    # - Ritorna una bytearray di integer
    #######################################################################
    def _readBuffer_OK_TO_BE_DELETED(self):
        logger = self._setLogger(package=__name__)

        if self.close_port_after_each_call:
            logger.debug('opening port...')
            self.serial.open()

        buffer = bytearray()

        logger.debug( "reading buffer")
        chInt=-1
        while chInt != self.STX:
            ch = self.serial.read(1)       # ch e' un bytes
            if ch == b'': continue
            chInt = int.from_bytes(ch, 'little')


        buffer.append(chInt)
        logger.debug( "Received: STX")
        chInt=-1
        while chInt != self.ETX:
            ch = self.serial.read(1)       # ch e' un bytes
            if ch == b'': continue
            chInt = int.from_bytes(ch, 'little')
            buffer.append(chInt)
            logger.debug( "Received: byte hex: {0:02x}... waiting for {1:02x}".format(chInt, self.ETX) )

        if self.close_port_after_each_call:
            logger.debug('closing port...')
            self.serial.close()

        return buffer

    #######################################################################
    # - Lettura dati fino a:
    # -     EOD = None ... fino al primo NULL byte
    # -     EOD = xxx ... fino al char xxx
    # - Ritorna una bytearray di integer
    #######################################################################
    def _readRawBuffer_OK_TO_BE_DELETED(self, EOD=None):
        logger = self._setLogger(package=__name__)

        if self.close_port_after_each_call:
            logger.debug('opening port...')
            self.serial.open()

        # print ('..............', EOD)
        buffer = bytearray()

        if EOD:
            chInt=-1
            while chInt != EOD:
                ch = self.serial.read(1)       # ch e' un bytes
                if ch == b'': continue
                chInt = int.from_bytes(ch, 'little')
                buffer.append(chInt)
                logger.debug( "Received byte: {0:02x}... waiting for {1:02x}".format(chInt, EOD) )

        else:
            while True:
                ch = self.serial.read(1)       # ch e' un bytes
                if ch == b'': break
                chInt = int.from_bytes(ch, 'little')
                buffer.append(chInt)
                logger.debug( "Received byte: {0:02x}... waiting for NULL".format(chInt) )

        if self.close_port_after_each_call:
            logger.debug('closing port...')
            self.serial.close()

        return buffer


'''