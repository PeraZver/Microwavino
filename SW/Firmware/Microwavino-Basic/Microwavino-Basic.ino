#include <Arduino.h>

/*  Basic Microwavino configuration and test
 *
 *  This example configures sinthesyzer MAX2871 over SPI.
 *  CLK is pin 13
 * DATA IN (MISO) is pin 12
 * DATA OUT (MOSI) is pin 11
 * LE (SS) for MAX2871 is pin 5
 * CE for MAX2871 is pin 4
 * RF EN is pin 7

Pero, September 2017

*/
#include <SPI.h>
#include "MAX2871.h"

// user input
char incomingChar = 0;  //Serial input

// sweep parameters
uint16_t a, b, dt;
char divider_type;

//fractional mode flag
boolean frac = 0;

//functiondeclarations
uint16_t String2Int();

void setup() {
  Serial.begin(9600);
  while (!Serial.available()) ;
  Serial.println("Microwavino");
  Serial.println("Basic configuration example");
  Serial.println("v1.0");
  Serial.println("Pero, September 2017");
  Serial.println(" ");
  Serial.println(" ");

  // initialize SPI:
  SPI.begin();
  Serial.println("SPI Initialized");

  MAX2871_Init ();
  Serial.println("MAX2871 Initialized");
}

void loop() {
  if(Serial.available() > 0){
    incomingChar = Serial.read();

    switch (incomingChar){


      case 'e':
         MAX2871_RFA_Enable();
         Serial.println("RFA out activated.");
         break;

      case 'd':
         MAX2871_RFA_Disable();
         Serial.println("RFA out deactivated.");
         break;

      case 'r':
         MAX2871_Read(Serial.read());   // type t for temperature or v for VCO tuning voltage
         break;

      case 'g':
         Serial.println("MAX2871 Registers printout:");
         MAX2871_Print_Registers();
         break;

      case 'p':
          MAX2871_RFA_SelectPower(Serial.read());
          break;

      case 'D':
          //Serial.print("Set output divider value DIVA (0-7): ");
          MAX2871_SetDIVA(Serial.read());
          break;

      case 'N':
          //Serial.print("Set integer division value N: ");
          MAX2871_SetN(String2Int());
          break;

      case 'F':
          //Serial.println("Fractional mode selected!");
          if(!frac)
            frac = 1;
            MAX2871_SetFracMode();
          //Serial.print("Set fractional division value F: ");
          MAX2871_SetF(String2Int());
          break;

      case 'R':
          //Serial.print("Set reference divider value R: ");
          MAX2871_SetR(String2Int());
          break;

      case 'M':
          //Serial.print("Set modulus division value M: ");
          MAX2871_SetM(String2Int());
          break;

      case 's':
          divider_type = Serial.read();  // N or F
          a = String2Int();   // lower limit
          b = String2Int();   // upper limit
          dt = String2Int();  //time delay
          Serial.println("Frequency Sweep activated.");
          Serial.print(divider_type);
          Serial.print(" from ");
          Serial.print(a);
          Serial.print(" to ");
          Serial.print(b);
          Serial.print(" in steps of ");
          Serial.print(dt);
          Serial.println(" ms.");
          MAX2871_Sweep(divider_type, a, b, dt);
          break;

      default:
            ;
    }
  }

  delay(1000);
}

uint16_t String2Int(){
  /* converts stream of serial input characters to integer.
     Condition is that stream ends with 'a'. */
  uint16_t number = 0;
  char a = '0';
  while (a != 'a'){
    if(Serial.available()){
      a = Serial.read();
      // Serial.print("\nRead character: ");
    //  Serial.print(a, DEC);
      if (a != 'a')
          number = number*10 + (a - 48);  // in ascii '0' is 48 dec.
    }
  }
  return number;
}
