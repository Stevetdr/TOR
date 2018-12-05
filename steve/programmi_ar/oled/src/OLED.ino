
#include <Wire.h>
#include <OzOLED.h>

void setup(){

  OzOled.init();  //initialze SEEED OLED display
  OzOled.printString("Hello World!"); //Print the String

}

void loop(){

}