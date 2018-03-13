void setup() {
    Serial.begin(9600);
    float Gradi= -123.3436767;

    Serial.print("valore da convertire: ");Serial.println(Gradi);
    String stringVal = "";      //( reset
    stringVal = String(int(Gradi)) + "." + String(getDecimal(Gradi)); //combining both whole and decimal part in string with a fullstop between them

    Serial.print("stringVal: ");Serial.println(stringVal);              //display string value

    char charVal[stringVal.length()+1];                      //initialise character array to store the values

    stringVal.toCharArray(charVal,stringVal.length()+1);     //passing the value of the string to the character array

//------------------------------------------------------------
    Serial.print("charVal:   ");
    for(uint8_t i=0; i<sizeof(charVal)-1;i++) {                // stampa i caratteri singoli
        //{ if (charVal[i] == "\0") { break;}

        Serial.print(charVal[i]);
        Serial.print("_");} //display character array

    }

void loop() {}


//function to extract decimal part of float
long getDecimal(float val) {

    int intPart = int(val);     // prende la parte intera es. di 317,23 ottiene 317
    long decPart = 1000 * (val - intPart);  // ottengo prima 0,23 poi 230 (moltiplicato per 1000) x 3 decimali
    Serial.print("parte intera del numero    :");Serial.println(intPart);
    Serial.print("parte decimale del numero  :");Serial.println(decPart);
                                        //Change to match the number of decimal places you need
   //if (decPart < 0) {decPart * -1;}    // se negativo lo rende positivo
    if (decPart > 0)
        { return(decPart);}             //return the decimal part of float number if it is available

    else if(decPart < 0)                //
        { return((-1)*decPart);}        //if negative, multiply by -1

    else { return(0);}                   //return 0 if decimal part of float number is not available
}
