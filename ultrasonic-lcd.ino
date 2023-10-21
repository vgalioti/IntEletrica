#include <LiquidCrystal.h>
#include <Ultrasonic.h>

#define PINO_ECHO 13
#define PINO_TRIG 10

Ultrasonic ultrasonic(PINO_TRIG, PINO_ECHO);

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {

  // set up the LCD's number of columns and rows:

  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop() {
  float cm_sec;
  long microsec = ultrasonic.timing();
  cm_sec = ultrasonic.convert(microsec, Ultrasonic::CM)

  lcd.setCursor(0, 0);
  

  for (int thisChar = 0; thisChar < 10; thisChar++) {

    lcd.print(thisChar);

    delay(500);

  }

  int contraste = 100;
  
  for(int i = 0; i < 100; i++) {
    analogWrite(6, contraste - cm_sec);
    delay(50);
  }

  // set the cursor to (16,1):

  lcd.setCursor(16, 1);

  // set the display to automatically scroll:

  lcd.autoscroll();

  // print from 0 to 9:

  for (int thisChar = 0; thisChar < 10; thisChar++) {

    lcd.print(thisChar);

    delay(500);

  }

  lcd.noAutoscroll();
  
  for(int i = 100; i > 0; i--) {
    analogWrite(6, contraste - i);
    delay(50);
  }

  lcd.clear();
}
