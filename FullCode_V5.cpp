#include <LiquidCrystal.h>
#include <Ultrasonic.h>

#define PINO_ECHO 13
#define PINO_TRIG 10
#define LCD_BACKLIGHT 6
#define LCD_CONTRAST 9
#define MINIMUN 100

Ultrasonic ultrasonic(PINO_TRIG, PINO_ECHO);

using namespace std;

// Base da dados offline. Dados obtidos por meio da aquisição de dados, explicitada na seção 4.6.
unsigned short int minutos[100] = {20, 19, 19, 19, 17, 17, 16, 16, 16, 15, 
                                   15, 15, 15, 15, 15, 14, 14, 12, 12, 12, 
                                   12, 12, 12, 11, 10, 10, 10, 10, 8, 8, 8,
                                   07, 06, 06, 05, 05, 05, 05, 04, 03, 03, 
                                   03, 03, 03, 03, 03, 03, 03, 03, 02, 02, 
                                   02, 02, 02, 02, 01, 01, 01, 01, 01, 01,
                                   01, 01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                   0, 0, 0, 0, 20, 19, 19, 19, 16, 16, 16, 
                                   15, 15, 15, 16, 16, 18, 18, 18, 20, 20, 
                                   20, 22, 22, 22, 23};


// Funcoes
ISR(TIMER2_COMPA_vect);
ISR(TIMER1_COMPA_vect);
void printAds();
void menu();
unsigned short int tempoOnibus();
void delay1Sec();

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
float vetor[2] = {0, 0};

// Variaveis globais para a TelaLCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {

  noInterrupts();

  lcd.begin(16, 2);
  // OS PINOS ABAIXO DEVEM ESTAR NA ENTRADA DA BACKLIGHT E DO CONTRASTE.
  // ALTERE OS PRIMEIROS NÚMEROS SE NECESSÁRIO.
  // LEMBRE DO RESISTOR
  analogWrite(LCD_BACKLIGHT, 230);
  analogWrite(LCD_CONTRAST, 50);

  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

  // Configuracao do timer para contar cada segundo com o clock.
  // OCR1A = ((16*10^6) / (F1 * 1024)) - 1
  
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 15624;  //para F1 = 1 Hz, OCR1A = 15624
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);

  // Configuracao do timer de 10ms para o sensor de proximidade
  TCCR2A = 0;
  TCCR2B = 0;
  OCR2A = 156;
  TCCR2A |= (1 << WGM21);
  TCCR2B |= (1 << CS22) | (1 << CS20);  // Preescaler 1024
  TIMSK2 |= (1 << OCIE2A);
  
  showAds = true;
  interrupts();
}

void loop() {
  if (digitalRead(8) && showAds){
    showAds = false;
    delay1Sec();
  }

  if (!showAds) menu();
}

//Funcao executada a cada 10ms
ISR(TIMER2_COMPA_vect) {
  tens_of_milisecs++;

  // Ativado a cada 100ms
  if (tens_of_milisecs >= 10) {

    float cm_sec;
    float backlight;
    long microsec = ultrasonic.timing();
    cm_sec = ultrasonic.convert(microsec, Ultrasonic::CM);

    if (cm_sec > 50) cm_sec = 50;
    
    vetor[0] = (cm_sec - vetor[1]) * (0.05);
    if(vetor[0] > 10) vetor[0] = 10;
    else if(vetor[0] < -10) vetor[0] = (-10);
    vetor[1] += vetor[0];

    if (5 * vetor[1] < MINIMUN)
    backlight = MINIMUN;

    else 
    backlight = 5 * vetor[1];

    analogWrite(LCD_BACKLIGHT, backlight);
  
    tens_of_milisecs = 0;
  }
}

// Imprime um novo anuncio na tela a cada 10 segundos.
// Se passar mais de 15 segundos sem interacao no menu, volta a exibir anuncios
ISR(TIMER1_COMPA_vect) {

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
    unsigned short int tempo1;

  auto change = [](int& tela, int& tela_ant) {
    tela_ant = tela;
    tela = -1;
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
      lcd.print("Chega ");
      lcd.setCursor(6, 1);
      tempo1 = tempoOnibus();
      if(tempo1 == 0)
         lcd.print("JA");
      else {
        lcd.print("em ");
        lcd.setCursor(9, 1);
        lcd.print(tempoOnibus());
        lcd.setCursor(11, 1);
        lcd.print(" min");
      }
      break;
    case 8022:
      lcd.print("===== 8022 =====");
      lcd.setCursor(0, 1);
      lcd.print("Chega em    min");
      lcd.setCursor(9, 1);
      lcd.print(tempoOnibus());
      break;
    case 8032:
      lcd.print("===== 8032 =====");
      lcd.setCursor(0, 1);
      lcd.print("Chega em    min");
      lcd.setCursor(9, 1);
      lcd.print(tempoOnibus());
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
    secondsAtMenu = 0;

  } else if (digitalRead(7)) {
    if (tela_ant == 1)
        tela = 2;
    else if (tela_ant == 2)
        tela = 3;

    delay1Sec();
    secondsAtMenu = 0;
    }
}

unsigned short int tempoOnibus() {
    short int tempo = (millis() * 0.0001);
    if(tempo > 99) tempo -= 99;

    return minutos[tempo];
}

void delay1Sec() {
    time = millis();
    for (;;) {
      if (millis() - time > 1000) break;  //Aparentemente a millis() retorna em centesimos de segundo?
    }
  }
