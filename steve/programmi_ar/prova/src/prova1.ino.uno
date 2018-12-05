/*
Questo programma  serve per trasformare un valore long in n.valori
 base 100 per essere poi salvati in memoria (nel mio caso futuro).
 Esempio: il valore 524.999 popola una matrice con risultato:
  result[3]=52, result[2]=49 e result[1]=99 che ricomposto, moltiplicato
  da' 524999
  */

#include <Wire.h>
long val = 201524999;
long resultout[10]; // int

//------------------------------------------------------------------------------
void setup(void) {
    Serial.begin(9600);
}
//------------------------------------------------------------------------------
void loop() {
resultout[] = loreto2(val);
stampaggio(5, resultout);
}
//------------------------------------------------------------------------------
long loreto2( long valore) {
    long resto;
    long result[10]; // int
    Serial.println("Loreto2 ");

    Serial.print(" valore da lavorare:  ");Serial.println(valore);
    int index = 0;
    while (true) {
        index += 1;

        if (valore > 99) {
            resto  = valore%100;
            valore = valore/100;
            result[index] = resto;
        }
        else {
            resto = valore;
            result[index] = resto;
            valore = 0;
            break;
        }
    }
    return (result);
}
//------------------------------------------------------------------------------
void stampaggio(int index, long resultout) {
    Serial.println("------------------------------");
    for (int i = index ; i > 0 ; i--) {
        Serial.print("i= ");Serial.print(i);Serial.print("    valore= ");Serial.println(resultout[i]);
    }
    Serial.println("  ");
    Serial.print(" ricalcolato : ");Serial.println(resultout[5]*100000000 + resultout[4]*1000000 +resultout[3]*10000 + resultout[2]*100 + resultout[1]);
    Serial.println("  ");
}
//------------------------------------------------------------------------------







#define DIM 15      // definisce un valore

void InArray (int[ ], int);
//-----------------
main( ) {
    int a[DIM];
    printf (“Inserisci %d interi \n”, DIM);
    InArray(a, DIM);
}


//-----------------
void InArray (int arr[ ], int n) {
    int i;
    if (n > 0)
    for ( i = 0; i < n; i++)
    scanf(“%d”, &arr [ i ]);
}












/*
//------------------------------------------------------------------------------
void loreto1( long valore) {
    long resto;
    Serial.println("Loreto1 ");

    Serial.print(" valore da lavorare:  ");Serial.println(valore);
    while (true) {
        if (valore > 99) {
            resto  = valore%100;
            valore = valore/100;
            Serial.print(" resto  :  ");Serial.print(resto); Serial.print(" - valore :  ");Serial.println(valore);
        }
        else {
            resto = valore;
            valore = 0;
            Serial.print(" resto  :  ");Serial.print(resto); Serial.print(" - valore :  ");Serial.println(valore);
            break;
        }
    }
}
*/