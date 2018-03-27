// programma di SS
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include <string.h>

#include "PCF8574.h"        // gestione IO per LCD
#include "DS3232RTC.h"      // gestione clock e termometro
#include <EEPROM.h>         // gestione EEPROM
#include "SteveGen.h"       // librerie costruite da Stefano 07 03 18

#define DS3231_I2C_ADDRESS      0x68     // i2c 0x68 del DS3231
#define DS3231_indirizzo        0x68     //    "     "
#define DS3231_TEMPERATURE_ADDR 0x11     // indirizzo della memoria interna
#define DS3231_TEMPERATURE_registro 0x11 //    "     "
#define LCD_PC8574_ADDRESS      0x23     // indirizzo dell'LCD attaccato al PC8574

LiquidCrystal_I2C lcd(LCD_PC8574_ADDRESS,16,2);       // i2c  LCD 16x2  tramite PC8573

uint8_t gradi[8]    = {0x0C,0x0C,0x07,0x08,0x08,0x07,0x00}; // 8 caratteri speciali per lo user (0-7)
uint8_t clock[8]    = {0x00,0x0E,0x15,0x17,0x11,0x0E,0x00};
uint8_t flip[8]     = {0x02,0x1F,0x12,0x00,0x09,0x1F,0x08}; // da testare per simbolo Flip Flop
uint8_t check[8]    = {0x00,0x01,0x03,0x16,0x1C,0x08,0x00};
uint8_t retarrow[8] = {0x01,0x01,0x05,0x09,0x1F,0x08,0x04};
uint8_t uparrow[8]  = {0x00,0x04,0x0E,0x15,0x04,0x04,0x00};
uint8_t dnarrow[8]  = {0x00,0x04,0x04,0x15,0x0E,0x04,0x00};
uint8_t riarrow[8]  = {0x00,0x04,0x02,0x1F,0x02,0x04,0x00};

//-----------------------------------------------------------------------------
void setup() {
    char testo1[] = "Irrigazione V127";     // testo iniziale - versione ----------

    Serial.begin(9600);             // apri il canale Seriale per output
    lcd.init();                     // inizializza  lcd
    pinMode(8, INPUT);              // pin per lettura tasto val 1
    pinMode(9, INPUT);              // pin per lettura tasto val 2
    pinMode(10, INPUT);             // pin per lettura tasto val 4
    pinMode(11, INPUT);             // pin per lettura tasto val 8
    pinMode(12, OUTPUT);            // pin abilitato per verificare il flag timer

    lcd.createChar(0, gradi);       // simbo del grado °C
    lcd.createChar(1, clock);       // simbolo dell'orologio
    lcd.createChar(2, flip);        // simbolo del Flip Flop
    lcd.createChar(3, check);       // simbolo dell'abilitato il disabilitato e' una X maiuscola
    lcd.createChar(4, retarrow);    // freccia return
    lcd.createChar(5, uparrow);     // freccia in su
    lcd.createChar(6, dnarrow);     // freccia in giu
    lcd.createChar(7, riarrow);     // freccia a destra

    setAlarm1(DS3231_indirizzo);    // abilita i bit del DS3231 per ottenere allarme di un secondo

    WriteString(testo1, 0, 0); // serve per vedere la versione
    delay(1000);
    Schermata_zero();
}
//-----------------------------------------------------------------------------
void loop() {                                           // programma principale
    // questa parte e' da sistemare per rilevare i minuti e registrare l'uso delle pompe
    // per info http://www.mauroalfieri.it/elettronica/centralina-irrigazione-arduino.html
    int flag = 0; float gradiC = 0;     // comodo variabile locale
    int Key_Press = 0;          // per tastiera
    Key_Press = LeggeTasti();

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
        lcd.setCursor(0,1);                 // prima posizione linea 2
        lcd.print((char)0);                 // stampa °C
        // verifica se si puo' semplificare la stampaGradi spostando la write dopo!!!!!!!
        gradiC = DS3231_get_treg(DS3231_indirizzo, DS3231_TEMPERATURE_registro);  // legge la temperatura della sonda
        stampaGradi(gradiC, 1, 1);          // su LCD i gradi dal col 1 su linea 1
        lcd.setCursor(7,1);                 // posizione dell' orologio
        lcd.print((char)1);                 // carattere orologio
    }
   // ---...---...---...---...
}
//-----------------------------------------------------------------------------
void MenuPrincipale(void) {             // loop sulle possibili scelte del menu
    int Key_Press = 0;                  // per tastiera
    lcd.clear();
    lcd.setCursor(0,1); lcd.print((char)7);  // posizione del carattere di skip
    lcd.setCursor(2,1); lcd.print("_");      // "  carattere _
    lcd.setCursor(4,1); lcd.print("_");      // "  carattere _
    lcd.setCursor(6,1); lcd.print((char)4);  // "  carattere enter
    //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
    char testo2[] = "Modifica  ora   ";     // visualizza CAMBIO ORA --------------
    WriteString(testo2, 0, 0);          // visualizza su LCD "Modifica  ora   "
    Key_Press = 0;  // reset                                                        sono tutti da togliere?????
    while (Key_Press == 0) { Key_Press = LeggeTasti(); } // in Attesa!
    if (Key_Press == 1) {
        Cambio_OraData(1);      // si va' alla schermata cambio ORA (1)
    }    //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
    char testo4[] = "Modifica  data  ";     // visualizza CAMBIO DATA -------------
    WriteString(testo4, 0, 0);          // visualizza su LCD "Modifica  data  "
    Key_Press = 0;  // reset
    while (Key_Press == 0) { Key_Press = LeggeTasti(); } // in Attesa!
    if (Key_Press == 1) {
        Cambio_OraData(2);      // si va' alla schermata cambio DATA (2)
    }    //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
    char testo5[] = "Ena/Disa pompe  ";     // Ena/Disa pompe ---------------------
    WriteString(testo5, 0, 0);           // visualizza su LCD "Ena/Disa pompa  "
    Key_Press = 0;  // reset
    while (Key_Press == 0) { Key_Press = LeggeTasti(); } // in Attesa!
    if (Key_Press == 1) {
        CambioStatoPompa();     // si va' alla schermata Ena/disa le pompe     ""
    }    //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
    char testo6[] = "Cambia ora pompe";     // Cambia orario pompe start stop -----
    WriteString(testo6, 0, 0);           // visualizza su LCD "Cambia ora pompe"
    Key_Press = 0;  // reset
    while (Key_Press == 0) { Key_Press = LeggeTasti(); } // in Attesa!
    if (Key_Press == 1) {
        CambioOrarioLavoroPompe();
    }    //°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

    // ----------------------------------- ripristina schermata di partenza ---
    Schermata_zero();
}
//-----------------------------------------------------------------------------
void Schermata_zero(void) {     // stampa la schermata iniziale
    lcd.clear();
    char testo1[]  = "Premi un tasto  ";
    WriteString(testo1, 0, 0);      // va alla stampa
    lcd.setCursor(7,1); lcd.print((char)1);   // posizione dell' orologio
    lcd.setCursor(10,1); lcd.print(":"); // due punti orario
    lcd.setCursor(13,1); lcd.print(":"); // "
    WriteTime(8, 1);            // visualizza sulla line 1 l'ora -> HH:MM:SS
}
//-----------------------------------------------------------------------------
void Cambio_OraData(int i) {  // schermata CAMBIO 1 = ORA o 2 = DATA
    int H[20];   // definizione matrice contenitore singoli dati ORA e DATA   (*)
    int X[20]={0,2,9,5,9,5,9,0,0,0,0,3,9,1,9,9,9}; //val max per gli elementi (*)
    int CursPos = 10;                  // posizione cursore alla partenza
    int skip = 10;                     // per utilizzo nella routine, fisso a 10
    int fuori = 0;                     // per utilizzo uscita routine, init a 0
    int delta = 0;                     // serve per distinguere ora da data +10
    int indice = 0;                    // puntatore a H[] e X[]
    int Key_Press = 0;                 // per tastiera
    char testo7[] = "Cambia ->       ";     // per cambio ora e data --------------
    char testoA[] =         "        ";     // su linea 1 per blank su ora/data --
    char testo8[] =           "hhmmss";     // su linea 1 per cambio ora ----------
    char testo9[] =           "ggmmaa";     // su linea 1 per cambio data ---------
    byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
    readDS3231time(DS3231_indirizzo, &second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);

    // trasferimento dati in ORA +++++++++++++(da 1 a 6)   per l'orario Hxx:xx:xx
    if (hour<10) { H[1] = 0; H[2] = hour; }         // trasformazione ore
    else { H[1] = int(hour/10); H[2] = hour - ( H[1] * 10); }

    if (minute<10) { H[3] = 0; H[4] = minute; }     // trasformazione minuti
    else { H[3] = int(minute/10); H[4] = minute - ( H[3] * 10); }

    if (second<10) { H[5] = 0; H[6] = second; }    // trasformazione secondi
    else { H[5] = int(second/10); H[6] = second - ( H[5] * 10); }
    // trasferimento dati in DATA  +++++++++++++(da 11 a 16) per la data Hxx:xx:xx
    if (dayOfMonth<10) { H[11] = 0; H[12] = dayOfMonth; } // trasformazione giorno
    else { H[11] = int(dayOfMonth/10); H[12] = dayOfMonth - ( H[11] * 10); }

    if (month<10) { H[13] = 0; H[14] = month; }            // trasformazione mese
    else { H[13] = int(month/10); H[14] = month - ( H[13] * 10); }

    H[15] = int( year / 10);            // int(18/10) = 1 trasforma anno
    H[16] = year - (H[15] * 10);        // 18-(1*10) = 8
    // fine trasferimento dati++++++++++++++++++++++++++++++++++++++++++++++++++++
    lcd.clear();
    lcd.setCursor(0,1); lcd.print((char)7);  // posizione del carattere di skip
    lcd.setCursor(2,1); lcd.print((char)5);  //  " carattere freccia su
    lcd.setCursor(4,1); lcd.print((char)6);  // "  carattere freccia giu
    lcd.setCursor(6,1); lcd.print((char)4);  // "  carattere enter
    WriteString(testo7, 0, 0);      // scrive CAMBIA ->
    //__________________________________________________________________________
    if (i == 1) {           // stampa parte schermo per cambio ORA  (1)
        delta = 0;          // indica il delta per l' ORA (per data e' =10)
        indice = 1;         // partenza della prima variabile per hhmmss (per data -> 11)
        WriteString(testo8, 10, 1);      // scrive "hhmmss"
    }
    else if  (i == 2) {     // stampa parte schermo per cambio DATA (2)
        delta = 10;         // indica il delta per la DATA (+10 nelle variabili)
        indice = 11;        // partenza della prima variabile per hhmmss (per data -> 11)
        WriteString(testo9, 10, 1);      // scrive "ggmmaa"
    }
    //__________________________________________________________________________
    lcd.setCursor(CursPos, 0);
    for (i=1; i<=7; i++) { lcd.print(H[delta+i]);} // su LCD ora (1) o data (2)

    lcd.blink(); // fa' lampeggiare il cursore alla posizione fissata
    lcd.setCursor(CursPos,0); // posiziona il cursore nella posizione 8, linea 0
                            // corrisponde al puntamento al carattere (valore) a cui fare riferimento
    while (fuori == 0) {
        Key_Press = 0;          // per lettura tastiera fisso
        //-----------------------------------------------------------------
        while (Key_Press == 0) {
            Key_Press = LeggeTasti();  // sono in Attesa!
            delay(250);
            switch (Key_Press) {
                case 1: {       // premuto ok       val=1 ->  salva i dati
                    fuori=99;
                    setDS3231time(DS3231_indirizzo, H[5]*10+H[6], H[3]*10+H[4], H[1]*10+H[2], 1, H[11]*10+H[12],H[13]*10+H[14],H[15]*10+H[16]); // sec min ore giorno_sett giorno mese anno
                    lcd.noBlink();  // via al lampeggio del cursore
                    WriteString(testoA, 10, 1);      // pulisce hhmmss o ggmmaa
                    break;}
                case 2: {       // premuto giu'     val=2 ->  decrementa !!!
                    H[indice] = H[indice] - 1;  // decrementa il valore
                    if (H[indice] < 0) {        // controllo se superato lo zero minimo(-1)
                        H[indice] = X[indice] ;      // se si, rimetto il valore massimo X[skip]
                    }
                    lcd.print(H[indice]);           // stampo alla posizione il valore
                    lcd.setCursor(skip, 0);    // posizione il cursore nel punto giusto
                    break;}
                case 4: {       // premuto su'      val=4 ->  incrementa !!!
                    H[indice] = H[indice] + 1;  // incrementa il valore
                    if (H[indice] > X[indice]) {    // controllo se superato il massimo
                        H[indice] = 0;           // se si, rimetto il valore minimo (0)
                    }
                    lcd.print(H[indice]);     // stampo alla posizione il valore H[x]
                    lcd.setCursor(skip, 0); // ri posizione il cursore indietro di una casella
                    break;}
                case 8: {       // premuto skip     val=8 ->  salta a destra                OK!!!
                    skip = skip + 1;        // a destra di un carattere
                    indice = indice + 1;    // aumenta per la cifra successiva
                    if (skip >= 16) {
                        indice = 1;         // da 6 si passa a 1
                        skip = 10;}         // riposiziona se supera
                    lcd.setCursor(skip, 0); // per reset del cursore
                    break;}
                default: {      // premuto diverso da 1 2 4 8
                    break; }
            }
        }
    }               // fine del while con fuori=0  se valorizzato diverso da zero
    // (*) Spiegazione : si passano alle relative routine: l'indice del valore massimo
    //  che si puo' raggiungere (il minimo e' sempre 0). La sub-routine controlla
    //  l'aumento e restituisce il valore corretto. Poi mette su LCD nel posto corretto
    //  Alla fine salva dul DS i valori corretti
}   // fine della routine
//-----------------------------------------------------------------------------
void CambioStatoPompa(void) {
    int P[4]={0,0,0,0}; // valore per P[0] P[1] P[2] P[3]   P[0] non usato per ora
    char testo0[] = "Ena/Disa Nr.    ";     // per cambio stato pompe ------------
    char testo1[] =         "rele 123";     // per testo su linea 1 --------------
    char testoA[] =         "        ";     // su linea 1 per blank su ora/data --
    int fuori = 0;                     // per utilizzo uscita routine, init a 0
    //char testo7[] = "Cambia ->       ";   // per cambio ora e data -------------
    int Key_Press = 0;           // per tastiera
    int skip = 13;               // per utilizzo nella routine da pos 13
    int indice = 1;              // puntatore a P[]

    P[1] = LeggeEEprom (0x10);    // salva in p1/p2/p3 lo stato  della pompa 1
    P[2] = LeggeEEprom (0x20);    // che legge nella EEPROM            pompa 2
    P[3] = LeggeEEprom (0x30);    // alle locazioni 0x10/20/30         pompa 3

    lcd.clear();
    lcd.setCursor(0,1); lcd.print((char)7);  // posizione del carattere di skip
    lcd.setCursor(2,1); lcd.print((char)2);  //  " carattere Flip/Flop pompe
    lcd.setCursor(4,1); lcd.print("_");      // "  carattere _
    lcd.setCursor(6,1); lcd.print((char)4);  // "  carattere enter

    WriteString(testo0, 0, 0);      // scrive CAMBIA stato ena disa pompe
    WriteString(testo1, 8, 1);

    lcd.setCursor(13,0); // posiziono simboli stato abilitazione pompe
    if (P[1] == 0) {lcd.print("x");} // controllo valore p1
    else {lcd.print((char)3);}
    if (P[2] == 0) {lcd.print("x");} // controllo valore p2
    else {lcd.print((char)3);}
    if (P[3] == 0) {lcd.print("x");} // controllo valore p3
    else {lcd.print((char)3);}

    lcd.setCursor(13,0); // posiziona il cursore sotto il simbolo pompa 1
    lcd.blink(); // fa' lampeggiare il cursore alla posizione fissata
                 // corrisponde al puntamento al carattere (valore) a cui fare riferimento
    while (fuori == 0) {
        Key_Press = 0;          // per lettura tastiera fisso

        while (Key_Press == 0) {
            Key_Press = LeggeTasti();  // sono in Attesa!
            delay(250);
            switch (Key_Press) {
                case 1: {       // premuto ok   val=1 ->   salva valori ed esci
                    ScriveEEprom ( P[1], 0x10);
                    ScriveEEprom ( P[2], 0x20);
                    ScriveEEprom ( P[3], 0x30);
                    fuori=99;
                    break;}
                case 4: {       // premuto FlipFlop       val=4 ->  incrementa
                    if (P[indice] == 0) {       // controllo se il valore e' 0
                        P[indice] = 1;          // altrimenti FlipFlop
                    }
                    else { P[indice] = 0; }
                    if (P[indice] == 0) {lcd.print("x");} // controllo valore p1
                    else {lcd.print((char)3);}
                    lcd.setCursor(skip, 0); // ri posizione il cursore indietro di una casella
                    break;}
                case 8: {       // premuto skip     val=8 ->  salta a destra                OK!!!
                    skip = skip + 1;        // a destra di un carattere
                    indice = indice + 1;    // aumenta per la cifra successiva
                    if (skip >= 16) {
                        indice = 1;         // da 4 si passa a 1
                        skip = 13;}         // riposiziona se supera
                    lcd.setCursor(skip, 0); // per reset del cursore
                    break;}
                default: {      // premuto diverso da 1 2 4 8
                    break; }
            }
        }
    }               // fine del while con fuori=0  se valorizzato diverso da zero

    lcd.noBlink();  // via al lampeggio del cursore
    WriteString(testoA, 8, 1);        // pulisce la seconda meta' della linea1
    lcd.setCursor(2,1); lcd.print("_");      // "  carattere _
    lcd.setCursor(4,1); lcd.print("_");      // "  carattere _
    return ; // schermata di cambio stato della pompa ena/disa
}
//-----------------------------------------------------------------------------
void CambioOrarioLavoroPompe(void) {
    int fuori = 0;                     // per utilizzo uscita routine, init a 0
    int indice = 1;                    // puntatore a H[] e X[]
    int Key_Press = 0;                 // per tastiera
    char testoA[] =         "        ";     // su linea 1 per blank su ora/data --
    int P1[13]={0,0,0,0,0,0,0,0,0,0,0,0,0}; //val ora start stop P1 6+6 val
    int P2[13]={0,0,0,0,0,0,0,0,0,0,0,0,0}; //val ora start stop P2 6+6 val
    int P3[13]={0,0,0,0,0,0,0,0,0,0,0,0,0}; //val ora start stop P3 6+6 val

    for (int i=0x11; i <= 0x1C; i++) {  // legge le EEprom e popola gli array
        P1[indice] = LeggeEEprom(i);          // pompa 1
        P2[indice] = LeggeEEprom(i + 0x10);   // pompa 2
        P3[indice] = LeggeEEprom(i + 0x20);   // pompa 3
        indice = indice + 1;    // incrementa indice di +1
    }

    lcd.clear();
    lcd.setCursor(0,1); lcd.print("1");  // posizione per tasto pompa 1
    lcd.setCursor(2,1); lcd.print("2");  // posizione per tasto pompa 2
    lcd.setCursor(4,1); lcd.print("3");  // posizione per tasto pompa 3
    lcd.setCursor(6,1); lcd.print((char)4);  // posizione del carattere di enter

    char testo7[] = "Cambia ora pompa";     // per cambio orario SS pompe ------
    WriteString(testo7, 0, 0);              // scrive CAMBIA ORA POMPA

    while (fuori == 0) {        // attesa selezione pompa da modificare
        Key_Press = 0;          // per lettura tastiera fisso
        //-----------------------------------------------------------------
        while (Key_Press == 0) {
            Key_Press = LeggeTasti();  // sono in Attesa!
            delay(250);
            switch (Key_Press) {
                case 1: {       // premuto ok       val=1 ->  salva i dati
                    fuori=99;
                    lcd.noBlink();  // via al lampeggio del cursore
                    WriteString(testoA, 10, 1);      // pulisce hhmmss o ggmmaa
                    break;}
                case 2: {       // premuto [3]     val=2 ->  pompa 3
                    CambiaOraSSPompe(3,P1, P2, P3);
                    fuori=99;
                    break;}
                case 4: {       // premuto [2]     val=4 ->  pompa 2
                    CambiaOraSSPompe(2,P1, P2, P3);
                    fuori=99;
                    break;}
                case 8: {       // premuto [1]     val=8 ->  pompa 1
                    CambiaOraSSPompe(1,P1, P2, P3); // passa nr.pompa e tutti gli array
                    fuori=99;
                    break;}
                default: {      // premuto diverso da 1 2 4 8
                    break; }
            }
        }
    }               // fine del while con fuori=0  se valorizzato diverso da zero
}
//-----------------------------------------------------------------------------
void WriteString(char testo[], int col, int linea)  {
    // riceve il testo e la col da cui partire. Linea 0 o 1
    //int i = 0;        // comodo
    //int lung = 0; // comodo
    int lung = strlen(testo)-1;   // calcola la lunghezza del testo e sottrae il \0
    for (int i=0; i <= lung; i++) { // loop per scrivere
        lcd.setCursor(i + col , linea); // posiziona il cursore
        lcd.print(testo[i]);            // scrive il testo
    }
}
//-----------------------------------------------------------------------------
void WriteTime(int col, int linea) {    // a video lcd i valori dell'orario
    // scrive HH:MM:SS alla colonna col e sulla linea 0 o 1
    byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
    // retrieve data from DS3231
    readDS3231time(DS3231_I2C_ADDRESS, &second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
    // - - - - - - - - - - stampa le ore - - - - - - - - - - - - -
    lcd.setCursor(col, linea);
    if (hour<10) {
        lcd.print("0"); lcd.print(hour);
    }
    else { lcd.print(int(hour/10)); lcd.print(hour-(int(hour/10)*10)); }
    // - - - - - - - - - - stampa i minuti - - - - - - - - - - - -
    lcd.setCursor(col+3, linea);
    if (minute<10) {
        lcd.print("0"); lcd.print(minute);
    }
    else { lcd.print(int(minute/10)); lcd.print(minute-(int(minute/10)*10));}
    // - - - - - - - - - - stampa i secondi  - - - - - - - - - - -
    lcd.setCursor(col+6, linea);
    if (second<10) {
        lcd.print("0"); lcd.print(second);
    }
    else { lcd.print(int(second/10)); lcd.print(second-(int(second/10)*10)); }
}
//-----------------------------------------------------------------------------
void CambiaOraSSPompe(int pompa, int P1[], int P2[], int P3[]) { // riceve pompa e array

    int i = 0;              // variabile generica
    int skip = 10;          // indica la posizione del cursore sull'LCD
    char testoP[] = "Start Px        ";// per cambio ora e data --------------
    char testoS[] = "Stop  Px        ";// per cambio ora e data --------------
    int fuori = 0;                     // per utilizzo uscita routine, init a 0
    int Key_Press = 0;                 // per tastiera
    int XX[13]={0,0,0,0,0,0,0,0,0,0,0,0,0}; // array di comodo
    int X[13]={0,2,9,5,9,5,9,2,9,5,9,5,9}; //valori max per gli elementi dell'orario
    int indice = 0;

    lcd.clear();
    lcd.setCursor(0,1); lcd.print((char)7);  // posizione del carattere di skip
    lcd.setCursor(2,1); lcd.print((char)5);  //  " carattere freccia su
    lcd.setCursor(4,1); lcd.print((char)6);  // "  carattere freccia giu
    lcd.setCursor(6,1); lcd.print((char)4);  // "  carattere enter
    // a fronte di pompa setta indirizzi e trasferisce array da Px[k] in XX[k]
    if (pompa == 1) { for (i=0; i<13; i++ ) {XX[i] = P1[i];} }        // idxEEprom = 0x10 ;
    if (pompa == 2) { for (i=0; i<13; i++ ) {XX[i] = P2[i];} }        // idxEEprom = 0x20 ;
    if (pompa == 3) { for (i=0; i<13; i++ ) {XX[i] = P3[i];} }        // idxEEprom = 0x30 ;

    //==================================================================== START
    WriteString(testoP, 0, 0);              // scrive Start P                           varia
    lcd.setCursor(7,0); lcd.print(pompa);   // scrive nr. della pompa
    fuori = 0;          // reset del puntatore per uscita
    indice = 1;                    // puntatore a XX[] e X[] da 1 a 6                   varia
    skip = 1 ;      // stampa su LCD hhmmss dello start da pos 10(sotto)                varia
    for (i = 10; i<=15; i++) {lcd.setCursor(i, 0); lcd.print(XX[skip]); skip++;}

    lcd.blink(); // fa' lampeggiare il cursore alla posizione fissata
    skip = 10;              // posiziono il cursore
    lcd.setCursor(skip, 0); // posiziona il cursore nella posizione 10, linea 0

    while (fuori == 0) {
        Key_Press = 0;          // per lettura tastiera fisso
        //-----------------------------------------------------------------
        while (Key_Press == 0) {
            Key_Press = LeggeTasti();  // sono in Attesa!
            delay(250);
            switch (Key_Press) {
                case 1: {       // premuto ok       val=1 ->  salva i dati
                    fuori=99;   // XX da 1 a 6, ho i dati sistemati
                    break;}
                case 2: {       // premuto giu'     val=2 ->  decrementa !!!
                    XX[indice] = XX[indice] - 1;  // decrementa il valore
                    if (XX[indice] < 0) {        // controllo se superato lo zero minimo(-1)
                        XX[indice] = X[indice] ;      // se si, rimetto il valore massimo X[skip]
                    }
                    lcd.print(XX[indice]);           // stampo alla posizione il valore
                    lcd.setCursor(skip, 0);    // posizione il cursore nel punto giusto
                    break;}
                case 4: {       // premuto su'      val=4 ->  incrementa !!!
                    XX[indice] = XX[indice] + 1;  // incrementa il valore
                    if (XX[indice] > X[indice]) {    // controllo se superato il massimo
                        XX[indice] = 0;           // se si, rimetto il valore minimo (0)
                    }
                    lcd.print(XX[indice]);     // stampo alla posizione il valore H[x]
                    lcd.setCursor(skip, 0); // ri posizione il cursore indietro di una casella
                    break;}
                case 8: {       // premuto skip     val=8 ->  salta a destra                OK!!!
                    skip = skip + 1;        // a destra di un carattere
                    indice = indice + 1;    // aumenta per la cifra successiva
                    if (skip >= 16) {
                        indice = 1;         // da 6 si passa a 1
                        skip = 10;}         // riposiziona se supera
                    lcd.setCursor(skip, 0); // per reset del cursore
                    break;}
                default: {      // premuto diverso da 1 2 4 8
                    break; }
            }
        }
    }               // fine del while con fuori=0  se valorizzato diverso da zero
    //====================================================================  STOP
    WriteString(testoS, 0, 0);              // scrive Start P                           varia
    lcd.setCursor(7,0); lcd.print(pompa);   // scrive nr. della pompa
    fuori = 0;          // reset del puntatore per uscita
    indice = 7;                    // puntatore a XX[] e X[]                            varia
    skip = 7 ;      // stampa su LCD hhmmss dello start da pos 7(sotto)                 varia
    for (i = 10; i<=15; i++) {lcd.setCursor(i, 0); lcd.print(XX[skip]); skip++;}

    lcd.blink(); // fa' lampeggiare il cursore alla posizione fissata
    skip = 10;              // posiziono il cursore
    lcd.setCursor(skip, 0); // posiziona il cursore nella posizione 10, linea 0

    while (fuori == 0) {
        Key_Press = 0;          // per lettura tastiera fisso
        //-----------------------------------------------------------------
        while (Key_Press == 0) {
            Key_Press = LeggeTasti();  // sono in Attesa!
            delay(250);
            switch (Key_Press) {
                case 1: {       // premuto ok       val=1 ->  salva i dati
                    fuori=99;   // XX da 1 a 6, ho i dati sistemati
                    break;}
                case 2: {       // premuto giu'     val=2 ->  decrementa !!!
                    XX[indice] = XX[indice] - 1;  // decrementa il valore
                    if (XX[indice] < 0) {        // controllo se superato lo zero minimo(-1)
                        XX[indice] = X[indice] ;      // se si, rimetto il valore massimo X[skip]
                    }
                    lcd.print(XX[indice]);           // stampo alla posizione il valore
                    lcd.setCursor(skip, 0);    // posizione il cursore nel punto giusto
                    break;}
                case 4: {       // premuto su'      val=4 ->  incrementa !!!
                    XX[indice] = XX[indice] + 1;  // incrementa il valore
                    if (XX[indice] > X[indice]) {    // controllo se superato il massimo
                        XX[indice] = 0;           // se si, rimetto il valore minimo (0)
                    }
                    lcd.print(XX[indice]);     // stampo alla posizione il valore H[x]
                    lcd.setCursor(skip, 0); // ri posizione il cursore indietro di una casella
                    break;}
                case 8: {       // premuto skip     val=8 ->  salta a destra                OK!!!
                    skip = skip + 1;        // a destra di un carattere
                    indice = indice + 1;    // aumenta per la cifra successiva
                    if (skip >= 16) {
                        indice = 1;         // da 6 si passa a 1
                        skip = 10;}         // riposiziona se supera
                    lcd.setCursor(skip, 0); // per reset del cursore
                    break;}
                default: {      // premuto diverso da 1 2 4 8
                    break; }
            }
        }
    }               // fine del while con fuori=0  se valorizzato diverso da zero

    // Salva solo i dati modificati nella locazione EEprom corretta
    if (pompa == 1) {   // salva in pompa 1 se pompa=1
        for (i=0; i<13; i++ ) { ScriveEEprom (XX[i], 0x10 + i); } }
    if (pompa == 2) {   // salva in pompa 2 se pompa=2
        for (i=0; i<13; i++ ) { ScriveEEprom (XX[i], 0x20 + i); } }
    if (pompa == 3) {   // salva in pompa 3 se pompa=3
        for (i=0; i<13; i++ ) { ScriveEEprom (XX[i], 0x30 + i); } }

    lcd.clear();

}   // fine routine gestione cambio ora pompe start stop
//-----------------------------------------------------------------------------











    //    Serial.print(" XX[skip] / skip / i "); Serial.print(XX[skip]); Serial.print(" / ");
    //    Serial.print(skip);Serial.print(" / ");Serial.println(i);

/*
    skip=0;
    Serial.println(" prima delle modifiche");
    Serial.println("P1 - P2 - P3");

    for(i=0;i<13;i++) {
        Serial.print(LeggeEEprom(0x10 + skip));Serial.print(" - ");
        Serial.print(LeggeEEprom(0x20 + skip));Serial.print(" - ");
        Serial.println(LeggeEEprom(0x30 + skip));Serial.print(" - ");

        skip=skip+1;
    }
    Serial.println(" ");
    //qui deve salvare i dati ed uscire
*/