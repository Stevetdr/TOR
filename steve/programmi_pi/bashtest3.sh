
#!/bin/bash



# L'utilissimo costrutto "if-grep":
# --------------------------------
if grep -q Bash file
then echo "File contiene almeno un'occorrenza di Bash."
fi

parola=Linux
sequenza_lettere=inu
if echo "$parola" | grep -q "$sequenza_lettere"
# L'opzione "-q" di grep elimina l'output.
then
  echo "$sequenza_lettere trovata in $parola"
else
  echo "$sequenza_lettere non trovata in $parola"
fi


if COMANDO_CON_EXIT_STATUS_0_SE_NON_SI_VERIFICA_UN_ERRORE
then echo "Comando eseguito."
else echo "Comando fallito."
fi