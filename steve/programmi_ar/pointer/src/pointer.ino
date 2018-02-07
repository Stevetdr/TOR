 #include <stdio.h>


void setup() {
    //int a = 10;
    int *myPointer;     // definisce il pointer
    //char myVar[]={'A','B','C','D','E','F','G','H','I','J','\0'};    // crea array
    int myVar[]={1,2,3,4,9,3,7,2,5,1,3};

    Serial.begin(9600);

    //Serial.print("char myVar[]={'A','B','C','D','E','F','G','H','I','J','\0'}");
    Serial.println("int myVar[]={1,2,3,4,9,3,7,2,5,1,3}");
    //myPointer = &myVar[0];  // punta all'indirizzo del valore 0
    myPointer = myVar;
                            // *myPointer punta al contenuto
    //Serial.print ("myPointer    : "); Serial.println(myPointer);
    Serial.print ("-----------------");
    type(myVar[]);
    type(myPointer);
    Serial.print ("-----------------");

    Serial.print ("&myVar[0] uguale a *myPointer  : "); Serial.println(*myPointer);

    myPointer = myPointer +1;
    Serial.print
    ("myPointer +1 : "); Serial.println(*myPointer);

    myPointer = myPointer +2;
    Serial.print ("myPointer +2 : "); Serial.println(*myPointer);

    myPointer = sizeof(myVar);
    Serial.print ("myPointer +2 : "); Serial.println(*myPointer);
}



void loop() {
    //return 0;
}


/*
void setup() {
  int *myPointer;
  int myVar[5] = {1, 3, 5, 6, 8};
  myPointer = &myVar[0]; // Holds the address of the 1st element.
  myPointer = myPointer + 2;
// Incremented by 2 to get 3rd value of our array.
  Serial.begin(9600);
  Serial.print(*myPointer);
}
*/
