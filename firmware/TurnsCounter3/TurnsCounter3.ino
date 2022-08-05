#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // Устанавливаем дисплей

// Configuration

int pinHallSensor1 = 2; // D2          <------- First Hall Sensor
int Interrupt_H1 = 0;   // D2 - INT 0
int pinHallSensor2 = 3; // D3          <------- Second Hall Sensor
int Interrupt_H2 = 1;   // D3 - INT 1

//test

int addr = 0;
struct LastDataType {
  byte CurrentIndex = 0;
  volatile long TurnsCountArray[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
}
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
  TurnsCount = 0;
  IsNextTurn = true;
  
  Serial.begin(250000);
  
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);

  pinMode(pinHallSensor1, INPUT);
  pinMode(pinHallSensor2, INPUT);
  attachInterrupt(Interrupt_H1, HallSensor_1, RISING);
  attachInterrupt(Interrupt_H2, HallSensor_2, RISING);
  
  lcd.init();                     
  lcd.backlight();// Включаем подсветку дисплея

  //Writing Zero Data to EEPROM
//  noInterrupts();
//  EEPROM.put(addr, LastData);
//  interrupts();

  // Restore Last Data from EEPROM, Read and Display Typing
  LastData = EEPROM.get(addr);
  byte thisIndex = 0;
  for (byte i = 1; i <= 10; i++) {
    if ( (LastData.CurrentIndex+i)<=10 ) {
      thisIndex = i;
    }
    else {
      thisIndex = (LastData.CurrentIndex+i)-10;
    }
    TurnsCount = LastData.TurnsCountArray[ thisIndex ];
    lcd.setCursor(0, 0);
    lcd.print("Index: "); 
    lcd.setCursor(7, 0);
    lcd.print(thisIndex); 
    lcd.setCursor(0, 1);
    lcd.print("Turns: "); 
    lcd.setCursor(7, 1);
    lcd.print(TurnsCount); 
    delay(1500);
  }
  
}

//################################################################
//################################################################
//################################################################
ISR(RESET_vect) {
  // ----------------
  LastData.CurrentIndex = LastData.CurrentIndex+1;
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
  lcd.setCursor(0, 0);
  lcd.print("Turns count:    ");
//  lcd.setCursor(0, 1);
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
  // Выводим на экран количество секунд с момента запуска ардуины
  lcd.setCursor(0, 1);
  lcd.print(TurnsCount);  
  lcd.print("                ");
}
