// include the library code:
#include <LiquidCrystal.h>
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  analogWrite(6, 0);
  lcd.begin(16,2);
  lcd.clear();
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
}

int tela = 0;

void loop() {
   bool iniciado = false;
   bool selecionado = false;
   bool naselecao = false;
   if (tela == 0){                          
   lcd.setCursor(0,0);
   //lcd.clear();
   lcd.print(F("PREVISAO: ONIBUS"));      
   lcd.setCursor(0,1);                       
   lcd.print(F("APERTE O BOTAO"));      
   }

   else if (tela == 1){                          //se a variável tela for igual a MAIS, faça...
   lcd.setCursor(0,0);                    //posiciona o cursor no display (caracter, linha)      
   //lcd.clear();
   lcd.print(F("SELECIONE A LINHA   "));      //imprime o texto entre aspas 
   lcd.setCursor(0,1);                    //posiciona o cursor no display (caracter, linha)      
   lcd.print(F(" 8012 -> "));      //imprime o texto entre aspas 
   }

   else if (tela == 2){                          //se a variável tela for igual a MAIS, faça...
   lcd.setCursor(0,0);                    //posiciona o cursor no display (caracter, linha)      
   //lcd.clear();
   lcd.print(F("SELECIONE A LINHA   "));      //imprime o texto entre aspas 
   lcd.setCursor(0,1);                    //posiciona o cursor no display (caracter, linha)      
   lcd.print(F(" 8022 -> "));      //imprime o texto entre aspas 
   }

   else if (tela == 3){                          //se a variável tela for igual a MAIS, faça...
   lcd.setCursor(0,0);                    //posiciona o cursor no display (caracter, linha)      
   //lcd.clear();
   lcd.print(F("SELECIONE A LINHA   "));      //imprime o texto entre aspas 
   lcd.setCursor(0,1);                    //posiciona o cursor no display (caracter, linha)      
   lcd.print(F(" 8032  "));      //imprime o texto entre aspas 
   }

   else if (tela == 8012){                          //se a variável tela for igual a MAIS, faça...
   lcd.setCursor(0,0);                    //posiciona o cursor no display (caracter, linha)      
   //lcd.clear();
   lcd.print(F("   8012   "));      //imprime o texto entre aspas 
   lcd.setCursor(0,1);                    //posiciona o cursor no display (caracter, linha)      
   lcd.print(F(" Chega em 2 min "));      //imprime o texto entre aspas 
   }

   else if (tela == 8022){                          //se a variável tela for igual a MAIS, faça...
   lcd.setCursor(0,0);                    //posiciona o cursor no display (caracter, linha)      
   //lcd.clear();
   lcd.print(F("   8022   "));      //imprime o texto entre aspas 
   lcd.setCursor(0,1);                    //posiciona o cursor no display (caracter, linha)      
   lcd.print(F(" Chega em 4 min "));      //imprime o texto entre aspas 
   }

   else if (tela == 8032){                          //se a variável tela for igual a MAIS, faça...
   lcd.setCursor(0,0);                    //posiciona o cursor no display (caracter, linha)      
   //lcd.clear();
   lcd.print(F("   8032   "));      //imprime o texto entre aspas 
   lcd.setCursor(0,1);                    //posiciona o cursor no display (caracter, linha)      
   lcd.print(F(" Chega em 6 min "));      //imprime o texto entre aspas 
   }
  
  if(digitalRead(8)&& tela == 0) {
   lcd.clear();
   tela = 1;
   delay(1500);
  }
  
  if(digitalRead(8) && tela == 1){
    lcd.clear();
    tela = 8012;
    delay(1500);
  }
  
  if(digitalRead(9) && tela == 1) {
   lcd.clear();
   tela = 2;
   delay (1500);
  }
  
  if(digitalRead(8) && tela == 2) {
    lcd.clear();
    tela = 8022;
  }

  if(digitalRead(9) && tela == 2) {
   lcd.clear();
   tela = 3;
   delay (1500);
  }
  
  if(digitalRead(8) && tela == 3) {
   lcd.clear();
   tela = 8032;
   delay (1500);
  }
}
