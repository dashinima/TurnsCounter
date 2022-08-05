#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // Устанавливаем дисплей

// Configuration

int pinHallSensor1 = 2; // D2          <------- First Hall Sensor
int Interrupt_H1 = 0;   // D2 - INT 0
int pinHallSensor2 = 3; // D3          <------- Second Hall Sensor
int Interrupt_H2 = 1;   // D3 - INT 1

// Other Vars
boolean Hall_1_LastPinState = LOW;
boolean Hall_1_IsRising = false;
boolean Hall_2_LastPinState = LOW;
boolean Hall_2_IsRising = false;
unsigned long Hall_1_mills = 0;
unsigned long Hall_2_mills = 0;

boolean IsNextTurn = false;
long TurnsCount = 0;

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
  //lcd.print("Turns number:");
  //lcd.setCursor(8, 1);
  //lcd.print("t.");
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
  lcd.setCursor(0, 0);
  lcd.print("Turns count:");
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
