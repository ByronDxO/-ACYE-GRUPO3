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
boolean bandera_precio_vaso = false;
/************************/
String conca_matrix; 
boolean flag_inventary = false;
boolean flag_choose_icecream = false;
int cont_code = 0;    // cont del no. de codigo.
int cont_matrix = 1;  // cont cantidad del code.
int cont_sabor = 1;   // obtiene el No. de sabor.
int valor_vaso_definitivo = 0 ;
float money_vaso = 0.0;
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
//MyStruct MyStructValue = {10,10,0,10,10,0.0,0.0};
MyStruct MyStructValue;

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
//  EEPROM.put(eeAddress, MyStructValue);
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
int flag_vaso = 0 ;
boolean precio_vaso_1 = false ; 
void getVaso(){
    
      if(bandera_vaso != true){
      lcd.clear();    
      }
      if(flag_vaso == 0 ){
        lcd.setCursor(1,0);
      lcd.print("seleccione el");
      lcd.setCursor(0,1);
      lcd.print("vaso:");
      bandera_vaso = true;
      delay(1000);
      }
      
    
    
    
  
  if(!precio_vaso_1){
    valor_vaso = getTeclado();
    
    postPrecioVaso(valor_vaso);
  }
  getTeclado();
  
}
// switch para tamaño vaso y precio 
boolean vaso_limpiar = false;
int cerrar_precio = 0 ;
boolean cerrar_vaso = false ;
void postPrecioVaso(char x){
  if(!cerrar_vaso){
    bandera_precio_vaso = true;
  if(vaso_limpiar != true && flag_vaso==1){
    lcd.clear();
    vaso_limpiar=true;
  }
  
  switch (x){
    case '1':
    valor_vaso_definitivo =1 ;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(x);
    delay(1000);
    precio_vaso_1=true;
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("precio: 20"); money_vaso = 20.0;
    cerrar_vaso= true;
    flag_vaso = 1;
    delay(1500);
    flag_choose_icecream=true ;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sabor No. " + String(cont_sabor));
    break;
    case '2':
    valor_vaso_definitivo =2 ;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(x);
    delay(1000);
    precio_vaso_1=true;
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("precio: 30"); money_vaso = 30.0;
    flag_vaso = 1;
    delay(1500);
    cerrar_vaso= true;
    flag_choose_icecream=true ;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sabor No. " + String(cont_sabor));
    break;
    case '3':
    valor_vaso_definitivo = 3 ;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(x);
    delay(1000);
    precio_vaso_1=true;
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("precio: 40"); money_vaso = 40.0;
    flag_vaso = 1;
    delay(1500);
    cerrar_vaso= true;
    flag_choose_icecream=true ;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sabor No. " + String(cont_sabor));
    break;
        case '4':
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("error no");
    lcd.setCursor(0,1);
    lcd.print("existe vaso");
    delay(1000);
    lcd.clear();
    break; 
    case '5':
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("error no");
    lcd.setCursor(0,1);
    lcd.print("existe vaso");
    delay(1000);
    lcd.clear();
    break; 
    case '6':
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("error no");
    lcd.setCursor(0,1);
    lcd.print("existe vaso");
    delay(1000);
    lcd.clear();
    break; 
    case '7':
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("error no");
    lcd.setCursor(0,1);
    lcd.print("existe vaso");
    delay(1000);
    lcd.clear();
    break; 
    case '8':
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("error no");
    lcd.setCursor(0,1);
    lcd.print("existe vaso");
    delay(1000);
    lcd.clear();
    break; 
    case '9':
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("error no");
    lcd.setCursor(0,1);
    lcd.print("existe vaso");
    delay(1000);
    lcd.clear();
    break; 

  }
  }
  
}
/// reset 
void reset(){
      bandera_vaso = false ; 
    
    flag_vaso = 0 ;
    precio_vaso_1 = false ; 
    vaso_limpiar = false;
    
    cerrar_vaso = false ;
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
      choose_code(key);
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
     
      
      choose_code(key);
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


void choose_code(char key){
  
  if (flag_choose_icecream){
    
    if (cont_code<2) {
      if(cont_code == 0){ lcd.clear(); }
      lcd.setCursor(0,0);
      lcd.print("Sabor No. " + String(cont_sabor));
      lcd.setCursor(cont_code,1);
      lcd.print(key);
      conca_matrix += String(key);
      cont_code++;
      
    } else {
      lcd.setCursor(cont_code,1);
      lcd.print(key);
      conca_matrix += String(key);
      cont_code=0;
      switch_code();
    }
  }
}


void switch_code(){
  if(flag_choose_icecream){
    EEPROM.get(eeAddress, MyStructValue);
    switch(conca_matrix.toInt()){
      case 167: // sabor 1 
        if (MyStructValue.icecream0 == 0) {
          Serial.println("NO HAY EN INVENTARIO"); show_message_inventary_mistake();
        } else {
          Serial.println("HELADO 1: " + String(conca_matrix) + " | Stock: " + String(MyStructValue.icecream0)); cont_sabor++;
          MyStructValue.icecream0--; EEPROM.put(eeAddress, MyStructValue); // reasigno datos 
        }
        break;
      case 267: // sabor 2
        if (MyStructValue.icecream1 == 0) {
          Serial.println("NO HAY EN INVENTARIO"); show_message_inventary_mistake();
        } else {
          MyStructValue.icecream1--; EEPROM.put(eeAddress, MyStructValue); // reasigno datos 
          Serial.println("HELADO 2: " + String(conca_matrix) + " | Stock: " + String(MyStructValue.icecream1)); cont_sabor++;
        }
        break;
      case 367: // sabor 3
        if (MyStructValue.icecream2 == 0) {
          Serial.println("NO HAY EN INVENTARIO"); show_message_inventary_mistake();
          MyStructValue.icecream2--; EEPROM.put(eeAddress, MyStructValue); // reasigno datos 
        } else {
          Serial.println("HELADO 3: " + String(conca_matrix) + " | Stock: " + String(MyStructValue.icecream2)); cont_sabor++;
        }
        break;
      case 467: // sabor 4
        if (MyStructValue.icecream3 == 0) {
          Serial.println("NO HAY EN INVENTARIO"); show_message_inventary_mistake();
        } else {
          MyStructValue.icecream3--; EEPROM.put(eeAddress, MyStructValue); // reasigno datos 
          Serial.println("HELADO 4: " + String(conca_matrix) + " | Stock: " + String(MyStructValue.icecream3)); cont_sabor++;
        }
        break;
      case 567: // sabor 5
        if (MyStructValue.icecream4 == 0) {
          Serial.println("NO HAY EN INVENTARIO"); show_message_inventary_mistake();
        } else {
          MyStructValue.icecream4--; EEPROM.put(eeAddress, MyStructValue); // reasigno datos 
          Serial.println("HELADO 5: " + String(conca_matrix) + " | Stock: " + String(MyStructValue.icecream4)); cont_sabor++;
        }
        break;
      default:
        show_message_code_mistake();
        break;
    }
  }
  
  conca_matrix = "";
  delay(1000);
  lcd.clear();

  if (valor_vaso_definitivo < cont_sabor) {   
    cont_sabor = 1;
    flag_choose_icecream=false ;
    valor_vaso_definitivo = 0;
    Serial.println("Precio: " + String(money_vaso));
    EEPROM.get(eeAddress, MyStructValue); MyStructValue.money += money_vaso; EEPROM.put(eeAddress, MyStructValue); 
    money_vaso = 0.0;
    if(precio_vaso_1){
      reset();
    }
  } else {
      lcd.setCursor(0,0);
      lcd.print("Sabor No. " + String(cont_sabor));
    
  }
  
}



void show_message_code_mistake(){
  Serial.println("Codigo de Sabor: " + String(conca_matrix));
  lcd.setCursor(0,0);
  lcd.print("NO EXISTE ESE");
  lcd.setCursor(0,1);
  lcd.print("CODIGO DE SABOR");
}

void show_message_inventary_mistake(){
  Serial.println("NO HAY STOCK");
  lcd.setCursor(0,0);
  lcd.print("NO HAY STOCK");
  lcd.setCursor(0,1);
  lcd.print("DE ESE SABOR");
}
