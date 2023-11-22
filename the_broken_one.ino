#include <LiquidCrystal.h>
#include <Ultrasonic.h>

#define PINO_ECHO 13
#define PINO_TRIG 16
#define LCD_BACKLIGHT 6
#define LCD_CONTRAST 9

Ultrasonic ultrasonic(PINO_TRIG, PINO_ECHO);

// Funcoes
ISR(TIMER0_COMPA_vect);
ISR(TIMER1_COMPA_vect);
void printAds();
void menu();

// Variaveis globais
bool showAds = true;
int adsId;
int secondsAtAds = 10;
int secondsAtMenu = 0;
int changeAds = 0;
int tens_of_milisecs = 0;
int tela = 0;
int tela_ant = 0;
unsigned long time;

// Variaveis globais para a TelaLCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  lcd.begin(16, 2);
  // OS PINOS ABAIXO DEVEM ESTAR NA ENTRADA DA BACKLIGHT E DO CONTRASTE.
  // ALTERE OS PRIMEIROS NÚMEROS SE NECESSÁRIO.
  // LEMBRE DO RESISTOR
  analogWrite(6, 0);
  analogWrite(9, 100);

  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);

  // Configuracao do timer para contar cada segundo com o clock.
  // OCR1A = ((16*10^6) / (F1 * 1024)) - 1
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 15624;  //para F1 = 1 Hz, OCR1A = 15624
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);

  // Configuracao do timer de 10ms para o sensor de proximidade
  TCCR0A = 0;
  TCCR0B = 0;
  OCR0A = 156;
  TCCR0A |= (1 << WGM01);
  TCCR0B |= (1 << CS02) | (1 << CS00);  // Preescaler 1024
  TIMSK0 |= (1 << OCIE0A);

  sei();
  interrupts();
}

void loop() {
  showAds = false;
  if (!showAds) menu();
}

//Funcao executada a cada 10ms
ISR(TIMER0_COMPA_vect) {
  tens_of_milisecs++;

  // Ativado a cada 100ms
  if (tens_of_milisecs >= 10) {

    tens_of_milisecs = 0;
  }
}

// Imprime um novo anuncio na tela a cada 10 segundos.
// Se passar mais de 15 segundos sem interacao no menu, volta a exibir anuncios
ISR(TIMER1_COMPA_vect) {
  float cm_sec;
  long microsec = ultrasonic.timing();
  if (showAds) {
    secondsAtMenu = 0;
    secondsAtAds++;

    if (secondsAtAds >= 15) {
      printAds();
      secondsAtAds = 0;
    }
  } else {
    secondsAtAds = 0;
    secondsAtMenu++;
    if (secondsAtMenu > 25) {
      showAds = true;
      secondsAtAds = 15;
    }
  }
  cm_sec = ultrasonic.convert(microsec, Ultrasonic::CM);
  if (cm_sec > 50)
    cm_sec = 50;
  analogWrite(6, (5 * cm_sec));
}

void printAds() {
  lcd.clear();
  lcd.setCursor(0, 0);
  if (changeAds == 0) {
    lcd.print("Projeto Jupiter");
    lcd.setCursor(0, 1);
    lcd.print("   PARTICIPE!   ");
    changeAds++;
  }

  else if (changeAds == 1) {
    lcd.print("    POLISAT    ");
    lcd.setCursor(0, 1);
    lcd.print(" Nano Satelites ");
    changeAds = 0;
  }
}

void menu() {
  auto change = [](int& tela, int& tela_ant) {
    tela_ant = tela;
    tela = -1;
  };

  auto delay1Sec = []() {
    time = millis();
    for (;;) {
      if (millis() - time > 100) break;  //Aparentemente a millis() retorna em centesimos de segundo?
    }
  };

  
  if (tela != -1){
    lcd.setCursor(0, 0);
    lcd.clear();

  switch (tela) {
    case 0:
      lcd.print("LINHAS DE ONIBUS");
      lcd.setCursor(0, 1);
      lcd.print(" Aperte o botao ");
      break;
    case 1:
      lcd.print("== SELECIONE ==");
      lcd.setCursor(0, 1);
      lcd.print("      8012   -> ");
      break;
    case 2:
      lcd.print("== SELECIONE ==");
      lcd.setCursor(0, 1);
      lcd.print("      8022   -> ");
      break;
    case 3:
      lcd.print("== SELECIONE ==");
      lcd.setCursor(0, 1);
      lcd.print("      8032   -> ");
      break;
    case 8012:
      lcd.print("===== 8012 =====");
      lcd.setCursor(0, 1);
      lcd.print("Chega em 02 min");
      break;
    case 8022:
      lcd.print("===== 8022 =====");
      lcd.setCursor(0, 1);
      lcd.print("Chega em 04 min");
      break;
    case 8032:
      lcd.print("===== 8032 =====");
      lcd.setCursor(0, 1);
      lcd.print("Chega em 06 min");
      break;
  }
  change(tela, tela_ant);
}

  if (digitalRead(8)) {
    switch (tela_ant) {
      case 0:
        tela = 1;
        break;
      case 1:
        tela = 8012;
        break;
      case 2:
        tela = 8022;
        break;
      case 3:
        tela = 8032;
        break;
    }
    if (tela_ant == 8012 || tela_ant == 8022 || tela_ant == 8032) {
      tela = 0;
    }
    delay1Sec();
    secondsAtMenu = 0

  } else if (digitalRead(7)) {
    if (tela_ant == 1)
        tela = 2;
    else if (tela == 2)
        tela = 3;

    delay1Sec();
    secondsAtMenu = 0
    }
}
