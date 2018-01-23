 #include <stdio.h>

int counter = 0;

void setup() {
    Serial.begin(9600);
    // -------------------------------------------------------------------------
    int test1 = 10;             // definisco la variabile
    int   *ptr1;                // definisco il pointer
    ptr1 = &test1;              // ptr1 contiene l'indirizzo fisico di test1
    // -------------------------------------------------------------------------
    char test2 = 90;             // definisco la variabile
    char   *ptr2;                // definisco il pointer
    ptr2 = &test2;              // ptr1 contiene l'indirizzo fisico di test1
    // -------------------------------------------------------------------------
    float test3 = 3.141517121;             // definisco la variabile
    float   *ptr3;                // definisco il pointer
    ptr3 = &test3;              // ptr1 contiene l'indirizzo fisico di test1
    // -------------------------------------------------------------------------
    int array[100];             // definisco matrice
    array[1] = {9};
    int *ptr_arr;                // definisco il pointer
    ptr_arr = array;            // indirizzo in memoria, corrisponde a ptr_arr = &array[0]

    // -------------------------------------------------------------------------
    Serial.println("--");
    Serial.print(" Contenuto della variabile test1 / *ptr1 = "); Serial.print(test1);Serial.print(" / "); Serial.println(*ptr1);      //
    Serial.print(" Ptr1 / &test1  in memoria in Hex: "); Serial.print( (long) ptr1, HEX);Serial.print(" / "); Serial.println( (long) &test1, HEX);
    Serial.println("--");   // indirizzo fisico memoria
    // -------------------------------------------------------------------------
    Serial.print(" Contenuto della variabile test2 / *ptr2 = "); Serial.print(test2);Serial.print(" / "); Serial.println(*ptr2);      //
    Serial.print(" Ptr2 / &test2  in memoria in Hex: "); Serial.print( (long) ptr2, HEX);Serial.print(" / "); Serial.println( (long) &test2, HEX);
    Serial.println("--");
    // -------------------------------------------------------------------------
    Serial.print(" Contenuto della variabile test3 / *ptr3 = "); Serial.print(test3);Serial.print(" / "); Serial.println(*ptr3);      //
    Serial.print(" Pointer in memoria in Hex: "); Serial.print( (long) ptr3, HEX);Serial.print(" / ");Serial.println( (long) &test3, HEX);      // indirizzo fisico memoria
    Serial.println("--");
    // -------------------------------------------------------------------------
    Serial.print(" Contenuto della variabile array= "); Serial.println(array[1]);       //
    Serial.print(" =  contenuto del pointer: "); //Serial.println(*array[1]);
    Serial.print(" Pointer in memoria in Hex: "); Serial.println( (long) ptr_arr, HEX);    // indirizzo fisico memoria
    Serial.print(" =  pointer in memoria in Hex: "); Serial.println( (long) &ptr_arr[0], HEX);   // "
    Serial.println("");
    // -------------------------------------------------------------------------
    Serial.println("--***--");
    Serial.print(" sizeof di test1 : ");Serial.println(sizeof(test1));
    Serial.print(" sizeof di test2 : ");Serial.println(sizeof(test2));
    Serial.print(" sizeof di test3 : ");Serial.println(sizeof(test3));
    Serial.print(" sizeof di array : ");Serial.println(sizeof(array));
    //float test2 = 3.14159;
    //char test3[ ] = "This is a character array";
    //float *ptr2;
    //char  *ptr3;

    //ptr2 = &test2;
    //ptr3 = &test3;


}

void loop() {
    //Serial.println("--__--");
    //exit(0);

}

