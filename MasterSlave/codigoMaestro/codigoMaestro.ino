
#include <MatrizLed.h>

#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

#include<LedControl.h>


LiquidCrystal_I2C lcd(0x27,20,4);
unsigned long contador  = 0; 
boolean mensaje = false ; 
void setup() {
  lcd.begin(16,2);
  lcd.clear();

}

void loop() {
 metodoMensaje();
}

void metodoMensaje(){
    
  if(millis()<contador+1500){
    if(mensaje != true){
      contador = millis() + 1500;  
    }
       
    lcd.setCursor(0,0);
    lcd.print("Grupo 3");
    lcd.setCursor(2,1);
    lcd.print("Proyecto 1");
    mensaje = true;
  }else{
    if(millis()>contador){
      if(mensaje == true){
        lcd.clear();
        mensaje = false ; 
      }
      lcd.setCursor(2,1);
      lcd.print("iniciando");
    }
  }
}
