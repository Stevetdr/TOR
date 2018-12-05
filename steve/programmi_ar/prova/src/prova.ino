/*
Questo programma  usa una funzione InArray a cui si spedisce un array arr con il numero
 di elementi che contiene. La funzione (per esempio) fa' uno scan degli elementi e
 rimanda la somma degli elementi
  */

#define ELEMENTI 6      // definisce un valore

void setup(void) {
    Serial.begin(9600);
}
//-----------------
void loop() {
    long pippo = 0 ;
    int arr[ELEMENTI]; // definizione dell'array
    arr[1]=100; arr[2]=200; arr[3]=300; arr[4]=400; arr[5]=500; arr[6]=600;

    pippo = InArray(arr, ELEMENTI);    // chiama la routine e ottiene pippo
                                       // passa array e numero elementi

    Serial.println(); Serial.println(pippo);
}
//-----------------
long InArray (int arr[ ], int n) {
    Serial.println("--");
    int i;
    long gallo = 0;
    if (n > 0) {
        for ( i = 1; i <= n; i++) {
            Serial.print (" nr.:");Serial.print(i);
            Serial.print("    cont.: ");Serial.println(arr[i]);
            gallo = gallo + arr[i];
        }
    }
    return gallo;
}
//-----------------


