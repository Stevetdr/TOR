#!/bin/bash

# Eseguite lo script con almeno 10 parametri, per esempio
# ./nomescript 1 2 3 4 5 6 7 8 9 10
MINPARAM=10

echo

echo "Il nome dello script è \"$0\"."
# Aggiungete ./ per indicare la directory corrente
echo "Il nome dello script è \"`basename $0`\"."
# Visualizza il percorso del nome (vedi 'basename')

echo
# -n stringa not null
if [ -n "$1" ]                #  Utilizzate il quoting per la variabile
                              #+ da verificare.
then
 echo "Il parametro #1 è $1"  # È necessario il quoting
                              #+ per visualizzare il #
fi

if [ -n "$2" ]
then
 echo "Il parametro #2 è $2"
fi

if [ -n "$3" ]
then
 echo "Il parametro #3 è $3"
fi

# ...


if [ -n "${10}" ]  #  I parametri > $9 devono essere racchiusi
                   #+ tra {parentesi graffe}.
then
 echo "Il parametro #10 è ${10}"
fi

echo "-----------------------------------"
echo "In totale i parametri passati sono : "$#""     # $*

if [ $# -lt "$MINPARAM" ]   #se $# meno di $MINPARAM allora
then
  echo
  echo "Lo script ha bisogno di almeno $MINPARAM argomenti da riga di comando!"
fi

echo

exit 0