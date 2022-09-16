#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // Устанавливаем дисплей

// Configuration

#define pinHallSensor1  2 // D2          <------- First Hall Sensor
#define Interrupt_H1    0 // D2 - INT 0
#define pinHallSensor2  3 // D3          <------- Second Hall Sensor
#define Interrupt_H2    1 // D3 - INT 1

#define pinResetButton 12
#define pinSaveButton 11

//test

int addr = 0;
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
  pinMode(pinResetButton, INPUT_PULLUP);
  pinMode(pinSaveButton, INPUT_PULLUP);
  TurnsCount = 0;
  IsNextTurn = true;
  //===================================================================================
  Serial.begin(250000);
  //===================================================================================
  pinMode(pinHallSensor1, INPUT);
  pinMode(pinHallSensor2, INPUT);
  attachInterrupt(Interrupt_H1, HallSensor_1, RISING);
  attachInterrupt(Interrupt_H2, HallSensor_2, RISING);
  //===================================================================================
  lcd.init();                     
  lcd.backlight();// Включаем подсветку дисплея
  //===================================================================================
//  //Writing Zero Data to EEPROM
//  TurnsCount = 0;
//  for (byte i = 1; i <= 10; i++) {
//    LastData.CurrentIndex = i;
//    LastData.TurnsCountArray[ LastData.CurrentIndex ] = LastData.CurrentIndex;
//  }
//  LastData.CurrentIndex = 4;
//  EEPROM.put(addr, LastData);   
  //===================================================================================
  // Restore Last Data from EEPROM, Read and Display Typing
  EEPROM.get(addr, LastData);
  
  byte thisIndex = 0;  
  for (byte i = 1; i <=10; i++) { 
    thisIndex = LastData.CurrentIndex+i;
    if ( thisIndex > 10 ) {
      thisIndex = thisIndex-10;
    }
    TurnsCount = LastData.TurnsCountArray[ thisIndex ];
    lcd.setCursor(0, 0);
    lcd.print("Index: "); 
    lcd.setCursor(7, 0);  
    lcd.print(thisIndex); 
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("Turns: "); 
    lcd.setCursor(7, 1);
    lcd.print(TurnsCount);    
    lcd.print("                ");  
    delay(1500);
  }

  //===================================================================================  
}

//################################################################
//################################################################
//################################################################
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
  if (digitalRead(pinSaveButton) == false) {
    // ----------------
    byte _CI = LastData.CurrentIndex+1;
    if ( _CI>10 ) {
      _CI = 1;
    }
    LastData.CurrentIndex = _CI;
    LastData.TurnsCountArray[ _CI ] = TurnsCount;
    EEPROM.put(addr, LastData);
  }  
  //===================================================================================
  if (digitalRead(pinResetButton) == false){
    TurnsCount = 0;
  }
  //===================================================================================
  lcd.setCursor(0, 0);
  lcd.print("Turns count:    ");
  lcd.setCursor(0, 1);
  //===================================================================================
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
  //===================================================================================
  // Выводим на экран количество секунд с момента запуска ардуины
  lcd.setCursor(0, 1);
  lcd.print(TurnsCount);  
  lcd.print("                ");  
}
