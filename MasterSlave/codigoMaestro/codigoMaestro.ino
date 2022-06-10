#include <Wire.h> 
#include <MatrizLed.h>
#include <Key.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>
#include <Servo.h>
#include<LedControl.h>


LiquidCrystal_I2C lcd(0x27,20,4);
int contador = 0 ; 
boolean mensaje = false ; 
void setup() {
  lcd.begin(16,2);
  lcd.print("Mancos qliados");

}

void loop() {
 metodoMensaje();
}

void metodoMensaje(){
  if(millis()>4000 && mensaje == false){
    lcd.begin(16,2);
    lcd.print("Grupo 3");
    lcd.begin(16,1);
    lcd.print("Proyecto 1");
    mensaje = true;
  }
}
