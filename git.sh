#!/bin/bash

data=`date '+Salvato il %d-%m-%Y alle %H:%M:%S'`

# parametri aggiuntivi
git config --global color.branch auto
git config --global color.diff auto
git config --global color.interactive auto
git config --global color.status auto

# lo status iniziale era:
#user.name=stevetdr
#user.email=stevetdr@gmail.com
#core.editor=
#core.editorwhich=subl
#push.default=simple
#credential.helper=cache --timeout=5
#core.repositoryformatversion=0
#core.filemode=true
#core.bare=false
#core.logallrefupdates=true
#remote.origin.url=git@github.com:Stevetdr/GIT-REPO-p3
#remote.origin.fetch=+refs/heads/*:refs/remotes/origin/*
#
echo " "
echo " Salvataggio dati di GIT-REPO da pi3 al repository GIT-REPO-p3 su remoto"
echo " "
echo " "

echo " ===> Situazione parametri globali: ----------------------- 1"
#git config -l
echo " "
echo " ===> Aggiungo eventuali file modificati: ----------------- 2"
echo " "
git add --all
echo " ===> Status:  -------------------------------------------- 3"
echo " "
git status
echo " ===> Costruisco la commit ... ---------------------------- 4"
echo "        il commento sara': $data"
echo " "
git commit -a -m "$data"   #  -a -m
git status
echo " ===> Controllo di essere sul master: --------------------- 5"
echo " "
git checkout master             # controllo di essere sul master
echo " ===> Effettuo spostamenti su repo remoto ----------------- 6"
echo " "
#git remote add origin git@github.com:Stevetdr/GIT-REPO-p3
git push -u origin master          # effettuo spostamenti file modificati
echo "------------------------------------------------------------"

# da esaminare:
# git init    # prima volta da directory da salvare
              # controllare che non ci siano altri .git perche' impediscono
              #  il salvataggio
#git add *    # per salvare tutto comprese le sottodirectory ed i files

# (controllo se c'e' o meno il remote con
#git remote -v
#git remote add origin git@github.comstevetdr/TOR master
#git commit -am "Con il commento da registrare"
# A questo punto un po' di dubbi, mi pare:
# git pull origin git@github.comstevetdr/TOR master # per spostare i file
    # dal remote (tipo i read me iniziali) al locale che sono io
    #git push - u origin master  # che salva il tutto

#    GIT REMOTE                           LOCAL
#     origin                              master
#   GITHUB in rete                        su pi3
