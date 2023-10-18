//lembrar de tirar a declaração da <Ultrasonic.h> e colocar a convert de microsec para CM
#include <LiquidCrystal.h>

#define DM 28
#define INC 71
#define PINO_ECHO 13
#define PINO_TRIG 10

class Ultrasonic {
  public:
    Ultrasonic(uint8_t sigPin) : Ultrasonic(sigPin, sigPin) {};
    Ultrasonic(uint8_t trigPin, uint8_t echoPin, unsigned long timeOut = 20000UL);
    unsigned int read(uint8_t und = DM);
    unsigned int distanceRead(uint8_t und = DM) __attribute__ ((deprecated ("This method is deprecated, use read() instead.")));
    void setTimeout(unsigned long timeOut) {timeout = timeOut;}
    void setMaxDistance(unsigned long dist) {timeout = dist*DM*2;}
    uint8_t trig;
    uint8_t echo;
    boolean threePins = false;
    unsigned long previousMicros;
    unsigned long timeout;
    unsigned int timing();
};



Ultrasonic::Ultrasonic(uint8_t trigPin, uint8_t echoPin, unsigned long timeOut) {
  trig = trigPin;
  echo = echoPin;
  threePins = trig == echo ? true : false;
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  timeout = timeOut;
}

unsigned int Ultrasonic::timing() {
  if (threePins)
    pinMode(trig, OUTPUT);

  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  delay(30);

  if (threePins)
    pinMode(trig, INPUT);
  
  previousMicros = micros();
  while(!digitalRead(echo) && (micros() - previousMicros) <= timeout); // wait for the echo pin HIGH or timeout
  previousMicros = micros();
  while(digitalRead(echo)  && (micros() - previousMicros) <= timeout); // wait for the echo pin LOW or timeout

  return micros() - previousMicros; // duration
}

/*
 * If the unit of measure is not passed as a parameter,
 * sby default, it will return the distance in centimeters.
 * To change the default, replace CM by INC.
 */
unsigned int Ultrasonic::read(uint8_t und) {
  return timing() / und / 2;  //distance by divisor
}

/*
 * This method is too verbal, so, it's deprecated.
 * Use read() instead.
 */
unsigned int Ultrasonic::distanceRead(uint8_t und) {
  return read(und);
}


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
  long microsec = ultrasonic.timing();

  lcd.setCursor(0, 0);
  

  for (int thisChar = 0; thisChar < 10; thisChar++) {

    lcd.print(thisChar);

    delay(500);

  }

  int contraste = 100;
  
  for(int i = 0; i < 100; i++) {
    analogWrite(6, contraste - microsec << 2);
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
