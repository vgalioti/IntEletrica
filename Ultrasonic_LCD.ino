#include <LiquidCrystal.h>
#include <Ultrasonic.h>

using namespace std;

#define PINO_ECHO 13
#define PINO_TRIG 10

Ultrasonic ultrasonic(PINO_TRIG, PINO_ECHO);

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to

ISR(TIMER1_COMPA_vect);
void printAds();
void menu();

// Variaveis globais
bool showAds = true;
int adsId;
int secondsAtAds = 10;
int secondsAtMenu = 0;
int teste = 0;

// Variaveis globais para a TelaLCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
  // Serial.begin(9600)
  pinMode(7, OUTPUT);
  lcd.begin(16, 2);
  analogWrite(6, 0);
  Serial.begin(9600);
  
  // Configuração do timer para contar cada segundo com o clock.
  // OCR1A = ((16*10^6) / (F1 * 1024)) - 1

  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 3124; //para F1 = 1 Hz, OCR1A = 15624
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10); 
  TIMSK1 |= (1 << OCIE1A);
  
  interrupts();
}

void loop()
{  
  // Código do sensor de proximidade.
  // Exemplo do timer (pseudocodigo)
  // if(muitoperto) {
  //    showAds = false;
  //    menu();
  //}
}

// Imprime um novo anuncio na tela a cada 10 segundos.
// Se passar mais de 15 segundos sem interacao no menu, volta a exibir anuncios
ISR(TIMER1_COMPA_vect) {
  float cm_sec;
  long microsec = ultrasonic.timing();
  if(showAds) {
    secondsAtMenu = 0;
    secondsAtAds++;

    if(secondsAtAds >= 50) {
      printAds();
      secondsAtAds = 0;
    }
  }
  else {
    secondsAtAds = 0;
    secondsAtMenu++;
    lcd.clear();
    if(secondsAtMenu > 75) {
      showAds = true;
      secondsAtAds = 50;
    }
  }
  cm_sec = ultrasonic.convert(microsec, Ultrasonic::CM);
  if (cm_sec > 50)
    cm_sec = 50;
  analogWrite(6, (5 * cm_sec));
}

void printAds() {
  if(teste == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("POLI FINANCE");
    lcd.setCursor(0, 1);
    lcd.print("PS 2024 aberto!");
    teste++;
  }

  else if(teste == 1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("POLI JUNIOR");
    lcd.setCursor(0, 1);
    lcd.print("Maior empresa JR");
    teste = 0;
  }
}
