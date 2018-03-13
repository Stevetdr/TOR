// programma di SS
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "PCF8574.h"
#include "DS3232RTC.h"
#include <string.h>
#include <stdio.h>
#include "SteveGen.h"       // librerie costruite da Stefano 07 03 18

#define DS3231_I2C_ADDRESS      0x68    // i2c 0x68 del DS3231
#define DS3231_indirizzo        0x68    //    "     "
#define DS3231_TEMPERATURE_ADDR 0x11    // indirizzo della memoria interna
#define DS3231_TEMPERATURE_registro 0x11 //    "     "

LiquidCrystal_I2C lcd(0x23,16,2);       // i2c 0x38 LCD 16x2
//PCF8574 tastiera(0x25);                 // i2c 0x25 per gestione tastiera con PCF8574

int Key_Press = 0;          // per tastiera
uint8_t value = 0;          // per tastiera
uint8_t value1 = 0;         // per tastiera
int i = 0;
int col = 0;                // per LCD
int linea = 0;              // per LCD
int lung;                // intero senza segno
int flag = 0; float gradiC = 0;

// character from 0 to 7 special to build 5x7 pixel
uint8_t gradi[8]    = {0x0C,0x0C,0x07,0x08,0x08,0x07,0x00};
uint8_t clock[8]    = {0x00,0x0E,0x15,0x17,0x11,0x0E,0x00};
uint8_t heart[8]    = {0x00,0x0A,0x1F,0x1F,0x0E,0x04,0x00};
uint8_t check[8]    = {0x00,0x01,0x03,0x16,0x1C,0x08,0x00};
uint8_t retarrow[8] = {0x01,0x01,0x05,0x09,0x1F,0x08,0x04};
uint8_t uparrow[8]  = {0x00,0x04,0x0E,0x15,0x04,0x04,0x00};
uint8_t dnarrow[8]  = {0x00,0x04,0x04,0x15,0x0E,0x04,0x00};
uint8_t riarrow[8]  = {0x00,0x04,0x02,0x1F,0x02,0x04,0x00};

char testo1[] = "Irrigazione V112";      // testo iniziale - versione
char testo2[] = "Modifica  ora   "; // visualizza CAMBIO ORA --------------
char testo4[] = "Modifica  data  "; // visualizza CAMBIO DATA -------------
char testo5[] = "Ena/Disa rele'  "; // Ena/Disa rele ----------------------
char testo6[] = "Cambia ora pompe"; // Cambia orario pompe start stop -----
char testo7[] = "Cambia ->       "; // per cambio ora e data --------------
char testo8[] =           "hhmmss"; // su linea 1 per cambio ora ----------
char testo9[] =           "ggmmaa"; // su linea 1 per cambio data ---------

//-----------------------------------------------------------------------------
void setup() {
    Serial.begin(9600);             // apri il canale Seriale per output
    lcd.init();                     // inizializza  lcd
    pinMode(8, INPUT);              // pin per lettura tasto val 1
    pinMode(9, INPUT);              // pin per lettura tasto val 2
    pinMode(10, INPUT);             // pin per lettura tasto val 4
    pinMode(11, INPUT);             // pin per lettura tasto val 8
    pinMode(12, OUTPUT);            // pin abilitato per verificare il flag timer
    //setDS3231time(30,33,18,1,26,02,19); // sec min ore giorno_sett day month year
    //timeSS(DS3231_indirizzo); // solo per settare un campo -da migliorare

    lcd.createChar(0, gradi);       // Spedisce i caratteri utente all'LCD
    lcd.createChar(1, clock);
    lcd.createChar(2, heart);
    lcd.createChar(3, check);
    lcd.createChar(4, retarrow);    // freccia return
    lcd.createChar(5, uparrow);     // freccia in su
    lcd.createChar(6, dnarrow);     // freccia in giu
    lcd.createChar(7, riarrow);     // freccia a destra

    setAlarm1(DS3231_indirizzo);    // abilita i bit del DS3231 per ottenere allarme di un secondo

    WriteString(testo1, 0, 0); // serve per vedere la versione
    delay(1000);

    schermata_zero();
}
//-----------------------------------------------------------------------------
void loop() {                                           // programma principale

    int Key_Press = LeggeTasti();
    if (Key_Press != 0) {       // se ritorna 0, continua il loop altrimenti
        MenuPrincipale();       // vai al menu principale per le selezioni
    }

    WriteTime(8, 1);            // visualizza sulla line 1 l'ora -> HH:MM:SS
    delay(250);                 // delay

    flag = CheckAlarm(DS3231_indirizzo);        // se flag=0, allarme (di 1 secondo) non scattato
    if (flag == 1) {            // se flag=1, allarme scattato, da resettare
        //Serial.println(" FLAG =1 ");
        digitalWrite(12, HIGH);                                  // LED acceso
        flag = 0;                           // reset del flag
        clearA1F(DS3231_indirizzo);         // reset del flag del DS3231
        //displayTime(DS3231_indirizzo);    // stampa su Serial, ora e data
        delay(200);                         // delay
        digitalWrite(12, LOW);                                  // LED spento
        gradiC = DS3231_get_treg(DS3231_indirizzo, DS3231_TEMPERATURE_registro);  // legge la temperatura della sonda
        lcd.setCursor(0,1);                 // prima posizione linea 2
        lcd.print((char)0);                 // stampa °C
        stampaGradi(gradiC, col, linea);    // su LCD i gradi dal col su linea
        lcd.setCursor(7,1);                 // posizione dell' orologio
        lcd.print((char)1);                 // carattere orologio
    }
   // int tempC = DS3231_get_treg(DS3231_I2C_ADDRESS, DS3231_TEMPERATURE_ADDR);  // Reads the temperature as an int, to save memory
   // float tempC = DS3231_get_treg(DS3231_I2C_ADDRESS, DS3231_TEMPERATURE_ADDR);
}
//-----------------------------------------------------------------------------
void MenuPrincipale(void) { // loop sulle possibili scelte del menu

    lcd.clear();
    lcd.setCursor(0,1); lcd.print((char)7);  // posizione del carattere di skip
    lcd.setCursor(6,1); lcd.print((char)4);  // "  carattere enter
    //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
    WriteString(testo2, 0, 0);          // scrivo su LCD
    Key_Press = 0;  // reset
    while (Key_Press == 0) { Key_Press = LeggeTasti(); } // in Attesa!
    if (Key_Press == 1) {
        Serial.println("premuto skip1");
        schermata_cambio_OraData(1); // si va' alla schermata cambio ORA
    }    //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
    //char testo4[] = "Modifica  data  "; // visualizza CAMBIO DATA -------------
    WriteString(testo4, 0, 0);
    Key_Press = 0;  // reset
    while (Key_Press == 0) { Key_Press = LeggeTasti(); } // in Attesa!
    if (Key_Press == 1) {
        Serial.println("premuto skip2");
        schermata_cambio_OraData(2); // si va' alla schermata cambio DATA
        // andare alla gestione del cambio ora
    }    //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
    //char testo5[] = "Ena/Disa rele'  "; // Ena/Disa rele ----------------------
    WriteString(testo5, 0, 0); // testo5
    Key_Press = 0;  // reset
    while (Key_Press == 0) { Key_Press = LeggeTasti(); } // in Attesa!
    if (Key_Press == 1) {
        Serial.println("premuto skip3");
        // andare alla gestione del cambio ora
    }    //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
    //char testo6[] = "Cambia ora pompe"; // Cambia orario pompe start stop -----
    WriteString(testo6, 0, 0);
    Key_Press = 0;  // reset
    while (Key_Press == 0) { Key_Press = LeggeTasti(); } // in Attesa!
    if (Key_Press == 1) {
        Serial.println("premuto skip4");
        // andare alla gestione del cambio ora
    }    //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
    // ----------------------------------- ripristina schermata di partenza ---
    schermata_zero();
}
//-----------------------------------------------------------------------------
void schermata_zero(void) {     // stampa la schermata iniziale
    lcd.clear();
    char testo1[]  = "Premi un tasto  ";
    WriteString(testo1, 0, 0);      // va alla stampa
    lcd.setCursor(7,1); lcd.print((char)1);   // posizione dell' orologio
    lcd.setCursor(10,1); lcd.print(":"); // due punti orario
    lcd.setCursor(13,1); lcd.print(":"); // "
    WriteTime(8, 1);            // visualizza sulla line 1 l'ora -> HH:MM:SS
}
//-----------------------------------------------------------------------------
void schermata_cambio_OraData(int i) {  // schermata CAMBIO ORA(1) o DATA(2)
    int H[7];   // definizione dei 6 byte (da 1 a 6) per l'orario H12:34:56
    int D[7];   // definizione dei 6 byte (da 1 a 6) per la data  D12-34-56
    int CursPos = 10;                  // posizione cursore alla partenza
    byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
    readDS3231time(DS3231_indirizzo, &second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);

    lcd.clear();
    lcd.setCursor(0,1); lcd.print((char)7);  // posizione del carattere di skip
    lcd.setCursor(2,1); lcd.print((char)5);  //  " carattere freccia su
    lcd.setCursor(4,1); lcd.print((char)6);  // "  carattere freccia giu
    lcd.setCursor(6,1); lcd.print((char)4);  // "  carattere enter
    WriteString(testo7, 0, 0);      // scrive CAMBIA ->
    // In questa parte si costruisce la schemata base per i cambi

    if (i == 1) {                   // stampa parte schermo per cambio ORA  (1)
        WriteString(testo8, 10, 1);      // scrive "hhmmss"
    // trasferimento dati in ORA ++++++++++++++++++++++++++++++++++++++++++++++
        if (hour<10) { H[1] = 0; H[2] = hour; }         // trasformazione ore
        else { H[1] = int(hour/10); H[2] = hour - ( H[1] * 10); }

        if (minute<10) { H[3] = 0; H[4] = minute; }     // trasformazione minuti
        else { H[3] = int(minute/10); H[4] = minute - ( H[3] * 10); }

        if (second<10) { H[5] = 0; H[6] = second; }    // trasformazione secondi
        else { H[5] = int(second/10); H[6] = second - ( H[5] * 10); }

        lcd.setCursor(CursPos, 0); lcd.print(H[1]); lcd.print(H[2]);
        lcd.print(H[3]); lcd.print(H[4]); lcd.print(H[5]); lcd.print(H[6]);
        lcd.blink(); // fa' lampeggiare il cursore alla posizione fissata
        lcd.setCursor(CursPos,0); // posiziona il cursore nella posizione 8, linea 0

        int fuori=0;
        while (fuori==0) {
            Serial.print("fuori :");Serial.println(fuori);
            Key_Press = 0;
            while (Key_Press == 0) {

                Key_Press = LeggeTasti();  // in Attesa!
                Serial.print("Key_Press : ");Serial.println(Key_Press);
                delay(250);
                switch (Key_Press) {
                    case 8:
                        Serial.println("premuto 8");
                        break;
                    case 4:
                        Serial.println("premuto 4");
                        break;
                    case 2:
                        Serial.println("premuto 2");
                        break;
                    case 1:
                        Serial.println("premuto 1");
                        fuori=99;
                        break;
                    default:
                        Serial.println("premuto Key_press");
                        break;
                }
            }
        }   // fuori




/*
    // gestione tastiera
        Key_Press = 0;  // reset
        while (Key_Press != 1) {    // se e' diverso da 1 deve essere esaminato
            Serial.print("1");
            while (Key_Press == 0) { Key_Press = LeggeTasti(); } // in Attesa!
                Serial.print("2");
                if (Key_Press == 8) {   // premuto tasto a destra
                    Serial.print("3");
                    CursPos = CursPos + 1 ; // a destra di uno
                        if (CursPos == 16) {CursPos = 10;}    // over
                        Serial.print("4");
                        Serial.print(CursPos);
                    lcd.setCursor(CursPos,0);   // sposta il cursore

                }
        }
*/
    } // fine cambio ora ......................................................

    else if  (i == 2) {             // stampa parte schermo per cambio DATA (2)
        // stampa data
        // copia la parte sopra
    } // fine cambio data .....................................................

    // trasferimento dati in DATA               sotto trasferimento in anno
    if (dayOfMonth<10) { D[1] = 0; D[2] = dayOfMonth; } // trasformazione giorno
    else { D[1] = int(dayOfMonth/10); D[2] = dayOfMonth - ( D[1] * 10); }

    if (month<10) { D[3] = 0; D[4] = month; }            // trasformazione mese
    else { D[3] = int(month/10); D[4] = month - ( D[3] * 10); }

    D[5] = int( decToBcd(year)  / 10);        // int(18/10) = 1 trasforma anno
    D[6] = decToBcd(year) - (D[5] * 10);     // 18-(1*10) = 8
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



    Key_Press = 0;  // reset
    while (Key_Press == 0) { Key_Press = LeggeTasti(); } // in Attesa!
    // se legge 8 fa' lo skip sull'altra cifra
    // se legge 4 decrementa il valore
    // se legge 2 incrementa il valore
    // se legge 1 esce e salva i dati nel DS

// -----------------------------------------******************************
//void displayTime(int DS3231_indirizzo)
//    byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
    // retrieve data from DS3231



// -----------------------------------------*****************************




    delay(3000);
    lcd.noBlink();  // via al lampeggio del cursore
}
//-----------------------------------------------------------------------------
void WriteString(char testo[], int col, int linea)  {
    // riceve il testo e la col da cui partire. Linea 0 o 1
    lung = strlen(testo)-1;   // calcola la lunghezza del testo e sottrae il \0
    for (i=0; i <= lung; i++) { // loop per scrivere
        lcd.setCursor(i + col , linea); // posiziona il cursore
        lcd.print(testo[i]);            // scrive il testo
    }
}
//-----------------------------------------------------------------------------
void WriteTime(int col, int linea) {
    // scrive HH:MM:SS alla colonna col e sulla linea 0 o 1
    byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
    // retrieve data from DS3231
    readDS3231time(DS3231_I2C_ADDRESS, &second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
// - - - - - - - - - - stampa le ore - - - - - - - - - - - - -
    lcd.setCursor(col, linea);
    if (hour<10) {
        lcd.print("0");
        lcd.print(hour);
    }
    else {
        lcd.print(int(hour/10));
        lcd.print(hour-(int(hour/10)*10));
    }
// - - - - - - - - - - stampa i minuti - - - - - - - - - - - -
    lcd.setCursor(col+3, linea);
    if (minute<10) {
        lcd.print("0");
        lcd.print(minute);
    }
    else {
        lcd.print(int(minute/10));
        lcd.print(minute-(int(minute/10)*10));
    }
// - - - - - - - - - - stampa i secondi  - - - - - - - - - - -
    lcd.setCursor(col+6, linea);
    if (second<10) {
        lcd.print("0");
        lcd.print(second);
    }
    else {
        lcd.print(int(second/10));
        lcd.print(second-(int(second/10)*10));
    }
}
//-----------------------------------------------------------------------------
// stampa temperatura in gradiC del DS sul LCD da colonna su linea
// non si riesce a mettere in SteveGen perche' mi da' errore
void stampaGradi(float gradiC, int col, int linea) {

    // ATTENZIONE: il valore gradiC deve essere tra -32000 e +32000
    //  oltre si sballa tutto. Converte solo 3 decimali
    String stringVal = "";      // reset della stringa

    stringVal = String(int(gradiC)) + "." + String(getDecimali(gradiC));
    char charVal[stringVal.length()+1];

    if (gradiC >= 0) {      // se gradi=>0 mette il + davanti alla cifra da stampare
        stringVal =  "+" + stringVal;
    }

    stringVal.toCharArray(charVal,stringVal.length()-1); // +1

    WriteString(charVal, 1, 1);     // solo questo OK
}       // esce da stampaGradi
// ============================================================================
// questa funzione estrae la parte decimale del float dei gradi
long getDecimali(float val) {
    int intPart = int(val);     // prende la parte intera es. di 317,23 ottiene 317
    long decPart = 1000 * (val - intPart);  // ottengo prima 0,23 poi 230 (moltiplicato per 1000) x 3 decimali
    if (decPart > 0) { return(decPart);} // se c'e' ritorna la parte decimale
    else if(decPart < 0) { return((-1)*decPart);} // se negativo, moltiplica per -1
    else { return(0);}                  //return 0 se non c'e' parte decimale
}
//-----------------------------------------------------------------------------


// non funziona, non sono riuscito a gestire lo switch e il case

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












//=============================================================================
//=============================================================================
//=============================================================================


/*
    Serial.print(" hdec :");Serial.println(H[1]);
    Serial.print(" huni :");Serial.println(H[2]);
    Serial.print(" mdec :");Serial.println(H[3]);
    Serial.print(" muni :");Serial.println(H[4]);
    Serial.print(" sdec :");Serial.println(H[5]);
    Serial.print(" suni :");Serial.println(H[6]);
    Serial.print(" ddec :");Serial.println(D[1]);
    Serial.print(" duni :");Serial.println(D[2]);
    Serial.print(" adec :");Serial.println(D[3]);
    Serial.print(" auni :");Serial.println(D[4]);
    Serial.print(" ydec :");Serial.println(D[5]);
    Serial.print(" yuni :");Serial.println(D[6]);

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