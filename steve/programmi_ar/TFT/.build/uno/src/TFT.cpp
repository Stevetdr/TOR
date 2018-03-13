#include <Arduino.h>
#include "Wire.h"
#include "EEPROM.h"
#include "DS3232RTC.h"               // libreria DS3232 per usare il DS3231
void setup();
void loop();
void work();
void ClearCol(int punt_col);
void sensorRead();
void OnDisplay(float temperature, int nibble);
void sensorConfig();
void Init_parte_fissa();
static unsigned short i2cRead16(byte reg, int SENSOR_ADDR);
static void i2cWrite16(byte reg, unsigned short value, int SENSOR_ADDR);
static void i2cWrite8(byte reg, byte value, int SENSOR_ADDR);
void Display(unsigned int n, unsigned int X, unsigned int Y, unsigned int col_on, unsigned int col_off);
void Display_meno(unsigned int X, unsigned int Y, unsigned int col_on, unsigned int col_off);
void Display_piu(unsigned int X, unsigned int Y, unsigned int col_on, unsigned int col_off);
void Lcd_Clear(unsigned int j);
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Rect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c);
void Rectf(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c);
void DisplayTempo(unsigned int n, unsigned int X, unsigned int Y, unsigned int col_on, unsigned int col_off);
void Lcd_Write_Com(unsigned char VH);
void Lcd_Write_Data(unsigned char VH);
void Lcd_Init(void);
void Lcd_Writ_Bus(unsigned char VH);
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void readDS3231time( byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year);
void displayTime();
byte decToBcd(byte val);
byte bcdToDec(byte val);
#line 1 "src/TFT.ino"

//#include "Wire.h"
//#include "EEPROM.h"
//#include "DS3232RTC.h"               // libreria DS3232 per usare il DS3231
//#include "TFT_cmd.h"				 // libreria grafica del display TFT

#define LCD_WR   A1     
#define LCD_RS   A2        
#define LCD_CS   A3       
#define LCD_REST A0       // A4  serve per potere usare la I2C con il pin A4. Spostato sull'LCD

#define BLACK   0x0000    // definizione dei colori
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define DS3231_I2C_ADDRESS          0x68            // per la gestione del DS3132
#define DS3231_TEMPERATURE_ADDR     0x11            // per la gestione del DS3132

/* per le definizioni dei pin del AT30TS75 vedi in fondo*/

bool fDEBUG = true;

//#define THR_VALUE_LOW       19*256
//#define THR_VALUE_HIGH      20*256
// gestione sensori della temperatura
int SENSOR_ADDR;         //SS
int SENSOR_ADDR_out = 0x4b;  //SS
int SENSOR_ADDR_in = 0x48;   //SS sono i 7 bit dell'indirizzo piu', all'inizio uno zero 0 1001 000 ovvero 0100 1000
char Temp_out = 0x00;    // registra la temperatura OUT ultima letta per essere messa in EEprom  -128 +127
char Temp_in  = 0x00;    //    "            "        IN    "              "          "
// gestione timer per grafico temperatura
unsigned char attesa_sec = 0;       // Ogni 5 secondi carica 1, parte da 0
unsigned char attesa_min = 0;       // Ogni minuto carica +1, parte da zero
char valo_int = 0;                  // Valore della temperatura da mettere in EEPROM, temperatura interna
char valo_out = 0;                  //            "                "                     "        esterna
unsigned char mezz_ora = 0;      // Parte a zero e quando arriva a 48 resetta il grafico (+1 ogni mezz'ora)
int punto_x = 0;                    // Contatore per ogni 6 minuti = 1 puntino
int puntatore_g = 51;               // E' il puntatore progressivo dell'asse delle X, parte dall'asse X+1
int puntatore_EE_in = 1;            // E' il puntatore alla locazione di memoria EEPROM per il salvataggio temp IN
int puntatore_EE_ou = 241;          // E' il puntatore alla locazione di memoria EEPROM per il salvataggio temp OUT
// si usano 240 locazioni per IN (da 1 a 240) e 240 per OUT (da 241 a 480) una ogni 6 minuti
// Arduino UNO = 1kb EEPROM storage.
int punt_col = 1;         // puntatore che serve per cancellare la colonna dove si andra' a riscrivere il grafico

/*=============================================================================================================*/
void setup() {           /* Viene eseguito solo all'inizio ====================================================*/
    // le definizioni sotto servono per lavorare sul TFT  
    for(int p=2;p<10;p++)  { pinMode(p,OUTPUT); }   // digitali OUTPUT tutti i pin digitali da 2 a 9
    pinMode(A0,OUTPUT); pinMode(A1,OUTPUT); pinMode(A2,OUTPUT); pinMode(A3,OUTPUT);         // pinMode(A4,OUTPUT);
    digitalWrite(A0, HIGH); digitalWrite(A1, HIGH); digitalWrite(A2, HIGH); digitalWrite(A3, HIGH); // digitalWrite(A4, HIGH);
    // fino a qui le definizioni per TFT
  
    Lcd_Init();             // libreria TFT grafica per inizializzazione
    Lcd_Clear(0x00);        // pulisce il video con sfondo tutto nero 0x00

    Init_parte_fissa();     // costruisce la parte grafica statica a video
	displayTime();			// display delle ore e data in fondo nell'ultimo rettangolo
  
    Wire.begin();           // I2C start-up        
    sensorConfig();         // Configure the temperature sensors
    RTC.setAlarm(ALM2_EVERY_MINUTE , 0, 0, 0, 0);       // set allarme ogni minuto
    RTC.squareWave(SQWAVE_NONE);                        // il pin non genera l'onda quadra altrimenti --> SQWAVE_1024_HZ);  
    RTC.alarmInterrupt(ALARM_2, true);                  // abilita l'allarme 2

    Serial.begin(9600);     // Debug   Serial start-up at 9600bps
}

/*============================================================================================================*/
/*====Poi esegue il loop sotto all'infinito ==================================================================*/
void loop() {                            //  ad ogni [minuto] esegue il programma work()
DisplayTempo(9, 110, 204, GREEN, BLACK);//stevetdr test error      
//    int c; 
if ( RTC.alarm(ALARM_2) == true) {       // has Alarm1 triggered?   yes, act on the alarm      if == true
DisplayTempo(1, 110, 204, RED, BLACK);//stevetdr test error
    work();
DisplayTempo(1, 110, 204, YELLOW, BLACK);//stevetdr test error                            
    displayTime(); //Serial.println("°");  // gestore del tempo, legge ed a ogni minuto ---->
    }
}   // chiuso il loop 
/*============================================================================================================*/
/*============================================================================================================*/
void work() {       // gestione grafica totale del display, lettura temperatura ed altro                          (***)
                    // Qui ci si arriva ogni minuto e si eseguono i vari test
    sensorRead();       /* "leggo i sensori"   e definisco valo_int e valo_out   */

    punto_x = punto_x + 1 ;             // aumenta di uno perche' e' passato un minuto
    attesa_min = attesa_min + 1;        // e' passato un altro minuto
//------------------------------------------------------------------------------------------------------------------------
    if (attesa_min == 30) {             // raggiunto i 30 minuti, e' tempo di pulire la freccia e ricostruire la colonna  (++)
        attesa_min = 0;                 // reset minuti
        mezz_ora = mezz_ora + 1;        // ogni mezz'ora aggiunge 1    
        // sotto - reset della situazione puntatori al raggiungimento delle 24 ore ===========================================**
        if (mezz_ora == 48)  {          // quando il contatore arriva a 48 sono passate 24 ore, reset di tutti i puntatori
            mezz_ora = 0 ;              // reset del contatore
            punto_x = 0 ;               // reset puntatore per i 6 minuti
            Rectf(puntatore_g,196,20,4,BLACK);    // trattino mobile in fondo cancellato
            puntatore_g = 51 ;          // reset del puntatore per posizione su asse X grafico a punti
            puntatore_EE_in = 1;        // reset puntatore alla locazione di memoria EEPROM per il salvataggio temp IN-  REWRITE
            puntatore_EE_ou = 241;      // reset puntatore alla locazione di memoria EEPROM per il salvataggio temp OUT- REWRITE
        } //==================================================================================================================**
    } // fine if attesa_min==30                                                                                           (++)
//------------------------------------------------------------------------------------------------------------------------
    if (punto_x == 6) {     // Serve per il grafico a puntini, ogni 6 minuti faccio il punto delle temperature
        punto_x = 0;        // reset del contatore  

        H_line(puntatore_g, 165-(valo_int * 2),0,WHITE);  // punto temperatura INTERNA       valore moltiplicato per 2 per  
        H_line(puntatore_g, 165-(valo_out * 2),0,RED);    // punto temperatura ESTERNA     avere migliore visione del puntino

        // quando rileva il puntatore_g corretto pulisce la colonna successiva a partire da puntatore_g + 1
        //--------------------------------------------------------------------  
        if (puntatore_g==70 || puntatore_g==90 || puntatore_g==110 || puntatore_g==130 || puntatore_g==150 || puntatore_g==170 || puntatore_g==190 || puntatore_g==210 || puntatore_g==230 || puntatore_g==250 || puntatore_g==270) {
            ClearCol(puntatore_g);  // alla pulizia della colonna successiva
        }
        if (puntatore_g==51) { 
            ClearCol(puntatore_g-1);  // questo if serve per partire o ripartire. il puntatore e' avanti di 1
            Rectf(punt_col+269,196,20,4,BLACK);    // trattino mobile in fondo a destra, cancellato in nero
            H_line(punt_col+269,74,19,BLACK);      // trattino mobile in alto a destra,  cancellato in nero            
        }       
//------------------------------------------------------------------------------------------------------------------------
        puntatore_g = puntatore_g + 1 ; // aumenta il puntatore per il grafico  !!! per la prossima volta
DisplayTempo(2, 110, 204, RED, BLACK);//stevetdr test error
        EEPROM.write(puntatore_EE_in, valo_int);   // mette il valore INT nella locazione aumentata di EEPROM
        EEPROM.write(puntatore_EE_ou, valo_out);   // mette il valore INT nella locazione aumentata di EEPROM 

        puntatore_EE_in = puntatore_EE_in +1; // incrementa puntamento per scrittura su EEPROM
        puntatore_EE_ou = puntatore_EE_ou +1; // incrementa puntamento per scrittura su EEPROM
DisplayTempo(2, 110, 204, YELLOW, BLACK);//stevetdr test error               
    }       
}                                                                                                       //      (***)
  
/*===============================================================================*/
/*===============================================================================*/
/*===============================================================================*/
/*===============================================================================*/
void ClearCol(int punt_col) {   // pulisce la colonna successiva del grafico per riscrivere le temperature prossime
DisplayTempo(3, 110, 204, RED, BLACK);//stevetdr test error
    int c;    // serve solo per il colore

    Rectf(punt_col+1,75,20,120,BLACK);      // pulisce tutta la colonna successiva 20x120 in nero      
    V_line(punt_col+20,75,120,30000);       // traccia asse secondario delle y a +20
    c=45500;H_line(punt_col+1,165,20,c);    // ricostruisce il trattino azzurro dell'asse delle Y
    V_line(punt_col+10,165,0,WHITE); V_line(punt_col+20,165,0,WHITE);   // puntini su asse X bianchi      
    c=30000;H_line(punt_col+1,85,20,c);H_line(punt_col+1,105,20,c);H_line(punt_col+1,125,20,c);   // trattini orizzontali
    H_line(punt_col+1,145,20,c);H_line(punt_col+1,166,20,c);H_line(punt_col+1,185,20,c);  // ! tolto asse asse y principale
    H_line(punt_col,74,19,YELLOW);      // trattino mobile giallo in alto posizionamento
    H_line(punt_col-20,74,19,BLACK);    // trattino mobile giallo precedente, cancellato con nero
    Rectf(punt_col,196,20,4,YELLOW);    // trattino mobile giallo in fondo ricostruito
    Rectf(punt_col-20,196,20,4,BLACK);  // trattino mobile giallo in fondo cancellato -20 con nero
DisplayTempo(3, 110, 204, YELLOW, BLACK);//stevetdr test error    
}
/*===============================================================================*/
/*----------------------------------------------------------------------------------------COMANDI SENSORE TEMPERATURA--*/
void sensorRead() {

DisplayTempo(4, 110, 204, RED, BLACK);//stevetdr test error
    /* 12-bit temperature is read/written using the formula:    (temp / 0.0625) << 4
    *
    * temp = temperature
    * 0.0625 = resolution for 12-bit values    */
float temperature;
int segno;  // bySS
int nibble;
char xEEPROM;         // valore intero della temperatura
    //---------------------------------------------------
temperature = (i2cRead16(0x00, SENSOR_ADDR_in) >> 4);
temperature *= 0.0625f;
if (temperature >= 0) { Display_piu(17,28,WHITE,0); Serial.print("segno: +    "); } 
else  { Display_meno(17,28,WHITE,0); Serial.print("segno: -    ");}
nibble = 1;                         // definisce che la temperatura e' quella interna=1
valo_int = (int)temperature ;      // mette nella variabile il valore della temperatura letta
//Serial.print("temperature int: "); Serial.print(temperature);      
//Serial.print("     valo_int: "); Serial.println(valo_int); 
OnDisplay(temperature, nibble); 
//---------------------------------------------------
temperature = (i2cRead16(0x00, SENSOR_ADDR_out) >> 4);
temperature *= 0.0625f;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ bySS   
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ bySS       
//      segno = 0x80 & 0xE5;                                                                                    // bySS
//      Serial.print("segno: "); Serial.println(segno);                                                         // bySS
//      if (segno != 0x80) { Display_piu(156,28,RED,0); Serial.print("segno: +    ");}                          // bySS
//      else  { Display_meno(156,28,RED,0); Serial.print("segno: -    "); }                                     // bySS
//      temperature = ~ (0xE5-0x01);                                                                            // bySS
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ bySS
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ bySS   
if (temperature >= 0) { Display_piu(156,28,RED,0); Serial.print("segno: +    ");}       // bySS original
else  { Display_meno(156,28,RED,0); Serial.print("segno: -    "); }                     // bySS original
nibble = 2;                         // definisce che la temperatura e' quella esterna=2
valo_out = (int)temperature ;      // mette nella variabile il valore della temperatura letta   

OnDisplay(temperature, nibble);
//---------------------------------------------------  
DisplayTempo(4, 110, 204, YELLOW, BLACK);//stevetdr test error  
}
//=conversione e display della temperatura======================================
void OnDisplay(float temperature, int nibble){

int gradi;
int dec_C;  //SS per display
int uni_C;  //SS per display 
int vir_C;  //SS per display    
unsigned int c,dx,dy;     
// ****** 
gradi = (temperature * 10);             // esempio : gradi diventa da 27,5   a   275
dec_C = (gradi - (gradi % 100)) / 100;  // (275 - (75)) /100  = 2
gradi = gradi - (dec_C * 100);          // 275 - (2 * 100) = 75
uni_C = (gradi - (gradi % 10)) / 10;    // (75 - (5)) /10     = 7
gradi = gradi - (uni_C * 10);           // 75 -(7 * 10)
vir_C = gradi;                          // 5 = 5
 
if (nibble == 1) { c=WHITE;dx=50 ;dy=15; }      // display temperatura IN
if (nibble == 2) { c=RED  ;dx=190;dy=15;  }      // display temperatura OUT

if(dec_C == 1) {Display(1,dx,dy,c,0); }       // 
if(dec_C == 2) {Display(2,dx,dy,c,0); }       // 
if(dec_C == 3) {Display(3,dx,dy,c,0); }       // 
if(dec_C == 4) {Display(4,dx,dy,c,0); }       // 
if(dec_C == 5) {Display(5,dx,dy,c,0); }       // 
if(dec_C == 6) {Display(6,dx,dy,c,0); }       // 
if(dec_C == 7) {Display(7,dx,dy,c,0); }       // 
if(dec_C == 8) {Display(8,dx,dy,c,0); }       // 
if(dec_C == 9) {Display(9,dx,dy,c,0); }       //                                                 
if(dec_C == 0) {Display(0,dx,dy,0,0); }       //  il colore nero si mette solo nelle decine per non visualizzare  

if(uni_C == 1) {Display(1,dx+30,dy,c,0); }    // 
if(uni_C == 2) {Display(2,dx+30,dy,c,0); }    // 
if(uni_C == 3) {Display(3,dx+30,dy,c,0); }    // 
if(uni_C == 4) {Display(4,dx+30,dy,c,0); }    // 
if(uni_C == 5) {Display(5,dx+30,dy,c,0); }    // 
if(uni_C == 6) {Display(6,dx+30,dy,c,0); }    // 
if(uni_C == 7) {Display(7,dx+30,dy,c,0); }    // 
if(uni_C == 8) {Display(8,dx+30,dy,c,0); }    // 
if(uni_C == 9) {Display(9,dx+30,dy,c,0); }    //                                                 
if(uni_C == 0) {Display(0,dx+30,dy,c,0); }    //     

if(vir_C == 1) {Display(1,dx+60,dy,c,0); }    // 
if(vir_C == 2) {Display(2,dx+60,dy,c,0); }    // 
if(vir_C == 3) {Display(3,dx+60,dy,c,0); }    // 
if(vir_C == 4) {Display(4,dx+60,dy,c,0); }    // 
if(vir_C == 5) {Display(5,dx+60,dy,c,0); }    // 
if(vir_C == 6) {Display(6,dx+60,dy,c,0); }    // 
if(vir_C == 7) {Display(7,dx+60,dy,c,0); }    // 
if(vir_C == 8) {Display(8,dx+60,dy,c,0); }    // 
if(vir_C == 9) {Display(9,dx+60,dy,c,0); }    //                                                 
if(vir_C == 0) {Display(0,dx+60,dy,c,0); }    //            
}
//================================================================================
void sensorConfig() {
    SENSOR_ADDR = SENSOR_ADDR_in;  i2cWrite8(0x01, 0x64, SENSOR_ADDR);      // valore in (0x48) in variabile  
    SENSOR_ADDR = SENSOR_ADDR_out; i2cWrite8(0x01, 0x64, SENSOR_ADDR);      // valore out (0x4B) in variabile  
}
//================================================================================
void Init_parte_fissa() {         // su display tutta la parte che deve rimanere sempre fissa

   unsigned int c;

    Rect (0,0,318,238,GREEN);  // x, y, width, high, color (20000 riquadro totale in verde
    Rect (8,10,303,52,BLUE);   // primo riquadro in alto in azzurro 30000      x le temperature
    Rect (8,63,303,137,BLUE);   // secondo riquadro a meta' in azzurro 30000   x grafico    
    Rect (8,201,303,29,BLUE);   // terzo riquadro a scendere in azzurro
    c=WHITE; 
// scrivo "in:"..................................................................
    H_line(19,16,1,c); H_line(19,17,1,c);             // puntino sulla i di in
    V_line(19,20,10,c);V_line(20,20,10,c);              // i della in
    V_line(24,20,10,c);V_line(25,20,10,c);H_line(26,20,1,c);H_line(26,21,1,c);V_line(28,20,10,c);V_line(29,20,10,c);  // n
    V_line(35,21,2,c);V_line(36,21,2,c);V_line(37,21,2,c);        // due punti
    V_line(35,28,2,c);V_line(36,28,2,c);V_line(37,28,2,c);        // due punti    
    V_line(104,52,5,c);V_line(105,52,5,c);V_line(106,52,5,c);   // virgola
// scrivo "out:"..................................................................
    c=RED; 
    V_line(150,20,10,c);V_line(151,20,10,c);V_line(152,20,1,c);V_line(153,20,1,c);      // lettera o
    V_line(154,20,10,c);V_line(155,20,10,c);V_line(152,29,1,c);V_line(153,29,1,c);      // lettera o
    V_line(159,20,10,c);V_line(160,20,10,c);V_line(163,20,10,c);V_line(164,20,10,c);  // lettera u
    V_line(161,29,1,c);V_line(162,29,1,c);                          // lettera u
    V_line(169,14,16,c);V_line(170,14,16,c);H_line(168,16,4,c);H_line(168,17,4,c);H_line(171,29,2,c);H_line(171,30,2,c); // lett t
    V_line(177,21,2,c);V_line(178,21,2,c);V_line(179,21,2,c);               // due punti
    V_line(177,28,2,c);V_line(178,28,2,c);V_line(179,28,2,c);               // due punti       
    V_line(245,52,5,c);V_line(246,52,5,c);V_line(247,52,5,c);             // virgola
// scrivo "°C" ................................................................
   c=BLUE;
    V_line(284,35,4,c);V_line(285,35,4,c);V_line(286,35,4,c);V_line(287,35,4,c);  // °
    H_line(295,35,9,c);H_line(294,36,10,c);   // barra orizzontale alta    scrivo il C
    V_line(294,37,10,c);V_line(295,37,10,c);   // barra verticale
    H_line(294,48,10,c);H_line(295,49,9,c);   // barra orizzontale bassa    
// costruzione dello spazio per il grafico.............................................
    c=45500;  // colore azzurro
    V_line(50,75,120,c); // asse X principale verticale
    H_line(50,165,239,c); // asse Y principale orizzontale
    c=30000; // colore grigio per
//  linee temperatura asse x lunghe orizzontali
    H_line(51,85,239,c);H_line(51,105,239,c);H_line(51,125,239,c);H_line(51,145,239,c);// tolto asse asse y principale 51,165
    H_line(51,166,239,c);H_line(51,185,239,c);
// linee ore asse y lunghe verticali
    V_line(70,75,120,c); V_line(90,75,120,c); V_line(110,75,120,c); V_line(130,75,120,c);
    V_line(150,75,120,c); V_line(170,75,120,c); V_line(190,75,120,c); V_line(210,75,120,c);    
    V_line(230,75,120,c); V_line(250,75,120,c); V_line(270,75,120,c); V_line(290,75,120,c);
    c=WHITE; 
// puntini su asse Y  x variabile e y a 90+75 con lo zero
    V_line(50 ,165,0,c); V_line(60,165, 0,c) ; V_line(70,165,0,c) ; V_line(80,165,0,c) ; V_line(90,165,0,c);
    V_line(100,165,0,c); V_line(110,165,0,c); V_line(120,165,0,c); V_line(130,165,0,c); V_line(140,165,0,c); 
    V_line(150,165,0,c); V_line(160,165,0,c); V_line(170,165,0,c); V_line(180,165,0,c); V_line(190,165,0,c); 
    V_line(200,165,0,c); V_line(210,165,0,c); V_line(220,165,0,c); V_line(230,165,0,c); V_line(240,165,0,c); 
    V_line(250,165,0,c); V_line(260,165,0,c); V_line(270,165,0,c); V_line(280,165,0,c); V_line(290,165,0,c);
// puntini su asse X bianchi
    V_line(50,85,0,c); V_line(50,105,0,c); V_line(50,125,0,c); V_line(50,145,0,c); V_line(50,165,0,c); V_line(50,185,0,c);  
// ....................................................................................  
// scrittura temperature su asse x a sinistra
    c=GREEN; // verde
    V_line(31,83,4,c);H_line(29,85,4,c);                                    // +
    V_line(35,82,4,c);H_line(36,86,3,c);V_line(39,82,7,c);                    // 4
    V_line(42,82,7,c);V_line(46,82,7,c);H_line(43,82,3,c);H_line(43,89,3,c);    // 0

    V_line(31,103,4,c);H_line(29,105,4,c);                                    // +  
    H_line(35,102,4,c);H_line(37,105,2,c);H_line(35,109,4,c);V_line(39,102,7,c);  // 3
    V_line(42,102,7,c);V_line(46,102,7,c);H_line(43,102,3,c);H_line(43,109,3,c);  // 0  

    V_line(31,123,4,c);H_line(29,125,4,c);                                                     // +    
    H_line(35,122,4,c);V_line(39,123,3,c);H_line(35,126,4,c);V_line(35,127,2,c);H_line(36,129,3,c);  // 2
    V_line(42,122,7,c);V_line(46,122,7,c);H_line(43,122,3,c);H_line(43,129,3,c);                   // 0    

    V_line(31,143,4,c);H_line(29,145,4,c);                                      // +    
    V_line(39,142,7,c);                                                     // 1
    V_line(42,142,7,c);V_line(46,142,7,c);H_line(43,142,3,c);H_line(43,149,3,c);    // 0  

    V_line(42,162,7,c);V_line(46,162,7,c);H_line(43,162,3,c);H_line(43,169,3,c);  // 0 centrale     

    H_line(29,185,4,c);                                                       // -    
    V_line(39,182,7,c);                                                       // 1
    V_line(42,182,7,c);V_line(46,182,7,c);H_line(43,182,3,c);H_line(43,189,3,c);    // 0    
// numeri che indicano le ore in cima al grafico in giallo
    c=YELLOW;
    H_line(21,67,2,c);V_line(20,68,2,c);H_line(21,71,2,c);V_line(24,68,2,c);                       // ore
    V_line(27,67,4,c);H_line(28,68,0,c);H_line(29,67,1,c);H_line(31,68,0,c);
    V_line(34,68,2,c);H_line(35,67,2,c);H_line(35,69,2,c);H_line(35,71,2,c);V_line(38,68,1,c);
    H_line(48,65,4,c);H_line(48,71,4,c);V_line(48,66,4,c);V_line(52,66,4,c);                       // ore 0    
    V_line(88,65,3,c);H_line(89,68,2,c);V_line(92,65,6,c);                                 // ore 4
    V_line(128,65,6,c);H_line(129,65,2,c);H_line(129,68,2,c);H_line(129,71,2,c);V_line(132,65,6,c);            // ore 8
    V_line(167,65,6,c);H_line(171,65,4,c);H_line(171,68,4,c);H_line(171,71,4,c);V_line(175,65,3,c);V_line(171,69,1,c); // ore 12 
    V_line(207,65,6,c);V_line(211,65,6,c);H_line(212,65,3,c);H_line(212,68,3,c);H_line(212,71,3,c);V_line(215,69,1,c); // ore 16 
    H_line(245,65,4,c);H_line(245,68,4,c);H_line(245,71,4,c);V_line(249,65,3,c);V_line(245,69,1,c);            // ore 20
    H_line(251,65,4,c);H_line(251,71,4,c);V_line(251,66,4,c);V_line(255,66,4,c);
    H_line(285,65,4,c);H_line(285,68,4,c);H_line(285,71,4,c);V_line(289,65,3,c);V_line(285,69,1,c);            // ore 24
    V_line(291,65,3,c);H_line(292,68,2,c);V_line(295,65,6,c); 

    // metto il trattino giallo che indica il punto di scrittura
    H_line(50,74,19,YELLOW);    // trattino mobile giallo in alto posizionamento
    Rectf(50,196,20,4,YELLOW);  // trattino mobile giallo in fondo posizionamento
    //.................................................................................  
}
/*---------------------------------------------------------------------------------------*/
 // void EEPROM_clean() { for (int i=0; i <= 200; i++) { EEPROM.write(i,0); }} // pulisce EEPROM da locazione 0 alla 200
/*----------------------------------------------------------------------------------------COMANDI CONVERSIONE BYTES--*/
/* This function reads two bytes for the given register */
static unsigned short i2cRead16(byte reg, int SENSOR_ADDR) {
    //unsigned short val; // bySS originale
    signed short val1;  // bySS
    Wire.beginTransmission(SENSOR_ADDR); // manda i primi sette bit
    Wire.write(reg);                    // legge il registro indicato
    Wire.endTransmission(true);
    Wire.requestFrom(SENSOR_ADDR, 2);   // legge i due byte del registro

    val1 = Wire.read() << 8;    // bySS
    val1 |= Wire.read();        // bySS

    return val1;                // bySS
}
/* This function writes two bytes to the given register */
static void i2cWrite16(byte reg, unsigned short value, int SENSOR_ADDR) {
    Wire.beginTransmission(SENSOR_ADDR);
    Wire.write(reg);
    Wire.write(value >> 8);
    Wire.write(value & 0xFF);
    Wire.endTransmission(true);
}
/* This function writes a byte to the given register */
static void i2cWrite8(byte reg, byte value, int SENSOR_ADDR) {
    Wire.beginTransmission(SENSOR_ADDR);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission(true);
}
/*----------------------------------------------------------------------------------------COMANDI COSTRUZIONE CARATTERI--*/
// viene passato il numero da riportare(n),la posizione X e Y, il colore di ON e il colore di off
void Display(unsigned int n, unsigned int X, unsigned int Y, unsigned int col_on, unsigned int col_off) {

    unsigned int ca; unsigned int cb; unsigned int cc; unsigned int cd; unsigned int ce; unsigned int cf; unsigned int cg;   

    if (n == 0) { ca = col_on; cb = col_on; cc = col_on; cd = col_on; ce = col_on; cf = col_on; cg = col_off;}        // 0
    if (n == 1) { ca = col_off; cb = col_on; cc = col_on; cd = col_off; ce = col_off; cf = col_off; cg = col_off;}    // 1
    if (n == 2) { ca = col_on; cb = col_on; cc = col_off; cd = col_on; ce = col_on; cf = col_off; cg = col_on;}       // 2
    if (n == 3) { ca = col_on; cb = col_on; cc = col_on; cd = col_on; ce = col_off; cf = col_off; cg = col_on;}       // 3
    if (n == 4) { ca = col_off; cb = col_on; cc = col_on; cd = col_off; ce = col_off; cf = col_on; cg = col_on;}      // 4
    if (n == 5) { ca = col_on; cb = col_off; cc = col_on; cd = col_on; ce = col_off; cf = col_on; cg = col_on;}       // 5
    if (n == 6) { ca = col_on; cb = col_off; cc = col_on; cd = col_on; ce = col_on; cf = col_on; cg = col_on;}        // 6
    if (n == 7) { ca = col_on; cb = col_on; cc = col_on; cd = col_off; ce = col_off; cf = col_off; cg = col_off;}     // 7
    if (n == 8) { ca = col_on; cb = col_on; cc = col_on; cd = col_on; ce = col_on; cf = col_on; cg = col_on;}         // 8 
    if (n == 9) { ca = col_on; cb = col_on; cc = col_on; cd = col_on; ce = col_off; cf = col_on; cg = col_on;}        // 9
   
    H_line(X+3 ,Y   ,14,ca);H_line(X+3 ,Y+1 ,14,ca);H_line(X+3 ,Y+2 ,14,ca);  // segmento A  
    V_line(X+18,Y+3 ,14,cb);V_line(X+19,Y+3 ,14,cb);V_line(X+20,Y+3 ,14,cb);  // segmento B  
    V_line(X+18,Y+22,14,cc);V_line(X+19,Y+22,14,cc);V_line(X+20,Y+22,14,cc);  // segmento C  
    H_line(X+3 ,Y+37,14,cd);H_line(X+3 ,Y+38,14,cd);H_line(X+3 ,Y+39,14,cd);  // segmento D  
    V_line(X   ,Y+22,14,ce);V_line(X+1 ,Y+22,14,ce);V_line(X+2 ,Y+22,14,ce);  // segmento E  
    V_line(X   ,Y+3 ,14,cf);V_line(X+1 ,Y+3 ,14,cf);V_line(X+2 ,Y+3 ,14,cf);  // segmento F  
    H_line(X+3 ,Y+18,14,cg);H_line(X+3 ,Y+19,14,cg);H_line(X+3 ,Y+20,14,cg);  // segmento G  
} 
/*-----------------------*/
void Display_meno(unsigned int X, unsigned int Y, unsigned int col_on, unsigned int col_off) {
  unsigned int ca = col_off; unsigned int cg = col_on;

  V_line(X+9,Y+12,14,ca);V_line(X+10,Y+12,14,ca);V_line(X+11,Y+12,14,ca);V_line(X+12,Y+12,14,ca); //segmento verticale off
  H_line(X+3 ,Y+18,14,cg);H_line(X+3 ,Y+19,14,cg);H_line(X+3 ,Y+20,14,cg);// segmento G  ON
} 
/*-----------------------*/
void Display_piu(unsigned int X, unsigned int Y, unsigned int col_on, unsigned int col_off) {
  unsigned int ca = col_on; unsigned int cg = col_on;

  V_line(X+9,Y+12,14,ca);V_line(X+10,Y+12,14,ca);V_line(X+11,Y+12,14,ca);V_line(X+12,Y+12,14,ca); //segmento verticale on
  H_line(X+3 ,Y+18,14,cg);H_line(X+3 ,Y+19,14,cg);H_line(X+3 ,Y+20,14,cg);// segmento G  ON
} 
/*----------------------------------------------------------------------------------------COMANDI GRAFICI--*/
//  pulisce il video , ma come?
void Lcd_Clear(unsigned int j) {  
  unsigned int i,m;

  Address_set(0,0,319,239);
  for(i=0;i<320;i++)
  for(m=0;m<240;m++)
  { Lcd_Write_Data(j>>8); Lcd_Write_Data(j); } 
}
/*  comando di costruzione linea orizzontale ---- il valore l deve essere 0 per 1 punto  */
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) { 
  unsigned int i,j;
  l=l+x;
  Address_set(x,y,l,y);
  j=l*2;
  for(i=1;i<=j;i++)
  { Lcd_Write_Data(c>>8); Lcd_Write_Data(c&0xFF); }  /*  c= colore */
}
/*  comando di costruzione linea verticale ---- il valore l deve essere 0 per 1 punto  */
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) { 
  unsigned int i,j;
  l=l+y;  // l diventa il punto finale relativo della Y
  Address_set(x,y,x,l);
  j=l*2;
  for(i=1;i<=j;i++)
  { Lcd_Write_Data(c>>8); Lcd_Write_Data(c&0xFF); }    /*  c= colore */
}
/*  comando di costruzione rettangolo vuoto*/
void Rect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c)
  { H_line(x  , y  , w, c); H_line(x  , y+h, w, c); V_line(x  , y  , h, c); V_line(x+w, y  , h, c); }
/*  comando di costruzione rettangolo pieno*/
void Rectf(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c) {
  unsigned int i;
  for(i=0;i<h;i++)
  { H_line(x  , y  , w, c); H_line(x  , y+i, w, c); }
}
/* --- display dei caratteri dell'ora e data nell'ultimo riquadro in basso----------------------*/
// viene passato il numero da riportare(n),la posizione X e Y, il colore di ON e il colore di off
void DisplayTempo(unsigned int n, unsigned int X, unsigned int Y, unsigned int col_on, unsigned int col_off) {

    unsigned int ca; unsigned int cb; unsigned int cc; unsigned int cd; unsigned int ce; unsigned int cf; unsigned int cg;   

    if (n == 0) { ca = col_on; cb = col_on; cc = col_on; cd = col_on; ce = col_on; cf = col_on; cg = col_off;}        // 0
    if (n == 1) { ca = col_off; cb = col_on; cc = col_on; cd = col_off; ce = col_off; cf = col_off; cg = col_off;}    // 1
    if (n == 2) { ca = col_on; cb = col_on; cc = col_off; cd = col_on; ce = col_on; cf = col_off; cg = col_on;}       // 2
    if (n == 3) { ca = col_on; cb = col_on; cc = col_on; cd = col_on; ce = col_off; cf = col_off; cg = col_on;}       // 3
    if (n == 4) { ca = col_off; cb = col_on; cc = col_on; cd = col_off; ce = col_off; cf = col_on; cg = col_on;}      // 4
    if (n == 5) { ca = col_on; cb = col_off; cc = col_on; cd = col_on; ce = col_off; cf = col_on; cg = col_on;}       // 5
    if (n == 6) { ca = col_on; cb = col_off; cc = col_on; cd = col_on; ce = col_on; cf = col_on; cg = col_on;}        // 6
    if (n == 7) { ca = col_on; cb = col_on; cc = col_on; cd = col_off; ce = col_off; cf = col_off; cg = col_off;}     // 7
    if (n == 8) { ca = col_on; cb = col_on; cc = col_on; cd = col_on; ce = col_on; cf = col_on; cg = col_on;}         // 8 
    if (n == 9) { ca = col_on; cb = col_on; cc = col_on; cd = col_on; ce = col_off; cf = col_on; cg = col_on;}        // 9
   
    H_line(X+2 ,Y   ,7,ca);H_line(X+2 ,Y+1 ,7,ca);  // segmento A  
    V_line(X+10,Y+2 ,7,cb);V_line(X+11,Y+2 ,7,cb);  // segmento B  
    V_line(X+10,Y+12,7,cc);V_line(X+11,Y+12,7,cc);  // segmento C  
    H_line(X+2 ,Y+20,7,cd);H_line(X+2 ,Y+21,7,cd);  // segmento D  
    V_line(X   ,Y+12,7,ce);V_line(X+1 ,Y+12,7,ce);  // segmento E  
    V_line(X   ,Y+2 ,7,cf);V_line(X+1 ,Y+2 ,7,cf);  // segmento F  
    H_line(X+2 ,Y+10,7,cg);H_line(X+2 ,Y+11,7,cg);  // segmento G  
} 
/*-----------------------*/
/*----------------------------------------------------------------------------------------COMANDI TECNICI DISPLAY--*/
void Lcd_Write_Com(unsigned char VH) {   
    digitalWrite(LCD_CS,LOW);
    digitalWrite(LCD_RS,LOW);
    Lcd_Writ_Bus(VH);
    digitalWrite(LCD_CS,HIGH);
    digitalWrite(LCD_RS,HIGH);
}
/*-----------------------*/
void Lcd_Write_Data(unsigned char VH) {
    digitalWrite(LCD_CS,LOW);
    digitalWrite(LCD_RS,HIGH);
    Lcd_Writ_Bus(VH);
    digitalWrite(LCD_CS,HIGH);
    digitalWrite(LCD_RS,HIGH);
} 
/*-----------------------*/
/* INIZIALIZZAIONE del DISPLAY */
void Lcd_Init(void) {
    int c;
    digitalWrite(LCD_REST,HIGH);
    delay(5); 
    digitalWrite(LCD_REST,LOW);
    delay(15);
    digitalWrite(LCD_REST,HIGH);
    delay(15);
    Lcd_Write_Com(0xF0); Lcd_Write_Data(0x5A); Lcd_Write_Data(0x5A);
    Lcd_Write_Com(0xFC); Lcd_Write_Data(0x5A); Lcd_Write_Data(0x5A);
    Lcd_Write_Com(0xFD); Lcd_Write_Data(0x00); Lcd_Write_Data(0x00);
    Lcd_Write_Data(0x10); Lcd_Write_Data(0x14); Lcd_Write_Data(0x12); Lcd_Write_Data(0x00); Lcd_Write_Data(0x04); Lcd_Write_Data(0x48);
    Lcd_Write_Data(0x40); Lcd_Write_Data(0x16); Lcd_Write_Data(0x16);
    Lcd_Write_Com(0x35);
    Lcd_Write_Com(0x36); Lcd_Write_Data(0x28);//0x68
    Lcd_Write_Com(0x3A); Lcd_Write_Data(0x55);//0x55
    Lcd_Write_Com(0xF2); Lcd_Write_Data(0x28); Lcd_Write_Data(0x5B); Lcd_Write_Data(0x7F); Lcd_Write_Data(0x08); Lcd_Write_Data(0x08);
    Lcd_Write_Data(0x00); Lcd_Write_Data(0x00); Lcd_Write_Data(0x15); Lcd_Write_Data(0x48); Lcd_Write_Data(0x04); Lcd_Write_Data(0x07);
    Lcd_Write_Data(0x01); Lcd_Write_Data(0x00); Lcd_Write_Data(0x00); Lcd_Write_Data(0x63); Lcd_Write_Data(0x08); Lcd_Write_Data(0x08);
    Lcd_Write_Com(0xF7); Lcd_Write_Data(0x01); Lcd_Write_Data(0x00); Lcd_Write_Data(0x10); Lcd_Write_Data(0x00); 
    Lcd_Write_Com(0xF8); Lcd_Write_Data(0x33); Lcd_Write_Data(0x00); Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xF6); Lcd_Write_Data(0x01); Lcd_Write_Data(0x01); Lcd_Write_Data(0x07); Lcd_Write_Data(0x00); Lcd_Write_Data(0x01);
    Lcd_Write_Data(0x0C); Lcd_Write_Data(0x03); Lcd_Write_Data(0x0C); Lcd_Write_Data(0x03);
    Lcd_Write_Com(0xF5); Lcd_Write_Data(0x00); Lcd_Write_Data(0x2E); Lcd_Write_Data(0x40); Lcd_Write_Data(0x00); Lcd_Write_Data(0x00);
    Lcd_Write_Data(0x01); Lcd_Write_Data(0x00); Lcd_Write_Data(0x00); Lcd_Write_Data(0x0D); Lcd_Write_Data(0x0D); Lcd_Write_Data(0x00);
    Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xF4); Lcd_Write_Data(0x07); Lcd_Write_Data(0x00); Lcd_Write_Data(0x00); Lcd_Write_Data(0x00); Lcd_Write_Data(0x22);
    Lcd_Write_Data(0x64); Lcd_Write_Data(0x01); Lcd_Write_Data(0x02); Lcd_Write_Data(0x2A); Lcd_Write_Data(0x4D); Lcd_Write_Data(0x06);
    Lcd_Write_Data(0x2A); Lcd_Write_Data(0x00); Lcd_Write_Data(0x06); Lcd_Write_Data(0x00); Lcd_Write_Data(0x00); Lcd_Write_Data(0x00);
    Lcd_Write_Data(0x00); Lcd_Write_Data(0x00); Lcd_Write_Data(0x00); 
    Lcd_Write_Com(0xF3); Lcd_Write_Data(0x01); //GAMMA
    Lcd_Write_Com(0xF9); Lcd_Write_Data(0x04);
    Lcd_Write_Com(0xFA); Lcd_Write_Data(0x0A); Lcd_Write_Data(0x04); Lcd_Write_Data(0x0C); Lcd_Write_Data(0x19); Lcd_Write_Data(0x25);
    Lcd_Write_Data(0x33); Lcd_Write_Data(0x2D); Lcd_Write_Data(0x27); Lcd_Write_Data(0x22); Lcd_Write_Data(0x1E); Lcd_Write_Data(0x1A);
    Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xFB); Lcd_Write_Data(0x0C); Lcd_Write_Data(0x04); Lcd_Write_Data(0x19); Lcd_Write_Data(0x1E); Lcd_Write_Data(0x20);
    Lcd_Write_Data(0x23); Lcd_Write_Data(0x18); Lcd_Write_Data(0x3D); Lcd_Write_Data(0x25); Lcd_Write_Data(0x19); Lcd_Write_Data(0x0B);
    Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xF9); Lcd_Write_Data(0x02);
    Lcd_Write_Com(0xFA); Lcd_Write_Data(0x0A); Lcd_Write_Data(0x04); Lcd_Write_Data(0x0C); Lcd_Write_Data(0x19); Lcd_Write_Data(0x25);
    Lcd_Write_Data(0x33); Lcd_Write_Data(0x2D); Lcd_Write_Data(0x27); Lcd_Write_Data(0x22); Lcd_Write_Data(0x1E); Lcd_Write_Data(0x1A);
    Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xFB); Lcd_Write_Data(0x0C); Lcd_Write_Data(0x04); Lcd_Write_Data(0x19); Lcd_Write_Data(0x1E); Lcd_Write_Data(0x20);
    Lcd_Write_Data(0x23); Lcd_Write_Data(0x18); Lcd_Write_Data(0x3D); Lcd_Write_Data(0x25); Lcd_Write_Data(0x19); Lcd_Write_Data(0x0B);
    Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xF9); Lcd_Write_Data(0x01);
    Lcd_Write_Com(0xFA); Lcd_Write_Data(0x0A); Lcd_Write_Data(0x04); Lcd_Write_Data(0x0C); Lcd_Write_Data(0x19); Lcd_Write_Data(0x25);
    Lcd_Write_Data(0x33); Lcd_Write_Data(0x2D); Lcd_Write_Data(0x27); Lcd_Write_Data(0x22); Lcd_Write_Data(0x1E); Lcd_Write_Data(0x1A);
    Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xFB); Lcd_Write_Data(0x0C); Lcd_Write_Data(0x04); Lcd_Write_Data(0x19); Lcd_Write_Data(0x1E); Lcd_Write_Data(0x20);
    Lcd_Write_Data(0x23); Lcd_Write_Data(0x18); Lcd_Write_Data(0x3D); Lcd_Write_Data(0x25); Lcd_Write_Data(0x19); Lcd_Write_Data(0x0B);
    Lcd_Write_Data(0x00);
    Lcd_Write_Com(0x11); delay(100);
    Lcd_Write_Com(0xF0); Lcd_Write_Data(0xA5); Lcd_Write_Data(0xA5);
    Lcd_Write_Com(0xFC); Lcd_Write_Data(0xA5); Lcd_Write_Data(0xA5);
    Lcd_Write_Com(0x2a); Lcd_Write_Data(0x00); Lcd_Write_Data(0x00); Lcd_Write_Data(0x01); Lcd_Write_Data(0x3f);
    Lcd_Write_Com(0x2b); Lcd_Write_Data(0x00); Lcd_Write_Data(0x00); Lcd_Write_Data(0x00); Lcd_Write_Data(0xef);
    Lcd_Write_Com(0x29);
    Lcd_Write_Com(0x2c);   
} 
/*====================================================*/
void Lcd_Writ_Bus(unsigned char VH) {
    
    unsigned int i,temp,data; 
    data=VH;
    for(i=8;i<=9;i++)
    {
      temp=(data&0x01);
      if(temp)
        digitalWrite(i,HIGH);
      else
        digitalWrite(i,LOW);
      data=data>>1;
    }
    digitalWrite(LCD_WR,LOW); 
    for(i=2;i<=7;i++)
    {
      temp=(data&0x01);
      if(temp)
        digitalWrite(i,HIGH);
      else
        digitalWrite(i,LOW);
      data=data>>1;
    }  
    digitalWrite(LCD_WR,HIGH);
}
/*====================================================*/
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{   Lcd_Write_Com(0x2a); Lcd_Write_Data(x1>>8); Lcd_Write_Data(x1&0xFF); Lcd_Write_Data(x2>>8); Lcd_Write_Data(x2&0xFF);
    Lcd_Write_Com(0x2b); Lcd_Write_Data(y1>>8); Lcd_Write_Data(y1&0xFF); Lcd_Write_Data(y2>>8); Lcd_Write_Data(y2&0xFF);
    Lcd_Write_Com(0x2c);  }        
// --------------------------------------------------------------------------------------------------------------
void readDS3231time( byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year)
{
DisplayTempo(5, 110, 204, RED, BLACK);//stevetdr test error 
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
DisplayTempo(5, 110, 204, WHITE, BLACK);//stevetdr test error  
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7); // attenzione al numero di byte letti  
  // request all bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
DisplayTempo(5, 110, 204, YELLOW, BLACK);//stevetdr test error   
}
// --------------------------------------------------------------------------------------------------------------
void displayTime() {
DisplayTempo(6, 110, 204, RED, BLACK);//stevetdr test error    
    byte second, minute, hour, dayOfWeek, dayOfMonth, month, year; 
    byte ore_dec, ore_uni, minuti_dec, minuti_uni, giorno_dec, giorno_uni, mese_dec, mese_uni, anno_mil, anno_cen, anno_dec, anno_uni;
    readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);  
    // ====================================================== orario
    ore_dec = int(hour / 10); 				DisplayTempo(ore_dec, 	 17, 204, YELLOW, BLACK); // 14/10 1,4  INT(1,4)= 1 ||  9/10 0,9 INT(0,9)=0
    ore_uni = hour - (ore_dec * 10);  		DisplayTempo(ore_uni, 	 32, 204, YELLOW, BLACK); // 14-(1*10) = 4          ||  9-(0*10) = 9
    V_line(49 ,210,2,YELLOW); V_line(50,210,2,YELLOW); V_line(49 ,217,2,YELLOW); V_line(50,217,2,YELLOW);   // puntini tra le ore
    minuti_dec = int(minute / 10);			DisplayTempo(minuti_dec, 55, 204, YELLOW, BLACK); 	
    minuti_uni = minute - (minuti_dec *10); DisplayTempo(minuti_uni, 70, 204, YELLOW, BLACK); 	
    // ====================================================== data
    giorno_dec = int(dayOfMonth / 10);		DisplayTempo(giorno_dec,156, 204, YELLOW, BLACK); 	
    giorno_uni = dayOfMonth - (giorno_dec * 10); DisplayTempo(giorno_uni,171, 204, YELLOW, BLACK);
    H_line(189, 214,5,YELLOW); H_line(189, 215,5,YELLOW);                                                   // trattino
    mese_dec = int(month / 10);				DisplayTempo(mese_dec, 201, 204, YELLOW, BLACK); 	
    mese_uni = month - (mese_dec * 10); 	DisplayTempo(mese_uni, 216, 204, YELLOW, BLACK);
    H_line(234, 214,5,YELLOW); H_line(234, 215,5,YELLOW);                                                   // trattino
    // attenzione le due righe sotto sono da verificare nella parte numero da displayare, 2016 esce come 0016! 
    anno_mil = int(year / 1000);			DisplayTempo(2, 246, 204, YELLOW, BLACK); year = year - (anno_mil * 1000);
    anno_cen = int(year / 100);				DisplayTempo(0, 261, 204, YELLOW, BLACK); year = year - (anno_cen * 100); 
    anno_dec = int(year / 10) ;				DisplayTempo(anno_dec, 276, 204, YELLOW, BLACK) ; year = year - (anno_dec * 10);
    anno_uni = (year);  					DisplayTempo(anno_uni, 291, 204, YELLOW, BLACK);
DisplayTempo(6, 110, 204, YELLOW, BLACK);//stevetdr test error      	
}
//---------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------
byte decToBcd(byte val) {     // Convert normal decimal numbers to binary coded decimal
  return( (val/10*16) + (val%10) ); }
//---------------------------------------------------------------------------------------------------------------
byte bcdToDec(byte val) {     // Convert binary coded decimal to normal decimal numbers
  return( (val/16*10) + (val%16) ); }


// int RGB(int r,int g,int b) { return r << 16 | g << 8 | b; }      // rem il 10 nov 15   non so' a cosa serve
/*====================================================*/
//==============================================================================

 /* * ==========================================================
 * vedi AT30TS75 ed i programmi sviluppati.  Connect the pins in the following way:
 * Arduino UNO          AK-AT30TS75
 * ==================================
 *   AN5                   SCL
 *   AN4                   SDA
 *   Digital 7             ALERT
 *   5V                    VCC
 *   GND                   GND
 * ===========================================================
 */



  /*Serial.print("pointer_EE: "); Serial.println(pointer_EE);
  Serial.print("valo_int  : "); Serial.println(valo_int); 
  Serial.print("valo_out  : "); Serial.println(valo_out);  
  Serial.print("attesa_sec: "); Serial.println(attesa_sec); 
  */
