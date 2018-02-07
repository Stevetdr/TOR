#!/bin/bash          
ECHO="-e"
mail_text="testo della mail da spedire"
mail_subject="questo e' il soggetto della mail"
mail_to=stevetdruser@gmail.com
$ECHO "Subject:$mail_subject\nTo:mail_to\n$mail_text"|sendmail $mail_to
