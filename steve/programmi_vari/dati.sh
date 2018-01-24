#!/bin/bash


data=`date '+Salvato il %d-%m-%Y alle %H:%M:%S'`
echo $data

echo " 1 " $1       # primo parametro passato all bash
echo " 2 " $2       # secondo parmetro passato
echo " 2a" $3       # terzo parmetro passato
echo " 3 " $PWD     # directory di dove si trova il programma
echo " 4 " $UID     # 1000???
echo " 5 " $PATH    # PATH generali
echo " 6 " $HOME    #
