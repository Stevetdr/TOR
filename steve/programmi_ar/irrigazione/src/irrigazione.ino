
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd1(0x20,16,2);  // A2 A1 A0 = 0 set the LCD address to 0x38 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd2(0x38,16,2);  // A2 A1 A0 = 0 set the LCD address to 0x38 for a 16 chars and 2 line display

int i;
int nRows;
int ledPin = 13;
int BYTE;
uint8_t bell[8] = {0x4,0xe,0xe,0xe,0x1f,0x0,0x4};
uint8_t note[8] = {0x2,0x3,0x2,0xe,0x1e,0xc,0x0};
uint8_t clock[8] = {0x0,0xe,0x15,0x17,0x11,0xe,0x0};
uint8_t heart[8] = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0};
uint8_t duck[8] = {0x0,0xc,0x1d,0xf,0xf,0x6,0x0};
uint8_t check[8] = {0x0,0x1,0x3,0x16,0x1c,0x8,0x0};
uint8_t cross[8] = {0x0,0x1b,0xe,0x4,0xe,0x1b,0x0};
uint8_t retarrow[8] = { 0x1,0x1,0x5,0x9,0x1f,0x8,0x4};

void setup() {
    pinMode(ledPin, OUTPUT);

    lcd1.init();                      // initialize the lcd
    lcd2.init();                      // initialize the lcd
}


void loop() {

    lcd1.clear();
    lcd2.clear();

    lcd1.setCursor(0,0);
    lcd1.print("A");
    lcd2.setCursor(0,0);
    lcd2.print("A");
    //lcd1.print("O mia che lavoro");

    digitalWrite(ledPin, HIGH);
    delay(500);

    lcd1.setCursor(0,1);
    lcd1.print("B");
    lcd2.setCursor(0,1);
    lcd2.print("B");
    //lcd1.print("seconda linea  !");

    digitalWrite(ledPin, LOW);
    delay(2000);


     //======define charset
/*

     lcd.createChar(0, bell);
     lcd.createChar(1, note);
     lcd.createChar(2, clock);
     lcd.createChar(3, heart);
     lcd.createChar(4, duck);
     lcd.createChar(5, check);
     lcd.createChar(6, cross);
     lcd.createChar(7, retarrow);
     lcd.home();

     i = 0;
     lcd.clear();
     while (i<nRows) {
           lcd.setCursor(0,i);
           lcd.print("user:");
           for (int j=0; j<7; j++) {
                 lcd.print(j, BYTE);
           }

           i++;
    }
*/
}