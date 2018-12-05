//
void setup(void) {
    Serial.begin(9600);
}
// ============================================================================
void loop() {
    VaiCheckTest();
    delay(3000);
}
// ============================================================================
void VaiCheckTest(void) {

long secondi_tot;      // unsigned long fino a 4,294,967,295
long mese_sec;          // unsigned
long giorno_sec;
long ora_sec;
long minuti_sec;
long secondi_sec;

long a = 4;     // mese         //int
long b = 9;     // giorno
long c = 16;    // ora
long d = 30;    // minuti
long e = 30;    // secondi

mese_sec   = a *31*24*60*60;         // 10.713.600
giorno_sec = b *   24*60*60;         //    345.600
ora_sec    = c *      60*60;         //     57.600  ok
minuti_sec = d *         60;         //      1.260  ok
secondi_sec= e;                      // secondi     ok

// trasformo tutto in secondi, escludo l'anno
secondi_tot = mese_sec + giorno_sec + ora_sec + minuti_sec + secondi_sec ;
long secondi_prima = 11500000;  // nel range
long secondi_dopo  = 11700000;

if (secondi_tot > secondi_prima && secondi_tot < secondi_dopo) {
    Serial.println("1 entro il confine! ");
}
else {Serial.println("1 fuori dal range "); }
//------------------------------------------------------------------------------
secondi_prima = 11400000;  // sotto
secondi_dopo  = 11500000;

if (secondi_tot > secondi_prima && secondi_tot < secondi_dopo) {
    Serial.println("2 entro il confine! ");
}
else {Serial.println("2 fuori dal range "); }
//------------------------------------------------------------------------------
secondi_prima = 11700000;  // sotto
secondi_dopo  = 11800000;

if (secondi_tot > secondi_prima && secondi_tot < secondi_dopo) {
    Serial.println("3 entro il confine! ");
}
else {Serial.println("3 fuori dal range "); }
//------------------------------------------------------------------------------
//Serial.print(" Valore convertito    :");Serial.println(secondi_tot);
//Serial.println("- - - - - - -  -- ");
//
//Serial.println();
//long mySec;// = 1234567899; //1.234.567.899
//mySec=secondi_tot;
//Serial.println();
//loreto1(mySec);
//Serial.println();    Serial.println();    Serial.println();
//mySec=secondi_tot;
//loreto2(mySec);
//Serial.println();
}
//-----------------------------------------------------------------------------------
void loreto1( long valore) {
    long resto;
    Serial.println("Loreto1 ");

    Serial.print(" valore da lavorare:  ");Serial.println(valore);
    while (true) {
        if (valore > 999) {
            resto  = valore%1000;
            valore = valore/1000;
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
//-----------------------------------------------------------------------------------
void loreto2( long valore) {
    long resto;
    int result[10];
    Serial.println("Loreto2 ");

    Serial.print(" valore da lavorare:  ");Serial.println(valore);
    int index = 0;
    while (true) {
        index += 1;

        if (valore > 999) {
            resto  = valore%1000;
            valore = valore/1000;
            Serial.print(" resto  :  ");Serial.print(resto); Serial.print(" - valore :  ");Serial.println(valore);
            result[index] = resto;
        }

        else {
            resto = valore;
            result[index] = resto;
            valore = 0;
            Serial.print(" resto  :  ");Serial.print(resto); Serial.print(" - valore :  ");Serial.println(valore);
            break;
        }
    }

    Serial.print(" index  :  ");Serial.println(index);
    Serial.println();
    Serial.print(" result  :  ");Serial.print(result[index--]);
    Serial.print('.');Serial.print(result[index--]);
    Serial.print('.');Serial.print(result[index--]);
    Serial.print('.');Serial.print(result[index--]);
    Serial.println();
}
//-----------------------------------------------------------------------------------

//Serial.print(" Valore convertito    :");Serial.println(secondi_tot);
//
//Serial.print(" secondi nel mese     :");Serial.println(mese_sec);
//Serial.print(" secondi nel giorno   :");Serial.println(giorno_sec);
//Serial.print(" secondi in ora       :");Serial.println(ora_sec);
//Serial.print(" secondi in minuti    :");Serial.println(minuti_sec);
//Serial.print(" secondi in secondi   :");Serial.println(secondi_sec);
//Serial.println("  - - - - - -  -- ");
//Serial.println("- - - - - - -  -- ");
//Serial.print(" secondi nel mese     :");Serial.println(mese_sec);
//Serial.print(" secondi nel giorno   :");Serial.println(giorno_sec);
//Serial.print(" secondi in ora       :");Serial.println(ora_sec);
//Serial.print(" secondi in minuti    :");Serial.println(minuti_sec);
//Serial.print(" secondi in secondi   :");Serial.println(secondi_sec);
