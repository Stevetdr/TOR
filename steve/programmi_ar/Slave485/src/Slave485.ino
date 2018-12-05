/* https://arduino-info.wikispaces.com/SoftwareSerialRS485Example
   - Used with YD_SoftwareSerialExampleRS485_1 on another Arduino
   - Remote: Receive data, loop it back...
   - Connect this unit Pins 10, 11, Gnd
   - To other unit Pins 11,10, Gnd  (Cross over)
   - Pin 3 used for RS485 direction control
   - Pin 13 LED blinks when data is received
*/

#include <SoftwareSerial.h>     /*-----( Import needed libraries )-----*/
/*-----( Declare Constants and Pin Numbers )-----*/
#define SSerialRX        10  //Serial Receive pin       RO
#define SSerialTX        11  //Serial Transmit pin      DI
#define SSerialTxControl 3   //RS485 Direction control  DE+RE

#define RS485Transmit    HIGH
#define RS485Receive     LOW

#define Pin13LED         13  //                         DE+RE

/*-----( Declare objects )-----*/
SoftwareSerial RS485Serial(SSerialRX, SSerialTX); // RX, TX

/*-----( Declare Variables )-----*/
//int byteReceived;
//int byteLetti;
char byteLetti;
/*----------------------------------------------------------------------------*/
void setup() {  /****** SETUP: RUNS ONCE ******/
    // Start the built-in serial port, probably to Serial Monitor
    Serial.begin(9600);                 // apre monitor
    Serial.println("SerialRemote");     // ci scrive

    pinMode(Pin13LED, OUTPUT);          // pin 13 in output
    pinMode(SSerialTxControl, OUTPUT);  // pin  3 in output

    digitalWrite(SSerialTxControl, RS485Receive);  // Init Transceiver

    // Start the software serial port, to another device
    RS485Serial.begin(4800);   // set the data rate
}   //--(end setup )---
/*----------------------------------------------------------------------------*/
void loop() { /****** LOOP: RUNS CONSTANTLY ******/
  //    Copy input data to output
    if (RS485Serial.available()>0) {    // ci sono dati da leggere
        //byteLetti = RS485Serial.readString();     // Read the byte
        //char inChar = (char)Serial.read();
        int inChar = RS485Serial.read();
        //Serial.print("intercettato dato : "); Serial.println (byteLetti);
        Serial.print("intercettato dato : "); Serial.println(inChar);

        digitalWrite(Pin13LED, HIGH);       // Show activity
        delay(50);
        digitalWrite(Pin13LED, LOW);

        //digitalWrite(SSerialTxControl, RS485Transmit);  // Enable RS485 Transmit
        //RS485Serial.write(byteLetti); // Send the byte back
        //delay(10);
        //digitalWrite(SSerialTxControl, RS485Receive);  // Disable RS485 Transmit
    }   // End If RS485SerialAvailable
    delay(100);
}       //--(end main loop )---
//*********( THE END )***********