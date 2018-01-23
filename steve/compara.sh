#!/bin/bash

dir_a="/home/pi/GIT-REPO/steve/"
dir_b="/home/pi/steve/"

rm trash1.txt		# canella i file creati nel giro precedente
rm trash2.txt  		# canella i file creati nel giro precedente

echo "Lanciando il comando bash compara.sh si ottiene la comparazione delle "
echo " due directory e la stampa delle singole differenze su:"
echo ""
echo "-> 1: "$dir_a
echo "-> 2: "$dir_b
echo ""
echo ""
echo "File presenti in " $dir_a " e --NON-- presenti in " $dir_b 
echo "----------------------------------------------------------"
echo ""
diff -r --brief $dir_a $dir_b | grep $dir_a
echo ""
echo ""
echo "File presenti in " $dir_b " e --NON-- presenti in " $dir_a 
echo "----------------------------------------------------------"
echo ""
diff -r --brief $dir_a $dir_b | grep $dir_b
echo ""
echo ""
echo ""
#===============================================================
echo "File presenti in " $dir_a " e --NON-- presenti in " $dir_b > trash1.txt
echo "----------------------------------------------------------" >> trash1.txt
#
diff -r --brief $dir_a $dir_b | grep $dir_a >> trash1.txt
#
#
echo "File presenti in " $dir_b " e --NON-- presenti in " $dir_a > trash2.txt
echo "----------------------------------------------------------" >> trash2.txt
#
diff -r --brief $dir_a $dir_b | grep $dir_b >> trash2.txt

echo "( Crea su questa directory i file trash1.txt e trash2.txt con il contenuto"
echo " dei risultati delle operazioni. Li cancella alla prossima ripartenza.)"
