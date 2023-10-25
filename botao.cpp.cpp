#include <LiquidCrystal.h>

LiquidCrystal lcd(2,3,4,5,6,7);
/*bool confirmar = digitalRead(8); 
bool seta = digitalRead(9);*/

void setup() {
  lcd.begin(16,2);
  lcd.clear();
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  /*while true{
    while (digitalRead(11) == LOW){
      lcd.print("PREVISAO: ONIBUS");
      lcd.setCursor(0,1);
      lcd.print("Aperte o botao para comecar");
      if (digitalRead(11) == HIGH){
          break;
      }
      delay (1000);
    }

    
  
    while (digitalRead(12) == LOW){
      lcd.clear();
      lcd.print("Selecione a linha");
      lcd.setCursor(0,1);
      lcd.print("    8012    ->");
      if (digitalRead(11) = HIGH){
        lcd.clear();
        lcd.print("8012");
        lcd.setCursor(0,1);
        lcd.print("A 2 minutos");
        delay(3000);
        break;
      }
      delay(1000);
    }
    
    
    while (digitalRead(12) == LOW){
      lcd.clear();
      lcd.print("Selecione a linha");
      lcd.setCursor(0,1);
      lcd.print("    8022    ->");
      if(digitalRead(11) = HIGH){
        lcd.clear();
        lcd.print("8022");
        lcd.setCursor(0,1);
        lcd.print("A 5 minutos");
        delay(3000);
        break;
      }
      delay(1000);
    }



    while (digitalRead(12) == LOW){
      lcd.clear();
      lcd.print("Selecione uma linha");
      lcd.setCursor(0,1);
      lcd.print("    8032    ");
      if(digitalRead(11) = HIGH){
          lcd.clear();
          lcd.print("8022");
          lcd.setCursor(0,1);
          lcd.print("A 5 minutos");
          delay(3000);
          break;
      }
    }
  }*/

}

void loop() {

    while (digitalRead(11) == LOW){
      lcd.print("PREVISAO: ONIBUS");
      lcd.setCursor(0,1);
      lcd.print("Aperte o botao para comecar");
      if (digitalRead(11) == HIGH){
          break;
      }
      delay (1000);
    }
      while (digitalRead(12) == LOW){
      lcd.clear();
      lcd.print("Selecione a linha");
      lcd.setCursor(0,1);
      lcd.print("    8012    ->");
      if (digitalRead(11) = HIGH){
        lcd.clear();
        lcd.print("8012");
        lcd.setCursor(0,1);
        lcd.print("A 2 minutos");
        delay(3000);
        break;
      }
      delay(1000);
    }
    
    
    while (digitalRead(12) == LOW){
      lcd.clear();
      lcd.print("Selecione a linha");
      lcd.setCursor(0,1);
      lcd.print("    8022    ->");
      if(digitalRead(11) = HIGH){
        lcd.clear();
        lcd.print("8022");
        lcd.setCursor(0,1);
        lcd.print("A 5 minutos");
        delay(3000);
        break;
      }
      delay(1000);
    }



    while (digitalRead(12) == LOW){
      lcd.clear();
      lcd.print("Selecione uma linha");
      lcd.setCursor(0,1);
      lcd.print("    8032    ");
        if(digitalRead(11) = HIGH){
          lcd.clear();
          lcd.print("8022");
          lcd.setCursor(0,1);
          lcd.print("A 5 minutos");
          delay(3000);
          break;
        }
      delay(1000);
    }


    
  

}

  
