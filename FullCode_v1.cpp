#include <LiquidCrystal.h>

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

void setup()
{
  lcd.begin(16, 2);
  // OS PINOS ABAIXO DEVEM ESTAR NA ENTRADA DA BACKLIGHT E DO CONTRASTE. 
  // ALTERE OS PRIMEIROS NÚMEROS SE NECESSÁRIO.
  // LEMBRE DO RESISTOR
  analogWrite(6, 0);
  analogWrite(9, 0);

  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  
  // Configuracao do timer para contar cada segundo com o clock.
  // OCR1A = ((16*10^6) / (F1 * 1024)) - 1
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 15624; //para F1 = 1 Hz, OCR1A = 15624
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10); 
  TIMSK1 |= (1 << OCIE1A);

  // Configuracao do timer de 10ms para o sensor de proximidade
  TCCR0A = 0;
  TCCR0B = 0;
  OCR0A = 156;
  TCCR0A |= (1<<WGM01);
  TCCR0B |= (1<<CS02) | (1<<CS00); // Preescaler 1024
  TIMSK0 |= (1<<OCIE0A);

  sei();
  interrupts();
}

void loop()
{  
  showAds = false;
  if(!showAds) menu();
}

//Funcao executada a cada 10ms
ISR(TIMER0_COMPA_vect) {
  tens_of_milisecs++;

  // Ativado a cada 100ms
  if(tens_of_milisecs >= 10) {

    tens_of_milisecs = 0;
  }
}

// Imprime um novo anuncio na tela a cada 10 segundos.
// Se passar mais de 15 segundos sem interacao no menu, volta a exibir anuncios
ISR(TIMER1_COMPA_vect) {
  if(showAds) {
    secondsAtMenu = 0;
    secondsAtAds++;

    if(secondsAtAds >= 10) {
      printAds();
      secondsAtAds = 0;
    }
  }
  else {
    secondsAtAds = 0;
    secondsAtMenu++;
    if(secondsAtMenu > 15) {
      showAds = true;
      secondsAtAds = 10;
    }
  }
}

void printAds() {
  if(changeAds == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Projeto Jupiter");
    lcd.setCursor(0, 1);
    lcd.print("   PARTICIPE!   ");
    changeAds++;
  }

  else if(changeAds == 1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("    POLISAT    ");
    lcd.setCursor(0, 1);
    lcd.print(" Nano Satelites ");
    changeAds = 0;
  }
}

void menu() {
  auto change = [] (int& tela, int& tela_ant) {
        tela_ant = tela;
        tela = -1;
  };

  auto delay1Sec = [] () {
   time = millis();
   for(;;) {
    if(millis() - time > 100) break; //Aparentemente a millis() retorna em centesimos de segundo?
   }
  };

  if (tela == 0){                          
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("LINHAS DE ONIBUS");
   lcd.setCursor(0,1);
   lcd.print(" Aperte o botao ");
   change(tela, tela_ant);
  }

  else if (tela == 1){
   lcd.clear();
   lcd.setCursor(0,0);                        
   lcd.print("== SELECIONE ==");       
   lcd.setCursor(0,1);                       
   lcd.print("      8012   -> ");
   change(tela, tela_ant);
  }

  else if (tela == 2){
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("== SELECIONE ==");
   lcd.setCursor(0,1);
   lcd.print("      8022   -> ");
   change(tela, tela_ant);
  }

  else if (tela == 3){
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("== SELECIONE ==");
   lcd.setCursor(0,1);
   lcd.print("      8032   -> ");
   change(tela, tela_ant);
  }

  else if (tela == 8012){
   lcd.setCursor(0,0);
   lcd.clear();
   lcd.print("===== 8012 =====");
   lcd.setCursor(0,1);
   lcd.print("Chega em 02 min");
   change(tela, tela_ant);
  }

  else if (tela == 8022){
   lcd.setCursor(0,0);
   lcd.clear();
   lcd.print("===== 8022 =====");
   lcd.setCursor(0,1);
   lcd.print("Chega em 04 min");
   change(tela, tela_ant);
  }

  else if (tela == 8032){
   lcd.setCursor(0,0);
   lcd.clear();
   lcd.print("===== 8032 =====");
   lcd.setCursor(0,1);
   lcd.print("Chega em 06 min");
   change(tela, tela_ant);
  }
  
  if(digitalRead(8) && tela_ant == 0) {
   tela = 1;
   secondsAtMenu = 0;
   delay1Sec();
  }
  
  if(digitalRead(8) && tela_ant == 1){
   tela = 8012;
   secondsAtMenu = 0;
   delay1Sec();
  }
  
  if(digitalRead(9) && tela_ant == 1) {
   tela = 2;
   secondsAtMenu = 0;
   delay1Sec();
  }
  
  if(digitalRead(8) && tela_ant == 2) {
   tela = 8022;
   secondsAtMenu = 0;
   delay1Sec();
  }

  if(digitalRead(9) && tela_ant == 2) {
   tela = 3;
   secondsAtMenu = 0;
   delay1Sec();
  }
  
  if(digitalRead(8) && tela_ant == 3) {
   tela = 8032;
   secondsAtMenu = 0;
   delay1Sec();
  }

  if((digitalRead(8) && (tela_ant == 8012 || tela_ant == 8022 || tela_ant == 8032))) {
    tela = 0;
    secondsAtMenu = 0;
    delay1Sec();
  }
}
