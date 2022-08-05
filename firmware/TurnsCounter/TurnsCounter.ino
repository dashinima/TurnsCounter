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
  attachInterrupt(Interrupt_H1, HallSensor_1, CHANGE);
  attachInterrupt(Interrupt_H2, HallSensor_2, CHANGE);
  
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
  int pinH1 = digitalRead(pinHallSensor1);
  if ((Hall_1_LastPinState==LOW)&&(pinH1==HIGH)) {
    Hall_1_IsRising = true;  
    Hall_1_LastPinState = HIGH;
  }
  if ((Hall_1_LastPinState==HIGH)&&(pinH1==LOW)) {
    Hall_1_IsRising = false;  
    Hall_1_LastPinState = LOW;
  }  
  Hall_1_mills = millis();
}
void HallSensor_2() {
  int pinH2 = digitalRead(pinHallSensor2);
  if ((Hall_2_LastPinState==LOW)&&(pinH2==HIGH)) {
    Hall_2_IsRising = true;  
    Hall_2_LastPinState = HIGH;
  }
  if ((Hall_2_LastPinState==HIGH)&&(pinH2==LOW)) {
    Hall_2_IsRising = false;  
    Hall_2_LastPinState = LOW;
  }  
  Hall_2_mills = millis();
}

//################################################################
//################################################################
//################################################################
void loop()
{
  // Устанавливаем курсор на вторую строку и нулевой символ.
  lcd.setCursor(0, 0);
  lcd.print("Turns count:");
//  lcd.setCursor(0, 1);
  //*********************************************
  
  if (Hall_1_mills < Hall_2_mills) { // Forward count
    if ( (IsNextTurn == true) && ( (Hall_1_IsRising == true) && (Hall_2_IsRising == true) ) ) {
        TurnsCount = TurnsCount + 1;
        IsNextTurn = false;
    }
    if ( (IsNextTurn == false) && ( (Hall_1_IsRising == false) && (Hall_2_IsRising == false) ) ) {
        IsNextTurn = true;
    }
  }
  if (Hall_1_mills > Hall_2_mills) { // Revers count
    if ( (IsNextTurn == true) && ( (Hall_1_IsRising == false) && (Hall_2_IsRising == false) ) ) {
        TurnsCount = TurnsCount - 1;
        IsNextTurn = false;    
    }
    if ( (IsNextTurn == false) && ( (Hall_1_IsRising == true) && (Hall_2_IsRising == true) ) ) {
        IsNextTurn = true;
    }
  }
  //*********************************************
  
  //*********************************************
  // Выводим на экран количество секунд с момента запуска ардуины
  //Serial.print(IsForwardTurn);
  lcd.setCursor(0, 1);
  lcd.print(TurnsCount);  
  lcd.print("                ");
}
