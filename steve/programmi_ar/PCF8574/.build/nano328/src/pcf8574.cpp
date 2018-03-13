#include <Arduino.h>
#include "PCF8574.h"
#include <Wire.h>
void setup();
void loop();
#line 1 "src/pcf8574.ino"

// PUPROSE: demo 
//

//#include "PCF8574.h"
//#include <Wire.h>

// adjust addresses if needed
PCF8574 PCF_3A(0x3A);  // add switches to lines  (used as input)
PCF8574 PCF_3B(0x3B);  // add leds to lines      (used as output)

void setup() {
  //Serial.begin(115200);
  //Serial.println("\nTEST PCF8574\n");

  uint8_t value = PCF_3A.read8();
  //Serial.print("#38:\t");

}

void loop()
{
  // echos the lines
  uint8_t value = PCF_3A.read8();
 // 	Serial.println(value);
 // PCF_3B.write8(value);


switch (value) {
    case 0xFE:	// premuto 1
    	PCF_3B.write8(0x01);
        for (int i=0; i<7; i++) {PCF_3B.shiftLeft();delay(50);}
      break;
    case 0xFD:	// premuto 2
         	PCF_3B.write8(0x80);
      	for (int i=0; i<7; i++) {PCF_3B.shiftRight();delay(50);}
      break;
    case 0xFB:	// premuto 4   
  		for (int i=0; i<8; i++) {PCF_3B.write(i, 1);delay(50);PCF_3B.write(i, 0);delay(50);}
      break;
    case 0xF7:	// premuto 8      
 		for (int i=0; i<8; i++) {PCF_3B.toggle(i);delay(50);PCF_3B.toggle(i);delay(50);}
      break;    
    default: 
      // if nothing else matches, do the default
      // default is optional  
      	//PCF_3B.write8(0x92); delay(80); PCF_3B.write8(0x24); delay(80);PCF_3B.write8(0x49); delay(80);
    break;
    //PCF_3B.write8(0x00);    
  }



  delay(100);
}
//
// END OF FILE
//