


















//=============================================================================
//=============================================================================
//=============================================================================

/*
void MenuPrincipale1(void) { // da togliere, era solo per valutare switch e case

    lcd.clear();
    //char testo2[] = "Modifica  ora   "; // visualizza CAMBIO ORA --------------
    WriteString(testo2, 0, 0);          // scrivo su LCD

    //char testo3[] = "                "; //
    //WriteString(testo3, 0, 1);

    lcd.setCursor(1,1);     // posizione del carattere di skip
    lcd.print((char)7);     // carattere skip
    lcd.setCursor(14,1);    // posizione del carattere  enter
    lcd.print((char)4);     // carattere enter

    //inizio variazione
    //int puntatore = 0;  // partenza
    Key_Press = 0;  // reset

    while (Key_Press == 0) {        // se e' vera esegue le istruzioni dentro le graffe
        Key_Press = LeggeTasti();
        Serial.print("====> legge tasti :");Serial.println(Key_Press);
    }

    int puntatore = 1 ; //puntatore + 1 ;
    while (puntatore <= 4) {
        Serial.print("puntatore: "); Serial.println(puntatore);     // ??????????
        switch (puntatore) {
            case 1:
                Serial.println("premuto skip1");
                //testo2a = "Modifica  ora   "; // visualizza CAMBIO ORA --------------
                WriteString(testo2, 0, 0);          // scrivo su LCD
                //puntatore = puntatore + 1 ;
                break;
            case 2:
                Serial.println("premuto skip2");
                //char testo4a[] = "Modifica  data  "; // visualizza CAMBIO DATA -------------
                WriteString(testo4, 0, 0);
                //puntatore = puntatore + 1 ;
                break;
            case 3:
                Serial.println("premuto skip4");
                //char testo5a[] = "Ena/Disa rele'  "; // Ena/Disa rele ----------------------
                WriteString(testo5, 0, 0); // testo5
                //puntatore = puntatore + 1 ;
                break;
            case 4:
                Serial.println("premuto skip8");
                //char testo6a[] = "Cambia ora pompe"; // Cambia orario pompe start stop -----
                WriteString(testo6, 0, 0);
                //puntatore = puntatore + 1 ;
                break;
            default:
                Serial.println("premuto exit");
                break;
        }
    puntatore = puntatore + 1 ;
    }
    // ----------------------------------- ripristina schermata di partenza ---
    schermata_zero();
}

*/

/*
    Serial.print(" hdec :");Serial.println(H[1]);
    Serial.print(" huni :");Serial.println(H[2]);
    Serial.print(" mdec :");Serial.println(H[3]);
    Serial.print(" muni :");Serial.println(H[4]);
    Serial.print(" sdec :");Serial.println(H[5]);
    Serial.print(" suni :");Serial.println(H[6]);
    Serial.print(" ddec :");Serial.println(H[1]);
    Serial.print(" duni :");Serial.println(H[2]);
    Serial.print(" adec :");Serial.println(H[3]);
    Serial.print(" auni :");Serial.println(H[4]);
    Serial.print(" ydec :");Serial.println(H[5]);
    Serial.print(" yuni :");Serial.println(H[6]);

    displayTime(DS3231_indirizzo);
*/

/*
//-----------------------------------------------------------------------------
/*
int Read_Keyboard(void) {       // ritorna 0 se nessun tasto premuto o il valore del tasto
    value = tastiera.read8();       // legge la tastiera con i2c
    if (value != 0) {               // esame di quanto letto, se e' stato premuto
        value1 = value;             // salva il valore
        delay(50);                 // delay
    }
    else
        { return value; }           // altrimenti value=0, nessun tasto premuto!
                                    // attesa che il pulsante venga rilasciato
    value = tastiera.read8();       // rilegge per avere ancora il valore
    while(value!=0) {               // se diverso da 0, pulsante premuto
        delay(50);                  // ritardo
        value = tastiera.read8();   // rilegge
    }
    return value1;                  // ritorna il valore del tasto premuto
}
*/
//-----------------------------------------------------------------------------
/*
void clearA1F(DS3231_indirizzo) {    // reset del flag A1F dopo allarme e attiva il prossimo
    Wire.beginTransmission(DS3231_indirizzo);
    Wire.write(0x0F);       // inizia a scrivere/leggere dal registro 0F
    Wire.requestFrom(DS3231_indirizzo, 1);    // richiede la lettura di un byte

    while(Wire.available()) {
        char tmp = Wire.read();     // legge il byte 0F
        tmp = tmp && 0xFE;          // mette il bit0=0
        Wire.write(tmp);            // lo riscrive per riabilitarlo
    }
    Wire.endTransmission();
}
*/
/*
int CheckAlarm(int DS3231_indirizzo) {  // legge se c'e' o meno il flag di ALLARME 1
    Wire.beginTransmission(DS3231_indirizzo);
    Wire.write(0x0F);       // inizia a scrivere/leggere dal registro 0F
    Wire.requestFrom(DS3231_indirizzo, 1);    // richiede la lettura di un byte

    while(Wire.available()) {
        if ((Wire.read() && 0x01) == 1) {     // legge il byte 0F
            flag = 1;
        }          // mette il bit0=0
    }
    Wire.endTransmission();
    return(flag);
}
*/
/*
void setAlarm1(void) {  // setta allarme ! - da fare alla partenza ALLARME 1

    int uso = 0;    // mi serve per lavoro

    Wire.beginTransmission(DS3231_I2C_ADDRESS);

    Wire.write(0x07);   // si posiziona sul byte 07

    Wire.write(0x80);   // 7 ON A1M1 per Alarm 1 - abilita INT/SQL ogni secondo
    Wire.write(0x80);   // 8 ON A1M2 per Alarm 1 -  si deve mettere a posto nel
    Wire.write(0x80);   // 9 ON A1M3 per Alarm 1 -  reg 0x0E il bit 2 e il bit 0
    Wire.write(0x80);   // A ON A1M4 per Alarm 1 -

    Wire.write(0x00);   // B ON A2M2 per Alarm 2 - azzerato
    Wire.write(0x00);   // C ON A2M3 per Alarm 2 - azzerato
    Wire.write(0x00);   // D ON A2M4 per Alarm 2 - azzerato

    Wire.endTransmission();
    // - - - - - - - - - - - - - - - - - - - - - - -
    // deve mettere a 1 il bit0 e bit2 del reg 0E per attivare allarme 1
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0x0E);                          // si posiziona sul byte 0E
    Wire.requestFrom(DS3231_I2C_ADDRESS, 1);   // richiede la lettura di un byte

    while(Wire.available()) {   // attendo che arrivi il byte
        uso = Wire.read();   // leggo il byte 0E, devo mettere a 1 i bit 2 e 1
        uso = uso && 0xFA;   // azzero i bit 2 e 1
        uso = uso + 0x05;    // adesso metto a 1 i bit 2 e 1
        Wire.write(uso);     // scrivo il valore nel DS al reg. 0E
    }

    Wire.endTransmission();
}
*/
/*
//---------- per correzione tempo su orologio in un solo campo ----------------
void timeSS() {
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(3); // set next input to start at the seconds register
    Wire.write(05); // set
    Wire.endTransmission();
}
*/
//-----------------------------------------------------------------------------
/*
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year) {
    // sets time and date data to DS3231
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0);                // set next input to start at the seconds register
    Wire.write(decToBcd(second));       // 0 set seconds
    Wire.write(decToBcd(minute));       // 1 set minutes
    Wire.write(decToBcd(hour));         // 2 set hours
    Wire.write(decToBcd(dayOfWeek));    // 3 set day of week (1=Sunday, 7=Saturday)
    Wire.write(decToBcd(dayOfMonth));   // 4 set date (1 to 31)
    Wire.write(decToBcd(month));        // 5 set month
    Wire.write(decToBcd(year));         // 6 set year (0 to 99)
    Wire.endTransmission();
}
*/
//-----------------------------------------------------------------------------
// tutte le funzioni sotto sono state spostate in SteveGen.h
/*
void readDS3231time(byte *second,byte *minute,byte *hour,byte *dayOfWeek,
byte *dayOfMonth,byte *month,byte *year) {
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set DS3231 register pointer to 00h
    Wire.endTransmission();                                                 //???? dubbi, e' corretto?
    Wire.requestFrom(DS3231_I2C_ADDRESS, 7); // request seven bytes of data requestFrom    // non dovrebbe essere prima?
                                             //DS3231 starting from register 00h
    *second = bcdToDec(Wire.read() & 0x7f);
    *minute = bcdToDec(Wire.read());
    *hour = bcdToDec(Wire.read() & 0x3f);
    *dayOfWeek = bcdToDec(Wire.read());
    *dayOfMonth = bcdToDec(Wire.read());
    *month = bcdToDec(Wire.read());
    *year = bcdToDec(Wire.read());
}
*/
//-----------------------------------------------------------------------------
/*
void displayTime(DS3231_indirizzo) {
    byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
    // retrieve data from DS3231
    readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
    // send it to the serial monitor
    Serial.print(hour, DEC);
    // convert the byte variable to a decimal number when displayed
    Serial.print(":");
    if (minute<10) {
        Serial.print("0");
    }
    Serial.print(minute, DEC);
    Serial.print(":");
    if (second<10) {
        Serial.print("0");
    }
    Serial.print(second, DEC);
    Serial.print(" ");
    Serial.print(dayOfMonth, DEC);
    Serial.print("/");
    Serial.print(month, DEC);
    Serial.print("/");
    Serial.print(year, DEC);
    Serial.print(" Giorno della settimana: ");
    switch(dayOfWeek){
        case 1:
            Serial.println("domenica");
            break;
        case 2:
            Serial.println("lunedi'");
            break;
        case 3:
            Serial.println("martedi'");
            break;
        case 4:
            Serial.println("mercoledi'");
            break;
        case 5:
            Serial.println("giovedi'");
            break;
        case 6:
            Serial.println("venerdi'");
            break;
        case 7:
            Serial.println("sabato");
            break;
    }
}
*/
//-----------------------------------------------------------------------------   *****
/*
void displayRegDS3231() {
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0x00);
    Wire.endTransmission();
    Wire.requestFrom(DS3231_I2C_ADDRESS, 19);

    while(Wire.available()) {
        Rseconds = Wire.read();
        Rminutes = Wire.read();
        Rhours = Wire.read();
        Rdayof = Wire.read();
        Rday = Wire.read();
        Rmonth = Wire.read();
        Ryear = Wire.read();
        R07 = Wire.read();
        R08 = Wire.read();
        R09 = Wire.read();
        R0A = Wire.read();
        R0B = Wire.read();
        R0C = Wire.read();
        R0D = Wire.read();
        R0E = Wire.read();
        R0F = Wire.read();
        R10 = Wire.read();
        R11 = Wire.read();
        R12 = Wire.read();
    }
    displayTime(DS3231_indirizzo);
    Serial.println(" ");
    Serial.println("Registi del DS3231 in hex :");Serial.println(" ");
    Serial.print("  R00 R01 R02 ->  ");Serial.print(Rhours,HEX);Serial.print(":");Serial.print(Rminutes,HEX);Serial.print(":"); Serial.println(Rseconds,HEX);
    Serial.print("  R03 -> "); Serial.print(Rdayof,HEX);Serial.println("  giorno della settimana");
    Serial.print("  R04 -> "); Serial.print(Rday,HEX);Serial.print("/");Serial.print(Rmonth,HEX);Serial.print("/");Serial.println(Ryear,HEX);
    Serial.println(" ");

    Serial.println("Allarme 1");
    Serial.print("  R07 -> "); Serial.println(R07,BIN);
    Serial.print("  R08 -> "); Serial.println(R07,BIN);
    Serial.print("  R09 -> "); Serial.println(R07,BIN);
    Serial.print("  R0A -> "); Serial.println(R0A,BIN);
    Serial.println(" ");

    Serial.println("Allarme 2");
    Serial.print("  R0B -> "); Serial.println(R0B,BIN);
    Serial.print("  R0C -> "); Serial.println(R0C,BIN);
    Serial.print("  R0D -> "); Serial.println(R0D,BIN);
    Serial.println(" ");

    Serial.print("Control Reg.0x0E :"); Serial.println(R0E,BIN);
    Serial.println(" ");

    Serial.print("Status  Reg.0x0F :"); Serial.println(R0F,BIN);
    Serial.println(" ");

    Serial.print("Aging   Reg.0X10 :"); Serial.println(R10,BIN);
    Serial.println(" ");

    Serial.println("Due byte per la temperatura:");
    Serial.print("  R11 -> "); Serial.println(R11,BIN);
    Serial.print("  R12 -> "); Serial.println(R12,BIN);

    Serial.print(" Temperatura °C ");
    Serial.println(DS3231_get_treg(DS3231_I2C_ADDRESS, DS3231_TEMPERATURE_ADDR));  // stampa la temperatura
}
*/



//-----------------------------------------------------------------------------    Create funzioni in SteveLib
// Convert normal decimal numbers to binary coded decimal
//byte decToBcd(byte val) { return( (val/10*16) + (val%10) );}
//-----------------------------------------------------------------------------
// Convert binary coded decimal to normal decimal numbers
//byte bcdToDec(byte val) { return( (val/16*10) + (val%16) );}

//-----------------------------------------------------------------------------
/*
float DS3231_get_treg(int DS3231_indirizzo, int DS3231_TEMPERATURE_registro) {
    float rv;  // oppure    int rv;  // Reads the temperature as an int, to save memory

    uint8_t temp_msb, temp_lsb;
    int8_t nint;

    Wire.beginTransmission(DS3231_indirizzo);
    Wire.write(DS3231_TEMPERATURE_registro);
    Wire.endTransmission();

    Wire.requestFrom(DS3231_indirizzo, 2);
    temp_msb = Wire.read();
    temp_lsb = Wire.read() >> 6;

    if ((temp_msb & 0x80) != 0) {
        nint = temp_msb | ~((1 << 8) - 1);}      // if negative get two's complement
    else {nint = temp_msb;}

    rv = 0.25 * temp_lsb + nint;
    return rv;
}
*/