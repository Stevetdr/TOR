#!/usr/bin/env python

while True:
    try:
        X = int(input("Entra un numero intero: "))
        break
    except ValueError:
        print (ValueError)
        print ("Oops!  Non digitato un numero intero.  Prova ancora...")
