/*
    http://www.gammon.com.au/forum/?id=11428
*/

// impostazione di un indirizzo fisso in EEPROM. Comodo per Rs-485
#include <EEPROM.h>

const byte myAddress        = 10;
const byte maxAddress       = 20;   // non so' a cosa serva

const bool updateAddress    = true;

void setup () {

    if (updateAddress) {        // scrittura dell'indirizzo su EEPROM(0)
        if (EEPROM.read (0) != myAddress)   // se indirizzo diverso, lavora
            EEPROM.write (0, myAddress);
        // if (EEPROM.read  (1) != maxAddress)
        //     EEPROM.write (1, maxAddress);
    }

    Serial.begin (9600);    // debugging prints

}  // end of setup

void loop () {          // questa parte visualizza solo
// from EEPROM
byte myAddress;        // who we are
byte maxAddress;  // maximum devices on the bus

    // debugging prints
    myAddress = EEPROM.read (0);
    Serial.print (F("My address    is : "));
    Serial.println (int (myAddress));

    maxAddress = EEPROM.read (1);
    Serial.print (F("Max addresses is : "));
    Serial.println (int (maxAddress));

    delay(2000);
}