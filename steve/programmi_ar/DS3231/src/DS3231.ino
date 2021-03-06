/* programmino per gestire regolazione di un DS3231
usando una tastiera 3x4 e visualizzando il risultato a video */

#include "Wire.h"
#include "DS3232RTC.h"
#define DS3231_I2C_ADDRESS          0x68    // indirizzo del dispositivo
#define DS3231_TEMPERATURE_ADDR     0x11    // indirizzo della memoria interna

const int C1 = 2; // prima colonna 			Costanti per collegare la tastiera 3x4 ad arduino
const int C2 = 3; // seconda colonna			I valori identificano i pin di Arduino nano
const int C3 = 4; // terza colonna
const int R1 = 5; // prima riga
const int R2 = 6; // seconda riga
const int R3 = 7; // terza riga
const int R4 = 8; // quarta riga

char sec1 = 0; // secondi 		variabili di comodo per aumentare o diminuire le variabili
char min1 = 0; // minuti
char ore1 = 0; // ore
char gow1 = 0; // giorno della settimana
char gio1 = 0; // giorno
char mes1 = 0; // mese
char ann1 = 0; // anno			fine definizioni di variabili
//byte sopra

void setup() {

// settaggio uscite colonne ===================================================================
  pinMode(C1, OUTPUT);       //prima colonna
  digitalWrite(C1, HIGH);    //setto alto il livello per disabilitare la colonna
  pinMode(C2, OUTPUT);       //seconda colonna
  digitalWrite(C2, HIGH);    //setto alto il livello per disabilitare la colonna
  pinMode(C3, OUTPUT);       //terza colonna
  digitalWrite(C3 , HIGH);   //setto alto il livello per disabilitare la colonna
// settaggio uscite righe  ===== notare attivazione resistenze pull-up==========================
  pinMode(R1, INPUT_PULLUP);
  pinMode(R2, INPUT_PULLUP);
  pinMode(R3, INPUT_PULLUP);
  pinMode(R4, INPUT_PULLUP);
// fine settaggio colonne e righe ==============================================================

Wire.begin();
pinMode(13, OUTPUT);     //led
Serial.begin(9600);
//            ss  mm  hh  nr  gg  mm  aa
//setDS3231time(03, 31, 07, 03, 22, 03, 16);        // <<<<<<<<<<<<-------------------------------- set time

RTC.setAlarm(ALM2_EVERY_MINUTE , 0, 0, 0, 0);
//RTC.alarmInterrupt(ALARM_1, true);
//RTC.alarm(ALARM_1);
RTC.squareWave(SQWAVE_NONE); //SQWAVE_1024_HZ);
delay(2000);
RTC.alarmInterrupt(ALARM_2, true);

  delay(1000);
// popolo le variabili
  byte sec, min, ore, gow, gio, mes, ann;
  readDS3231time(&sec, &min, &ore, &gow, &gio, &mes, &ann);
  sec1=sec;min1=min;ore1=ore;gow1=gow;gio1=gio;mes1=mes;ann1=ann;

}
// --------------------------------------------------------------------------------------------------------------
void loop() {

  int Datoxx ; // dato di ritorno

  Datoxx = 11;			// qui legge la tastiera
  //Datoxx = Leggi_tastiera();      // qui legge la tastiera

if (Datoxx != 99) {					// e' stato premuto un tasto (Datoxx diverso d 99)
    //Serial.println(Datoxx);				// si, tasto premuto

  byte sec, min, ore, gow, gio, mes, ann;
  readDS3231time(&sec, &min, &ore, &gow, &gio, &mes, &ann);
  sec1=sec;min1=min;ore1=ore;gow1=gow;gio1=gio;mes1=mes;ann1=ann;

	switch(Datoxx){
    	case 1:									// decrementa ore 			premendo 1
    	  ore1 = ore1 - 1 ;
    	  if (ore1 == -1) {ore1 = 23;}
    	  setDS3231_gen(2, ore1); displayTime(); Serial.println("     tasto 1 "); break;
    	case 2:									// incrementa ore 			premendo 2
        ore1 = ore1 + 1 ;
        if (ore1 == 24) {ore1 = 00;}
        setDS3231_gen(2, ore1); displayTime(); Serial.println("     tasto 2 "); break;
    	case 3:
        Serial.print("tasto 3"); break;
    	case 4:									// decrementa minuti 		premendo 4
        min1 = min1 - 1 ;
        if (min1 == -1) {min1 = 59;}
        setDS3231_gen(1, min1); displayTime(); Serial.println("     tasto 4 "); break;
    	case 5:									// incrementa minuti		premendo 5
        min1 = min1 + 1 ;
        if (min1 == 60) {min1 = 0;}
        setDS3231_gen(1, min1); displayTime(); Serial.println("     tasto 5 "); break;
    	case 6:
    	  Serial.print("Venerdi'"); break;
      case 7:                 // decrementa secondi    premendo 7
        sec1 = sec1 - 1 ;
        if (sec1 == -1) {sec1 = 59;}
        setDS3231_gen(0, sec1); displayTime(); Serial.println("     tasto 7 "); break;
      case 8:                 // incrementa secondi    premendo 8
        sec1 = sec1 + 1 ;
        if (sec1 == 60) {sec1 = 0;}
        setDS3231_gen(0, sec1); displayTime(); Serial.println("     tasto 8 "); break;
    	case 11:								// visualizza orario 		premendo asterisco
    		displayTime(); Serial.println(" "); break;
		}
}

  delay(40);


/* questo sotto funziona  // controllo allarme **********************************
if ( RTC.alarm(ALARM_2)) {     //has Alarm1 triggered?   yes, act on the alarm      if == true
  displayTime(); Serial.println("******minuto********");
  }
 // else { Serial.print("no alarm     "); displayReg(0x0E);}
// controllo allarme ************************************************************   */

}
// --------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year) {
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0);                    // set next input to start at the seconds register
  Wire.write(decToBcd(second));     // reg 00 set seconds
  Wire.write(decToBcd(minute));     // reg 01 set minutes
  Wire.write(decToBcd(hour));       // reg 02 set hours
  Wire.write(decToBcd(dayOfWeek));  // reg 03 set day of week (1=domenica, 2=lunedi, 3=martedi, 4=mercoledi, 5=giovedi, 6=venerdi, 7=Sabato)
  Wire.write(decToBcd(dayOfMonth)); // reg 04 set date (1 to 31)
  Wire.write(decToBcd(month));      // reg 05 set month
  Wire.write(decToBcd(year));       // reg 06 set year (0 to 99)
  //                                // reg 07 alm1
  //                                // reg 08 alm1a
  //                                // reg 09 alm1b
  //                                // reg 0A alm1c
  //                                // reg 0B alm2
  //                                // reg 0C alm2a
  //                                // reg 0D alm2b
  //                                // reg 0E control
  //                                // reg 0F control/status
  //                                // reg 10 aging ??
  //                                // reg 11  MSB of temperature
  //                                // reg 12  LSB of temperature

  Wire.endTransmission();
}
// --------------------------------------------------------------------------------------------------------------
void setDS3231_gen(byte reg, byte val1) {   // setDS3231_gen(X1, X2) setta il registro X1 con il valore X2
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(reg);                // set next input to start at the seconds register
  Wire.write(decToBcd(val1));     // setting
  Wire.endTransmission();
}
// --------------------------------------------------------------------------------------------------------------
void readDS3231time( byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year) {
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7); // attenzione al numero di byte letti  era 16???
  // request all bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}
// --------------------------------------------------------------------------------------------------------------
void displayTime() {
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  // ====================================================== orario
  Serial.print(" ore ");
  Serial.print(hour, DEC);  // send it to the serial monitor

  Serial.print(":");        // convert the byte variable to a decimal number when displayed
  if (minute<10) { Serial.print("0"); }

  Serial.print(minute, DEC);
  Serial.print(":");
  if (second<10) { Serial.print("0"); }

  Serial.print(second, DEC);
  // ====================================================== data
  Serial.print(" - del ");
  if (dayOfMonth<10) {Serial.print("0"); Serial.print(dayOfMonth, DEC);} // per scrivere il giorno 1 come 01
    else  {Serial.print(dayOfMonth, DEC);}
 // Serial.print(dayOfMonth, DEC);        // DEC presenta il valore come numero base 10
  Serial.print("/");
  if (month<10) {Serial.print("0"); Serial.print(month, DEC);}           // per scrivere il mese 1 come 01
    else  {Serial.print(month, DEC);}
  Serial.print("/");
  Serial.print(year, DEC);
    // ====================================================== giorno della settimana
  Serial.print(" -  Oggi e' ");
  switch(dayOfWeek){
    case 1:
      Serial.print("Domenica"); break;
    case 2:
      Serial.print("Lunedi'"); break;
    case 3:
      Serial.print("Martedi'"); break;
    case 4:
      Serial.print("Mercoledi'"); break;
    case 5:
      Serial.print("Giovedi'"); break;
    case 6:
      Serial.print("Venerdi'"); break;
    case 7:
      Serial.print("Sabato"); break;
  }
}
//---------------------------------------------------------------------------------------------------------------
void displayReg(byte Registro) {

    uint8_t Reg_A, Reg_A1;   // uint8_t equivale a BYTE

    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(Registro);
    Wire.endTransmission();

    Wire.requestFrom(DS3231_I2C_ADDRESS, 2);   // legge 2 byte. Registro e Registro+1
    	Reg_A = decToBcd(Wire.read());
    	Reg_A1= decToBcd(Wire.read());

    Serial.println("");
    Serial.print("Registro "); Serial.print(Registro, HEX);Serial.print(" : "); Serial.print(Reg_A, HEX);
    Serial.print("       Registro "); Serial.print(Registro+1, HEX);Serial.print(" : ");Serial.println(Reg_A1, HEX);
}
//---------------------------------------------------------------------------------------------------------------
float DS3231_get_treg() {
  float rv;

    uint8_t temp_msb, temp_lsb;   // uint8_t equivale a BYTE
    int8_t nint;

    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(DS3231_TEMPERATURE_ADDR);
    Wire.endTransmission();

    Wire.requestFrom(DS3231_I2C_ADDRESS, 2);
    temp_msb = Wire.read();           // legge il valore MSB
    temp_lsb = Wire.read() >> 6;      // legge il valore LSB e lo shifta a destra di 6 caratteri

    if ((temp_msb & 0x80) != 0)       // testa se il primo bit(7) e' diverso da 1 ovvero e' negativo
        nint = temp_msb | ~((1 << 8) - 1);      // se negativo fa' il complemento a 2
    else
        nint = temp_msb;                        // se positivo lo prende cosi' come e'

    rv = nint + 0.25 * temp_lsb;                // somma MSB e LSB

    return rv;
}
//---------------------------------------------------------------------------------------------------------------
/*
 void display_alm1() {

    uint8_t Reg_07, Reg_08, Reg_09, Reg_0A, Reg_0E, Reg_0F;    // uint8_t equivale a BYTE    legge i 6 byte

    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0x07); // primo registro di ALM1
    Wire.endTransmission();

    Wire.requestFrom(DS3231_I2C_ADDRESS, 6);
    Serial.print("    Reg_07: "); Serial.print( Wire.read());
    Serial.print("    Reg_08: "); Serial.print( Wire.read());
    Serial.print("    Reg_09: "); Serial.print( Wire.read());
    Serial.print("    Reg_0A: "); Serial.print( Wire.read());
    Serial.print("    Reg_0E: "); Serial.print( Wire.read());
    Serial.print("    Reg_0F: "); Serial.println( Wire.read());
}
*/
//---------------------------------------------------------------------------------------------------------------
byte decToBcd(byte val) {     // Convert normal decimal numbers to binary coded decimal
  return( (val/10*16) + (val%10) ); }
//---------------------------------------------------------------------------------------------------------------
byte bcdToDec(byte val) {     // Convert binary coded decimal to normal decimal numbers
  return( (val/16*10) + (val%16) ); }
//====================================================================================
int Leggi_tastiera(void) {
  int Dato = 99;
//**************************************************************************************
  digitalWrite(C1, LOW);                     //Seleziono la prima colonna

  if(digitalRead(R1) == LOW) { Dato=1;  while(digitalRead(R1) == LOW); } //primo pulsante prima riga ?
  if(digitalRead(R2) == LOW) { Dato=4;  while(digitalRead(R2) == LOW); } //secondo pulsante prima riga ?
  if(digitalRead(R3) == LOW) { Dato=7;  while(digitalRead(R3) == LOW); } //terzo pulsante  prima riga ?
  if(digitalRead(R4) == LOW) { Dato=11; while(digitalRead(R4) == LOW); } //quarto pulsante prima riga  ? *

//**************************************************************************************
  digitalWrite(C1, HIGH);                    //Deseleziono la prima colonna
  digitalWrite(C2, LOW);                     //Seleziono la seconda colonna

  if(digitalRead(R1) == LOW) { Dato=2;  while(digitalRead(R1) == LOW); } //primo pulsante seconda riga ?
  if(digitalRead(R2) == LOW) { Dato=5;  while(digitalRead(R2) == LOW); } //secondo pulsante seconda riga ?
  if(digitalRead(R3) == LOW) { Dato=8;  while(digitalRead(R3) == LOW); } //terzo pulsante  seconda riga ?
  if(digitalRead(R4) == LOW) { Dato=0;  while(digitalRead(R4) == LOW); } //quarto pulsante seconda riga  ?

//**************************************************************************************
  digitalWrite(C2, HIGH);                    //Deseleziono la seconda colonna
  digitalWrite(C3, LOW);                     //Seleziono la terza colonna

  if(digitalRead(R1) == LOW) { Dato=3;  while(digitalRead(R1) == LOW); } //primo pulsante seconda riga ?
  if(digitalRead(R2) == LOW) { Dato=6;  while(digitalRead(R2) == LOW); } //secondo pulsante seconda riga ?
  if(digitalRead(R3) == LOW) { Dato=9;  while(digitalRead(R3) == LOW); } //terzo pulsante  seconda riga ?
  if(digitalRead(R4) == LOW) { Dato=12; while(digitalRead(R4) == LOW); } //quarto pulsante seconda riga  ? #

//**************************************************************************************
  digitalWrite(C3, HIGH);                    //Deseleziono la seconda colonna
  return Dato;
}