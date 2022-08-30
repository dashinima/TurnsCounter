
/* Include DigitLedDisplay Library */
#include "DigitLedDisplay.h"

//  Arduino Pin to Display Pin
//   7 to DIN,      PB3 - 15 pin (11)
//   6 to CS,       PB4 - 16 pin (12)
//   5 to CLK     PB5 - 17 pin (13)
DigitLedDisplay ld = DigitLedDisplay(11, 12, 13);

  byte LED_PIN_R = PIN_PB0; // 8
  byte LED_PIN_G = PIN_PB1; // 9;
  byte LED_PIN_B = PIN_PB2; // 10;

void setup() {
  pinMode(LED_PIN_R, OUTPUT);
  pinMode(LED_PIN_G, OUTPUT);
  pinMode(LED_PIN_B, OUTPUT);

  /* Set the brightness min:1, max:15 */
  ld.setBright(10);

  /* Set the digit count */
  ld.setDigitLimit(8);

}

void loop() {

  /* Prints data to the display */
  ld.printDigit(12345678);
  delay(500);
  ld.clear();

  ld.printDigit(22222222);
  delay(500);
  ld.clear();

  ld.printDigit(44444444);
  delay(500);
  ld.clear();

  for (int i = 0; i < 100; i++) {
    ld.printDigit(i);

    /* Start From Digit 4 */
    ld.printDigit(i, 4);
    delay(50);
  }

  for (int i = 0; i <= 10; i++) {
    /* Display off */
    ld.off();
    delay(150);

    /* Display on */
    ld.on();
    delay(150);
  }

  /* Clear all display value */
  ld.clear();
  delay(500);

  for (long i = 0; i < 100; i++) {
    ld.printDigit(i);
    delay(25);
  }

  for (int i = 0; i <= 20; i++) {
    /* Select Digit 5 and write B01100011 */
    ld.write(5, B01100011);
    delay(200);

    /* Select Digit 5 and write B00011101 */
    ld.write(5, B00011101);
    delay(200);
  }

  /* Clear all display value */
  ld.clear();
  delay(500);

  
  digitalWrite(LED_PIN_R, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_PIN_R, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
  digitalWrite(LED_PIN_G, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_PIN_G, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
  digitalWrite(LED_PIN_B, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_PIN_B, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second

}
