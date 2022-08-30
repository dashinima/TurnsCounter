#include <Wire.h> 
#include "DigitLedDisplay.h"
#include <EEPROM.h>


//  Arduino Pin to Display Pin
//   7 to DIN,      PB3 - 15 pin (11)
//   6 to CS,       PB4 - 16 pin (12)
//   5 to CLK     PB5 - 17 pin (13)
DigitLedDisplay ld = DigitLedDisplay(PIN_PB3, PIN_PB4, PIN_PB5);

// Configuration

int pinHallSensor1 = PIN_PD2; // D2          <------- First Hall Sensor
int Interrupt_H1 = 0;   // D2 - INT 0
int pinHallSensor2 = PIN_PD3; // D3          <------- Second Hall Sensor
int Interrupt_H2 = 1;   // D3 - INT 1


byte LED_PIN_G = PIN_PB0; // 8
byte LED_PIN_B = PIN_PB1; // 9;
byte LED_PIN_R = PIN_PB2; // 10;

//test

volatile int addr = 0;
struct LastDataType {
  byte CurrentIndex = 0;
  volatile long TurnsCountArray[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
};
volatile LastDataType LastData;

// Other Vars
volatile boolean Hall_1_IsRising = false;
volatile boolean Hall_2_IsRising = false;
volatile unsigned long Hall_1_mills = 0;
volatile unsigned long Hall_2_mills = 0;

volatile boolean IsNextTurn = false;
volatile long TurnsCount = 0;

//################################################################
//################################################################
//################################################################
void setup()
{
  /* Set the brightness min:1, max:15 */
  ld.setBright(10);

  /* Set the digit count */
  ld.setDigitLimit(8);
  
    
  pinMode(LED_PIN_R, OUTPUT);
  digitalWrite(LED_PIN_R, HIGH);
  pinMode(LED_PIN_G, OUTPUT);
  digitalWrite(LED_PIN_G, HIGH);
  pinMode(LED_PIN_B, OUTPUT);
  digitalWrite(LED_PIN_B, HIGH);

  pinMode(pinHallSensor1, INPUT);
  pinMode(pinHallSensor2, INPUT);
  attachInterrupt(Interrupt_H1, HallSensor_1, RISING);
  attachInterrupt(Interrupt_H2, HallSensor_2, RISING);

//  //Writing Zero Data to EEPROM
//  TurnsCount = 0;
//  for (byte i = 1; i <= 10; i++) {
//    LastData.CurrentIndex = i;
//    LastData.TurnsCountArray[ LastData.CurrentIndex ] = LastData.CurrentIndex;
//  }
//  EEPROM.put(addr, LastData);   
//  digitalWrite(LED_PIN_G, LOW);
  
  // Restore Last Data from EEPROM, Read and Display Typing
  EEPROM.get( addr, LastData);
  byte thisIndex = 0;
  for (byte i = 1; i <= 10; i++) {
    if ( (LastData.CurrentIndex+i)<=10 ) {
      thisIndex = i;
    }
    else {
      thisIndex = (LastData.CurrentIndex+i)-10;
    };
    TurnsCount = LastData.TurnsCountArray[ thisIndex ];
    ld.printDigit(TurnsCount);
    delay(1500);
  };
  
  TurnsCount = 0;
  IsNextTurn = true;
}

//################################################################
//################################################################
//################################################################
ISR(RESET_vect) {
  // ----------------
  LastData.CurrentIndex = LastData.CurrentIndex+1;
  if (LastData.CurrentIndex>10) {
    LastData.CurrentIndex = 1;
  };
  LastData.TurnsCountArray[ LastData.CurrentIndex+1 ] = TurnsCount;
  EEPROM.put(addr, LastData);
}
void HallSensor_1() {
  Hall_1_IsRising = true;  
  Hall_1_mills = micros();
}
void HallSensor_2() {
  Hall_2_IsRising = true;  
  Hall_2_mills = micros();
}

//################################################################
//################################################################
//################################################################
void loop()
{
  //*********************************************
  
  if ( (Hall_1_IsRising == true) && (Hall_2_IsRising == true) ) { 
    if ( Hall_1_mills < Hall_2_mills ) {                          // Forward count
        TurnsCount = TurnsCount + 1;
    }
    if (Hall_1_mills > Hall_2_mills) {                            // Revers count
        TurnsCount = TurnsCount - 1;
    }
        Hall_1_IsRising = false;
        Hall_2_IsRising = false;
  }
  //*********************************************
  
  //*********************************************
  ld.printDigit(TurnsCount);
}
