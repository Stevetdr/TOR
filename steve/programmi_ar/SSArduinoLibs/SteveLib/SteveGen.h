//
// autore Steve
// PROTOTYPE ************************************************** inizio prototype
void displayTime(int DS3231_indirizzo);
void readDS3231time(int DS3231_indirizzo, byte *second,byte *minute,byte *hour,byte *dayOfWeek,
byte *dayOfMonth,byte *month,byte *year);
void stampaGradi(float gradiC, int col, int linea);     // inserita in GMM con funzione
long getDecimali(float val);                            // questo era gia' qui

void setDS3231time(int DS3231_indirizzo, byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year);
void ScriveEEprom (int valore, int address);
void WriteString(char testo[], int col, int linea) ;
//*************************************************************** fine prototype
//==============================================================================
//==============================================================================
//==============================================================================

//-----------------------------------------------------------------------------
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val) { return( (val/16*10) + (val%16) );}
//-----------------------------------------------------------------------------
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val) { return( (val/10*16) + (val%10) );}
//-----------------------------------------------------------------------------
// legge i registri del DS e restituisce il valore convertito in float
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
//-----------------------------------------------------------------------------
// legge e visualizza tutti i registri del DS3231 su Serial
void displayRegDS3231(int DS3231_indirizzo, int DS3231_TEMPERATURE_registro) {
byte Rseconds = 0; byte Rminutes = 0; byte Rhours = 0; byte Rdayof = 0;
byte Rday = 0; byte Rmonth = 0; byte Ryear = 0;
byte R07 = 0; byte R08 = 0; byte R09 = 0; byte R0A = 0; byte R0B = 0; byte R0C = 0;
byte R0D = 0; byte R0E = 0; byte R0F = 0; byte R10 = 0; byte R11 = 0; byte R12 = 0;

    Wire.beginTransmission(DS3231_indirizzo);
    Wire.write(0x00);
    Wire.endTransmission();
    Wire.requestFrom(DS3231_indirizzo, 19);

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
    Serial.print("  R08 -> "); Serial.println(R08,BIN);
    Serial.print("  R09 -> "); Serial.println(R09,BIN);
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
    Serial.println(DS3231_get_treg(DS3231_indirizzo, DS3231_TEMPERATURE_registro));  // stampa la temperatura
}
//-----------------------------------------------------------------------------
//visualizza su Serial l'orario del DS3231 indicato
void displayTime(int DS3231_indirizzo) {
    byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
    // retrieve data from DS3231
    readDS3231time(DS3231_indirizzo, &second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
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
//-----------------------------------------------------------------------------
void readDS3231time(int DS3231_indirizzo, byte *second,byte *minute,byte *hour,byte *dayOfWeek,
byte *dayOfMonth,byte *month,byte *year) {
    Wire.beginTransmission(DS3231_indirizzo);
    Wire.write(0); // set DS3231 register pointer to 00h
    Wire.endTransmission();                           //???? dubbi, e' corretto?
    Wire.requestFrom(DS3231_indirizzo, 7); // request seven bytes of data requestFrom
                                             //DS3231 starting from register 00h
    *second = bcdToDec(Wire.read() & 0x7f);
    *minute = bcdToDec(Wire.read());
    *hour = bcdToDec(Wire.read() & 0x3f);
    *dayOfWeek = bcdToDec(Wire.read());
    *dayOfMonth = bcdToDec(Wire.read());
    *month = bcdToDec(Wire.read());
    *year = bcdToDec(Wire.read());
}
//-----------------------------------------------------------------------------
void setDS3231time(int DS3231_indirizzo, byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year) {
    // sets time and date data to DS3231
    Wire.beginTransmission(DS3231_indirizzo);
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
//-----------------------------------------------------------------------------
void timeSS(int DS3231_indirizzo) {
    //---------- per correzione tempo su orologio in un solo campo ----------------
    Wire.beginTransmission(DS3231_indirizzo);
    Wire.write(06); // set  l'indirizzo in cui cambiare il
    Wire.write(18); // set   valore di questa Wire
    Wire.endTransmission();
}
//-----------------------------------------------------------------------------
void setAlarm1(int DS3231_indirizzo) {  // setta allarme ! - da fare alla partenza ALLARME 1

    int uso = 0;    // mi serve per lavoro

    Wire.beginTransmission(DS3231_indirizzo);

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
    Wire.beginTransmission(DS3231_indirizzo);
    Wire.write(0x0E);                          // si posiziona sul byte 0E
    Wire.requestFrom(DS3231_indirizzo, 1);   // richiede la lettura di un byte

    while(Wire.available()) {   // attendo che arrivi il byte
        uso = Wire.read();   // leggo il byte 0E, devo mettere a 1 i bit 2 e 1
        uso = uso && 0xFA;   // azzero i bit 2 e 1
        uso = uso + 0x05;    // adesso metto a 1 i bit 2 e 1
        Wire.write(uso);     // scrivo il valore nel DS al reg. 0E
    }

    Wire.endTransmission();
}
//-----------------------------------------------------------------------------
int CheckAlarm(int DS3231_indirizzo) {  // legge se c'e' o meno il flag di ALLARME 1
    int flag = 0;   // azzera il flag
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
//-----------------------------------------------------------------------------
void clearA1F(int DS3231_indirizzo) {    // reset del flag A1F dopo allarme e attiva il prossimo
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
//-----------------------------------------------------------------------------
int LeggeTasti() {  // legge tasti e restituisce 8-4-2-1 o zero
    int tastopremuto = 0;    // reset del tasto
    int memoria = 0;         // copia
    int flag = 0;           // flag tasto premuto

    if (digitalRead(8)  == 0) {tastopremuto = 1; flag = 1 ;memoria = 8  ;}
    if (digitalRead(9)  == 0) {tastopremuto = 2; flag = 1 ;memoria = 9  ;}
    if (digitalRead(10) == 0) {tastopremuto = 4; flag = 1 ;memoria = 10 ;}
    if (digitalRead(11) == 0) {tastopremuto = 8; flag = 1 ;memoria = 11 ;}

    if (flag == 1) {      // loop per attendere rilascio tasto
        flag = 0;         // reset del flag e poi attendo rilascio del tasto
        while(digitalRead(memoria) != 1 ) { delay(100); }
    }
    return (tastopremuto);

    // devo inserire una funzione che conta e poi restituisce un valore es.99
    // oppure il valore del timer che dopo un certo numero di secondi mi fa'
    //  uscire dalla funzione se non premo altri tasti.
}
//-----------------------------------------------------------------------------
String DaHEXaBin(int valore, int nr_bit) {
    // necessita di definire nel programma String pippo="" per il return
    // riceve un valore e restituisce una stringa lunga nr_bit (left zeroed) con blank
    // substring(A,B): A inizio stringa ( parte a 0) B nr. caratteri da prendere
    int zeros = String(valore,BIN).length();//This will check for the length of myNum in binary.
    int lungo = nr_bit;               // numero dei bit da convertire
    String myStr = "";                // crea stringa vuota myStr

    if (valore <= 255) {   // controllo se (8 bit) valore maggiore di 255

        for (int i=0; i < lungo - zeros; i++) {  // Aggiunge zeri a completamento
            myStr = myStr + "0"; }

        myStr = myStr + String(valore,BIN);  // zeri a sinistra+stringa iniziale
        myStr = myStr.substring(0,4) + " " + myStr.substring(4,8); // split
    }
    else { myStr = "too high!";}  // restituisce il valore troppo alto

    return(myStr);
}
//-----------------------------------------------------------------------------
void ResetEEprom(void) {    // resetta a 0 tutta la memoria del nano
    //
    for (int i = 0; i <= 0x800; i++) {ScriveEEprom(0x00,i);}
}
//-----------------------------------------------------------------------------
int LeggeEEprom (int address) {
    int value = 0;      // variabile
    //value = EEPROM.get(address, data);        // o questa                 cosa serve il data nella get?
    value = EEPROM.read(address);               // o questa
    return value;   // ritorna il valore letto
}
//-----------------------------------------------------------------------------
void ScriveEEprom (int valore, int address) {
    //EEPROM.put(address, valore);
    EEPROM.write(address, valore);
    return ;       // tanto per scrivere qualcosa
}
//-----------------------------------------------------------------------------
void stampaGradi(float gradiC, int col, int linea) {
    // PROVARE ad inviare il valore e prendere la stringa per portare fuori la WriteString(---) !!!!!
    // si potra' anche togliere la col e la linea da sopra

    // ATTENZIONE: il valore gradiC deve essere compreso tra -32000 e +32000
    //  oltre si sballa tutto. Converte solo 3 decimali
    // stampa temperatura in gradiC del DS sul LCD da colonna su linea
    // non si riesce a mettere in SteveGen perche' mi da' errore
    String stringVal = "";      // reset della stringa

    stringVal = String(int(gradiC)) + "." + String(getDecimali(gradiC));
    char charVal[stringVal.length()+1];

    // se gradi=>0 mette il "+" davanti alla cifra, altrimenti mette il "-"
    if (gradiC >= 0) {stringVal =  "+" + stringVal;}
    else {stringVal =  "-" + stringVal;}

    stringVal.toCharArray(charVal,stringVal.length()-1); // +1

    WriteString(charVal, col, linea);     // solo questo OK
    //WriteString(charVal, 1, 1);     // questa e' la vecchia istruzione che funzionava, da togliere
}       // esce da stampaGradi
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
long getDecimali(float val) {
    // questa funzione estrae la parte decimale del float dei gradi
    int intPart = int(val);     // prende la parte intera es. di 317,23 ottiene 317
    long decPart = 1000 * (val - intPart);  // ottengo prima 0,23 poi 230 (moltiplicato per 1000) x 3 decimali
    if (decPart >= 0) { return(decPart);} // se c'e' ritorna la parte decimale
    else if(decPart < 0) { return((-1)*decPart);} // se negativo, moltiplica per -1
    else { return(0);}                  //return 0 se non c'e' parte decimale
}       // esce da getDecimali vedi stampaGradi per utilizzo
//------------------------------------------------------------------------------
void WriteExtEEprom( int deviceaddress, unsigned int eeaddress, byte data ) {
    // indirizzo i2c della eeprom, indirizzo su cui scrivere, dato
    int rdata = data;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8));      // MSB
    Wire.write((int)(eeaddress & 0xFF));    // LSB
    Wire.write(rdata);
    Wire.endTransmission();
}
//------------------------------------------------------------------------------
byte ReadExtEeprom( int deviceaddress, unsigned int eeaddress ) {
    // indirizzo i2c della eeprom, indirizzo da cui leggere
    byte rdata = 0xFF;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,1);
    if (Wire.available()) rdata = Wire.read();
    return rdata;
}
//------------------------------------------------------------------------------
// Le funzioni sotto servono per leggere piu' byte (max 30) dalla memoria
// Sono da studiare e per ora le riporto ma non le uso
// WARNING: address is a page address, 6-bit end will wrap around
// also, data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
void i2c_eeprom_write_page( int deviceaddress, unsigned int eeaddresspage, byte* data, byte length ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddresspage >> 8)); // MSB
    Wire.write((int)(eeaddresspage & 0xFF)); // LSB
    byte c;
    for ( c = 0; c < length; c++)
        Wire.write(data[c]);
    Wire.endTransmission();
}
//------------------------------------------------------------------------------
// maybe let's not read more than 30 or 32 bytes at a time!
void i2c_eeprom_read_buffer( int deviceaddress, unsigned int eeaddress, byte *buffer, int length ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,length);
    int c = 0;
    for ( c = 0; c < length; c++ )
        if (Wire.available()) buffer[c] = Wire.read();
}
//------------------------------------------------------------------------------