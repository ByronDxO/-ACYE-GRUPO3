/* EEPROM */
#include <EEPROM.h>
/* Pantalla Led */
#include<LedControl.h>
#include <LiquidCrystal_I2C.h>
/* Teclado Matricial */
#include <Keypad.h>

/* bluetooth */
#define bluethoot 8
/* sensor de temperatura */
#define pinA1 1

/* TIME */
unsigned long time_matrix = 0;
unsigned long contador_1  = 0; 

/* Variables */
float temperature_C;
/************************/
int contador = 0 ; 
boolean mensaje = false ; 


/* Create Matrix Keyboard */

const byte matrix_row     = 4;
const byte matrix_column  = 3;
 
char keys[matrix_row][matrix_column] = {
   { '1','2','3'},
   { '4','5','6'},
   { '7','8','9'},
   { '*','0','#'}
};

/* PINES DE TECLADO MATRICIAL */
const byte matrix_row_pin[matrix_row]       = { 30, 31, 32, 33};
const byte matrix_column_pin[matrix_column] = { 34, 35, 36};

/* Create Struct */
int eeAddress = 0;
struct MyStruct{
  int icecream0;
  int icecream1;
  int icecream2;
  int icecream3;
  int icecream4;
  float money;
  float temperature;
};

/* Assignment Struct */
MyStruct MyStructValue = {10,10,10,10,10,0.0,0.0};

/* Declaration */
Keypad keypad = Keypad(makeKeymap(keys), matrix_row_pin, matrix_column_pin, matrix_row, matrix_column);
LiquidCrystal_I2C lcd(0x27,20,4);

void setup() {
  // put your setup code here, to run once:
  /** TERMINAL **/
  Serial.begin(9600);   // Terminal
  Serial1.begin(9600);  // Bluethooth

  pinMode(bluethoot, OUTPUT);
  /* EEPROM */
  EEPROM.put(eeAddress, MyStructValue);
  /* PANTALLA LCD */
  lcd.begin(16,2);
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  
metodoMensaje();
  if (Serial1.available() > 0) {
      char readed = Serial1.read();

      if (readed == 'C'){ // conectar Bluethooth
       Serial.println("Bluethoot Conectado!"); 
      }else if (readed == 'U'){ // UPDATE
        Serial.println("Actualizando Datos");
        EEPROM.get(eeAddress, MyStructValue);     // getStruct
        
        /* Temperatura */
        temperature_C = analogRead(pinA1);                      // Leemos el sensor por medio de analogRead, se encuentra en un rango de 0 a 1023.
        temperature_C = (5.0 * temperature_C * 100.0)/1024.0;   // Calculamos la temperatura con la fórmula.
        MyStructValue.temperature = temperature_C;
        EEPROM.put(eeAddress, MyStructValue);
        
        
        /* Enviar Datos a APP*/
        EEPROM.get(eeAddress, MyStructValue);     // getStruct
        Serial1.print(String(MyStructValue.icecream0));
        Serial1.print("|");
        Serial1.print(String(MyStructValue.icecream1));
        Serial1.print("|");
        Serial1.print(String(MyStructValue.icecream2));
        Serial1.print("|");
        Serial1.print(String(MyStructValue.icecream3));
        Serial1.print("|");
        Serial1.print(String(MyStructValue.icecream4));
        Serial1.print("|");
        Serial1.print(String(MyStructValue.money));
        Serial1.print("|");
        Serial1.print(String(MyStructValue.temperature));

        /* Recargar Stock */
        boolean flag = false;
        if (MyStructValue.icecream0 == 0){ MyStructValue.icecream0 = 10; flag = true; }
        if (MyStructValue.icecream1 == 0){ MyStructValue.icecream1 = 10; flag = true; }
        if (MyStructValue.icecream2 == 0){ MyStructValue.icecream2 = 10; flag = true; }
        if (MyStructValue.icecream3 == 0){ MyStructValue.icecream3 = 10; flag = true; }
        if (MyStructValue.icecream4 == 0){ MyStructValue.icecream4 = 10; flag = true; }
        if (flag) { EEPROM.put(eeAddress, MyStructValue); flag = false; }
        eeAddress = 0;

        
      }else if (readed == 'O') {
          digitalWrite(bluethoot, HIGH);
          EEPROM.get(eeAddress, MyStructValue);     // getStruct
          MyStructValue.icecream0--;
          EEPROM.put(eeAddress, MyStructValue); // reasigno datos 
          eeAddress += sizeof(MyStructValue);
          eeAddress = 0;
         
      } else if (readed == 'F'){
        digitalWrite(bluethoot, LOW);
      } 

  }

}
// meteodo seleccionar vaso 
boolean bandera_vaso = false ; 
int valor_vaso = 0 ;
unsigned long timer_vaso = 0 ;
boolean bandera_precio_vaso = false;
void getVaso(){
  if(bandera_precio_vaso != true){
    if(bandera_vaso != true){
    lcd.clear();
    lcd.setCursor(1,0);
  lcd.print("seleccione el");
  lcd.setCursor(0,1);
  lcd.print("vaso:");
  bandera_vaso = true;
  }
  
  } 
  
  valor_vaso = getTeclado();
  postPrecioVaso(valor_vaso);
}
// switch para tamaño vaso y precio 
boolean vaso_limpiar = false;
void postPrecioVaso(char x){
  bandera_precio_vaso = true;
  if(vaso_limpiar != true && bandera_vaso != false){
    lcd.clear();
    vaso_limpiar=true;
  }
  switch (x){
    case '1':
    lcd.setCursor(1,0);
    lcd.print("precio: 20");
    valor_vaso=0;
    break;
    case '2':
    lcd.setCursor(1,0);
    lcd.print("precio: 30");
    valor_vaso=0;
    break;
    case '3':
    lcd.setCursor(1,0);
    lcd.print("precio: 40");
    valor_vaso=0;
    break;
  }
}
/// metodo para retornar un numero 
int getTeclado(){
   if (millis() > time_matrix + 10){
    time_matrix = millis();

    char key = keypad.getKey();
    if (key) {
      Serial.println(key);
      lcd.setCursor(14,1);
      lcd.print(key);
      return int(key);
    }
  }
}
/* PANTALLA LCD */

void metodoTeclado(){
   if (millis() > time_matrix + 10){
    time_matrix = millis();

    char key = keypad.getKey();
    if (key) {
      Serial.println(key);
      lcd.setCursor(0,1);
      lcd.print(key);
    }
  }
}


/* MATRIZ KEYBOARD */
void metodoMensaje(){
    
  if(millis()<contador_1+1500){
    if(mensaje != true){
      contador_1 = millis() + 1500;  
    }
       
    lcd.setCursor(0,0);
    lcd.print("Grupo 3");
    lcd.setCursor(2,1);
    lcd.print("Proyecto 1");
    mensaje = true;
  }else{
    if(millis()>contador_1){
      if(mensaje == true){
        lcd.clear();
        mensaje = false ; 
      }
      lcd.setCursor(2,1);
    //  metodoTeclado();
    getVaso();
    }
  }
}
