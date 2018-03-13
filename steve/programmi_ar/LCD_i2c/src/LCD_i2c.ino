
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>
//#include "LiquidCrystal_PCF8574.h"// I2C LCD with PCF8574 driver

//LiquidCrystal_I2C lcd1(0x20,16,2);  // A2 A1 A0 = 0 set the LCD address to 0x38 for a 16 chars and 2 line display
//LiquidCrystal_I2C lcd2(0x38,16,2);  // A2 A1 A0 = 0 set the LCD address to 0x38 for a 16 chars and 2 line display
// indirizzo 0x38 per il cd con fili gialli
//LiquidCrystal_PCF8574 lcd(0x38);    //ss es era a 27
LiquidCrystal_I2C lcd(0x23,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int i;
int col = 0;
int linea = 0;
int ledPin = 13;
int lung;
int BYTE;
char testo[] = "1_3_5_7_9_*";

uint8_t bell[8]     = {0x04,0x0E,0x0E,0x0E,0x1F,0x00,0x04};
uint8_t note[8]     = {0x02,0x03,0x02,0x0E,0x1E,0x0C,0x00};
uint8_t clock[8]    = {0x00,0x0E,0x15,0x17,0x11,0x0E,0x00};
uint8_t heart[8]    = {0x00,0x0A,0x1F,0x1F,0x0E,0x04,0x00};
//uint8_t duck[8]     = {0x00,0x0C,0x1D,0x0F,0x0F,0x06,0x00};
uint8_t check[8]    = {0x00,0x01,0x03,0x16,0x1C,0x08,0x00};
//uint8_t cross[8]    = {0x00,0x1B,0x0E,0x04,0x0E,0x1B,0x00};
uint8_t retarrow[8] = {0x01,0x01,0x05,0x09,0x1F,0x08,0x04};
uint8_t riarrow[8]  = {0x00,0x04,0x02,0x1F,0x02,0x04,0x00};
uint8_t learrow[8]  = {0x00,0x04,0x08,0x1F,0x08,0x04,0x00};


//String msg;

void setup() {
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);

  //lcd.begin(16,2); // init the LCD
    lcd.init();
    lcd.setBacklight(1);
    lcd.home();
    lcd.clear();

    //======define charset
    lcd.createChar(0, bell); // Sends the custom char to lcd
    lcd.createChar(1, note);
    lcd.createChar(2, clock);
    lcd.createChar(3, heart);
    //lcd.createChar(4, duck);
    lcd.createChar(4, check);
    //lcd.createChar(6, cross);
    lcd.createChar(5, retarrow);
    lcd.createChar(6, riarrow);
    lcd.createChar(7, learrow);
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
void loop() {

    lcd.clear();
    col = 2;
    linea = 0;
    WriteString(testo, col, linea);
    digitalWrite(ledPin, HIGH);

    digitalWrite(ledPin, LOW);


    lcd.setCursor(0,1);

    lcd.print((char)0); // Custom char
    lcd.print((char)1);
    lcd.print((char)2);
    lcd.print((char)3);
    lcd.print((char)4);
    lcd.print((char)5);
    lcd.print((char)6);
    lcd.print((char)7);


    delay(2000);

}
//-----------------------------------------------------------------------------
