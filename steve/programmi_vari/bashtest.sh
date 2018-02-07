#!/bin/bash
#  Cleanup, versione 3

#  Attenzione:
#  -----------
#  In questo script sono presenti alcune funzionalità che verranno
#+ spiegate più avanti.
#  Quando avrete ultimato la prima metà del libro,
#+ forse non vi apparirà più così misterioso.



DIR_LOG=/var/log
ROOT_UID=0     # Solo gli utenti con $UID 0 hanno i privilegi di root.
LINEE=500       # Numero prestabilito di righe salvate.
E_XCD=66       # Riesco a cambiare directory?
E_NONROOT=67   # Codice di exit non-root.

#-------------------------------------------------------------------------------
# Da eseguire come root, naturalmente.
if [ "$UID" -ne "$ROOT_UID" ]   #se root UID=0 altrimenti e' 1000
then
echo " il valore $UID e' = " "$UID" #trovato valore 1000
  echo "Devi essere root per eseguire questo script."
  exit $E_NONROOT
fi
echo " il valore $UID e' = " "$UID"
#-------------------------------------------------------------------------------
if [ -n "$1" ]
# Verifica se è presente un'opzione da riga di comando (non-vuota).
# in pratica se eseguo sudo bash bashtest.sh 501 prendera' il valore 501
then
  linee=$1
else
  linee=$LINEE # Valore preimpostato, se non specificato da riga di comando.
fi

echo "linee= " "$linee"
#-------------------------------------------------------------------------------

cd $DIR_LOG
echo "1 pwd = " `pwd`
if [ `pwd` != "$DIR_LOG" ]  # o   if [ "$PWD" != "$DIR_LOG" ]
                            # Non siamo in /var/log?
then
  echo "Non riesco a cambiare in $DIR_LOG."
  exit $E_XCD
fi  #  Doppia verifica per vedere se ci troviamo nella directory corretta,
    #+ prima di cancellare il file di log.
echo "siamo nella directory: " "$DIR_LOG"
echo "2 pwd = " `pwd`

# ancora più efficiente:
#
# cd /var/log || {
#   echo "Non riesco a spostarmi nella directory stabilita." >&2
#   exit $E_XCD;
# }

tail -n $linee messages > mesgSS.temp # Salva l'ultima sezione del file di
                                    # log messages.
# tail -n (display delle ultime $linee) poi le mette nel file mesgSS.temp
exit
mv mesgSS.temp messages               # Diventa la nuova directory di log.
# sostituisce il file messages con il file mesgSS.temp

# cat /dev/null > messages
#* Non più necessario, perché il metodo precedente è più sicuro.

cat /dev/null > wtmp  #  ': > wtmp' e '> wtmp'  hanno lo stesso effetto.
# cat /dev/null > wtmp   svuota il file se esiste oppure lo crea vuoto
# equivale a : > wtmp
echo "Log cancellati."

exit 0
#  Il valore di ritorno zero da uno script
#+ indica alla shell la corretta esecuzione dello stesso.


