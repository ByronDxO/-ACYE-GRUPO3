/* Pantalla Led */
#include<LedControl.h>
#include <LiquidCrystal_I2C.h>

/* Teclado Matricial */
#include <Keypad.h>

/* TIME */
unsigned long time_matrix = 0;

/* Variables */
int contador = 0 ; 
boolean mensaje = false ; 

const byte matrix_row     = 4;
const byte matrix_column  = 3;
 
char keys[matrix_row][matrix_column] = {
   { '1','2','3'},
   { '4','5','6'},
   { '7','8','9'},
   { '*','0','#'}
};

const byte matrix_row_pin[matrix_row]       = { 30, 31, 32, 33};
const byte matrix_column_pin[matrix_column] = { 34, 35, 36};


/* Definition Object */
LiquidCrystal_I2C lcd(0x27,20,4);

Keypad keypad = Keypad(makeKeymap(keys), matrix_row_pin, matrix_column_pin, matrix_row, matrix_column);

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.print("Mancos qliados");

}

void loop() {
  
 //Serial.println("hola");
 //metodoMensaje();
  if (millis() > time_matrix + 10){
    time_matrix = millis();

    char key = keypad.getKey();
    if (key) {
      Serial.println(key);
    }
  }
 
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
