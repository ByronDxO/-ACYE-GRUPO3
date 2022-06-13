/* EEPROM */
#include <EEPROM.h>
/* Pantalla Led */
#include<LedControl.h>
#include <LiquidCrystal_I2C.h>
/* Teclado Matricial */
#include <Keypad.h>
/* Stepper */
#include <Stepper.h>

/* bluetooth */
#define bluethoot 8
/* sensor de temperatura */
#define pinA1 1

//Configuraciones del motor con driver
int stepsPerRevolution = 100;
int motSpeed = 4;
Stepper myStepper(stepsPerRevolution,10,11,12,13);



/* TIME */
unsigned long time_matrix = 0;
unsigned long contador_1  = 0; 
//Tiempor para ejecucion simultanea
unsigned long timeM1 = millis();
unsigned long timeM2 = millis();
long intervaloM1 = 1; //cambiar para que el motor vaya mas rapido o mas lento con millis().
long intervaloM2 = 1; //motor sin driver


// motor sin driver
const int a1 = 9;
const int a2 = 5;
const int b1 = 6;
const int b2 = 4;
int delayMotor = 40;

//variables para servir el helado
int tamanio = 0;
int bola1 = 0;
int bola2 = 0;
int bola3 = 0;
int posicion1 = 0;
int posicion2 = 0;
int posicion3 = 0;
int codH1 = 167;
int codH2 = 267;
int codH3 = 367;
int codH4 = 467;
int codH5 = 567;
//MOTORES DC
int motorDC = 52;
int dc2 = 53;
int dc3 = 17;
int dc4 = 16;
int dc5 = 15;
int retroceso = 0;
//LUCES
int led1 = 51;
int led2 = 50;
int led3 = 49;
int led4 = 48;
int led5 = 47;
//Sensor ultrasonico
long duracion;
long distancia; // float distancia para obtener decimales
int echo=14;
int trig=2;
//BUZZER
int buzz = 3;

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

  /* STTEPER */
  myStepper.setSpeed(motSpeed); //velocidad en rotaciones por minuto

  // initialize pins para el sin driver
  pinMode(a1, OUTPUT);
  pinMode(a2, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(b2, OUTPUT);
  //Motor dc
  pinMode(motorDC, OUTPUT);
  pinMode(dc2, OUTPUT);
  pinMode(dc3, OUTPUT);
  pinMode(dc4, OUTPUT);
  pinMode(dc5, OUTPUT);
  //leds
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  //Sensor ultrasonico
  pinMode(trig,OUTPUT); // emisor
  pinMode(echo,INPUT); // Receptor  
  //buzzer
  pinMode(buzz, OUTPUT);

  digitalWrite(a1, LOW);
  digitalWrite(a2, LOW);
  digitalWrite(b1, LOW);
  digitalWrite(b2, LOW);

  digitalWrite(motorDC, LOW);
  digitalWrite(dc2, LOW);
  digitalWrite(dc3, LOW);
  digitalWrite(dc4, LOW);
  digitalWrite(dc5, LOW);
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
          MyStructValue.icecream0--; EEPROM.put(eeAddress, MyStructValue); // reasigno datos 
          if (cont_sabor == 1) { bola1 = conca_matrix.toInt();
            Serial.print("Bola 1: "); Serial.println(conca_matrix.toInt()); 
          }else if(cont_sabor == 2){ bola2 = conca_matrix.toInt();
            Serial.print("Bola 2: "); Serial.println(conca_matrix.toInt());
          }else if(cont_sabor == 3){ bola3 = conca_matrix.toInt();
            Serial.print("Bola 3: "); Serial.println(conca_matrix.toInt());;
          }
          Serial.println("HELADO 1: " + String(conca_matrix) + " | Stock: " + String(MyStructValue.icecream0)); cont_sabor++;
        }
        break;
      case 267: // sabor 2
        if (MyStructValue.icecream1 == 0) {
          Serial.println("NO HAY EN INVENTARIO"); show_message_inventary_mistake();
        } else {
          MyStructValue.icecream1--; EEPROM.put(eeAddress, MyStructValue); // reasigno datos 
          if (cont_sabor == 1) { bola1 = conca_matrix.toInt();
            Serial.print("Bola 1: "); Serial.println(conca_matrix.toInt()); 
          }else if(cont_sabor == 2){ bola2 = conca_matrix.toInt();
            Serial.print("Bola 2: "); Serial.println(conca_matrix.toInt());
          }else if(cont_sabor == 3){ bola3 = conca_matrix.toInt();
            Serial.print("Bola 3: "); Serial.println(conca_matrix.toInt());;
          }
          Serial.println("HELADO 2: " + String(conca_matrix) + " | Stock: " + String(MyStructValue.icecream1)); cont_sabor++;
          
        }
        break;
      case 367: // sabor 3
        if (MyStructValue.icecream2 == 0) {
          Serial.println("NO HAY EN INVENTARIO"); show_message_inventary_mistake();
        } else {
          MyStructValue.icecream2--; EEPROM.put(eeAddress, MyStructValue); // reasigno datos 
          if (cont_sabor == 1) { bola1 = conca_matrix.toInt();
            Serial.print("Bola 1: "); Serial.println(conca_matrix.toInt()); 
          }else if(cont_sabor == 2){ bola2 = conca_matrix.toInt();
            Serial.print("Bola 2: "); Serial.println(conca_matrix.toInt());
          }else if(cont_sabor == 3){ bola3 = conca_matrix.toInt();
            Serial.print("Bola 3: "); Serial.println(conca_matrix.toInt());;
          }
          Serial.println("HELADO 3: " + String(conca_matrix) + " | Stock: " + String(MyStructValue.icecream2)); cont_sabor++;
        }
        break;
      case 467: // sabor 4
        if (MyStructValue.icecream3 == 0) {
          Serial.println("NO HAY EN INVENTARIO"); show_message_inventary_mistake();
        } else {
          MyStructValue.icecream3--; EEPROM.put(eeAddress, MyStructValue); // reasigno datos 
          if (cont_sabor == 1) { bola1 = conca_matrix.toInt();
            Serial.print("Bola 1: "); Serial.println(conca_matrix.toInt()); 
          }else if(cont_sabor == 2){ bola2 = conca_matrix.toInt();
            Serial.print("Bola 2: "); Serial.println(conca_matrix.toInt());
          }else if(cont_sabor == 3){ bola3 = conca_matrix.toInt();
            Serial.print("Bola 3: "); Serial.println(conca_matrix.toInt());;
          }
          Serial.println("HELADO 4: " + String(conca_matrix) + " | Stock: " + String(MyStructValue.icecream3)); cont_sabor++;
        }
        break;
      case 567: // sabor 5
        if (MyStructValue.icecream4 == 0) {
          Serial.println("NO HAY EN INVENTARIO"); show_message_inventary_mistake();
        } else {
          MyStructValue.icecream4--; EEPROM.put(eeAddress, MyStructValue); // reasigno datos 
          if (cont_sabor == 1) { bola1 = conca_matrix.toInt();
            Serial.print("Bola 1: "); Serial.println(conca_matrix.toInt()); 
          }else if(cont_sabor == 2){ bola2 = conca_matrix.toInt();
            Serial.print("Bola 2: "); Serial.println(conca_matrix.toInt());
          }else if(cont_sabor == 3){ bola3 = conca_matrix.toInt();
            Serial.print("Bola 3: "); Serial.println(conca_matrix.toInt());;
          }
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
    
    Serial.println("Precio: " + String(money_vaso));
    EEPROM.get(eeAddress, MyStructValue); MyStructValue.money += money_vaso; EEPROM.put(eeAddress, MyStructValue); 
    money_vaso = 0.0;
    tamanio = valor_vaso_definitivo;
    Serial.println("Bola 1: " + String(bola1));
    Serial.println("Bola 2: " + String(bola2));
    Serial.println("Bola 3: " + String(bola3));
    Serial.println("Tam: " + String(valor_vaso_definitivo));
    servirHelado();
    valor_vaso_definitivo = 0;
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

/* cinta transportadora */

void recorrerPos(){
  moverMotores();
  if(tamanio == 1 || tamanio == 2 || tamanio == 3){
    for(int i = 1; i <6; i++){
      if(posicion1 == i){
        encenderLed(i);
        seleccionarDC(i);
        apagarLed(i);
        if(tamanio == 2 || tamanio == 3){
          break;
        }
      }
      encenderLed(i);
      moverMotores();
    }
  }
  if(tamanio == 2 || tamanio == 3){
    if(posicion2 > posicion1){
      for(int i = posicion1; i < 6; i++){ //int i = posicion1+1
        if(posicion2 == i){
          encenderLed(i);
          seleccionarDC(i);
          apagarLed(i);
          if(tamanio == 3){
            break;
          }
        }
        encenderLed(i);
        moverMotores();
      }
    }else if(posicion2 == posicion1){
      for(int i = posicion1; i < 6; i++){
        if(posicion2 == i){
          encenderLed(i);
          seleccionarDC(i);
          apagarLed(i);
          if(tamanio == 3){
            break;
          }
      }
      encenderLed(i);
      moverMotores();
      }
    }else if(posicion2 < posicion1){
      retroceso = posicion1 - posicion2;
      for(int i = 1; i <= retroceso; i++){
        int ledEnciende = posicion1-i;
        moverMotoresRetroceso(ledEnciende);
        apagarLed2();
      }
      for(int i = posicion2; i < 6; i++){
        if(posicion2 == i){
          encenderLed(i);
          seleccionarDC(i);
          apagarLed(i);
          if(tamanio == 3){
            break;
          }
        }
        encenderLed(i);
        moverMotores();
      }
    }
  } //continuar con el tamaño 3
  if(tamanio == 3){
    if(posicion3 > posicion2){
      for(int i = posicion2;i<6;i++){
        if(posicion3 == i){
          encenderLed(i);
          seleccionarDC(i);
          apagarLed(i);
        }
        encenderLed(i);
        moverMotores();
      }
    }else if(posicion3 == posicion2){
      for(int i = posicion2; i < 6; i++){
        if(posicion3 == i){
          encenderLed(i);
          seleccionarDC(i);
          apagarLed(i);
        }
        encenderLed(i);
        moverMotores();
      }
    }else if(posicion3 < posicion2){
      retroceso = posicion2 - posicion3;
      for(int i = 1; i <= retroceso;i++){
        int ledEnciende = posicion2-i;
        moverMotoresRetroceso(ledEnciende);
        apagarLed2();
      }
      for(int i = posicion3; i < 6; i++){
        if(posicion3 == i){
          encenderLed(i);
          seleccionarDC(i);
          apagarLed(i);
        }
        encenderLed(i);
        moverMotores();
      }
    }
  }
  entregar();
  tamanio = 0;
  bola1 = 0;
  bola2 = 0;
  bola3 = 0;
}

void seleccionarDC(int num){
  unsigned long tiempo1 = millis();
  unsigned long tiempo2 = millis();

  while (true) {
    unsigned long currentTimeM1 = millis();
    unsigned long currentTimeM2 = millis();
    if(currentTimeM1 - timeM1 > intervaloM1){
        if(num == 1){
          digitalWrite(motorDC, HIGH);
        }else if(num == 2){
          digitalWrite(dc2, HIGH);
        }else if(num == 3){
          digitalWrite(dc3, HIGH);
        }else if(num == 4){
          digitalWrite(dc4, HIGH);
        }else if(num == 5){
          digitalWrite(dc5, HIGH);
        }
      
      timeM1 = currentTimeM1;
    }
    tiempo1 = millis();
    if (tiempo1 >= (tiempo2 + 5000)) {
      tiempo1 = 0;
      break;
    }
  }

  if(num == 1){
    digitalWrite(motorDC, LOW);
  }else if(num == 2){
    digitalWrite(dc2, LOW);
  }else if(num == 3){
    digitalWrite(dc3, LOW);
  }else if(num == 4){
    digitalWrite(dc4, LOW);
  }else if(num == 5){
    digitalWrite(dc5, LOW);
  }

}

void encenderLed(int num){
  if(num == 1){
    digitalWrite(led1, HIGH);
  }else if(num == 2){
    digitalWrite(led2, HIGH);
  }else if(num == 3){
    digitalWrite(led3, HIGH);
  }else if(num == 4){
    digitalWrite(led4, HIGH);
  }else if(num == 5){
    digitalWrite(led5, HIGH);
  }
}

void apagarLed(int num){
  if(num == 1){
    digitalWrite(led1, LOW);
  }else if(num == 2){
    digitalWrite(led2, LOW);
  }else if(num == 3){
    digitalWrite(led3, LOW);
  }else if(num == 4){
    digitalWrite(led4, LOW);
  }else if(num == 5){
    digitalWrite(led5, LOW);
  }
}

void apagarLed2(){
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);
}

void moverMotoresRetroceso(int num){
  unsigned long tiempo1 = millis();
  unsigned long tiempo2 = millis();

  while (true) {
    unsigned long currentTimeM1 = millis();
    unsigned long currentTimeM2 = millis();
    if(currentTimeM1 - timeM1 > intervaloM1){
      myStepper.step(-motSpeed);
      timeM1 = currentTimeM1;
    }
  
    if(currentTimeM2 - timeM2 > intervaloM2){
      funcionMotorRetroceso();
      timeM2 = currentTimeM2;
    }

    tiempo1 = millis();
    if (tiempo1 >= (tiempo2 + 5000)) {
      tiempo1 = 0;
      break;
    }
    if (tiempo1 >= (tiempo2 + 4000)) {
      encenderLed(num);
    }
  }
}

void moverMotores(){  
  unsigned long tiempo1 = millis();
  unsigned long tiempo2 = millis();

  while (true) {
    unsigned long currentTimeM1 = millis();
    unsigned long currentTimeM2 = millis();
    if(currentTimeM1 - timeM1 > intervaloM1){
      myStepper.step(motSpeed);
      timeM1 = currentTimeM1;
    }
  
    if(currentTimeM2 - timeM2 > intervaloM2){
      funcionMotor();
      timeM2 = currentTimeM2;
    }

    tiempo1 = millis();
    if (tiempo1 >= (tiempo2 + 5000)) {
      tiempo1 = 0;
      break;
    }
    if (tiempo1 >= (tiempo2 + 1000)) {
      apagarLed2();
    }
  }
}

void funcionMotorRetroceso(){
  step4();
  delay(delayMotor);
  //temporizador2(delayMotor);
  step2();
  delay(delayMotor);
  //temporizador2(delayMotor);
  step3();
  delay(delayMotor);
  //temporizador2(delayMotor);
  step1();
  delay(delayMotor);
  //temporizador2(delayMotor);
  step4();
  delay(delayMotor);
  //temporizador2(delayMotor);
  step2();
  delay(delayMotor);
  //temporizador2(delayMotor);
  step3();
  delay(delayMotor);
  //temporizador2(delayMotor);
  step1();
  delay(delayMotor);
  //temporizador2(delayMotor);
}

void servirHelado(){
  if(tamanio == 1){
    if (bola1 == codH1){
      posicion1 = 1;
    }else if (bola1 == codH2){
      posicion1 = 2;
    }else if(bola1 == codH3){
      posicion1 = 3;
    }else if(bola1 == codH4){
      posicion1 = 4;
    }else if(bola1 == codH5){
      posicion1 = 5;
    }
    posicion2 = 0;
    posicion3 = 0;
  }
  if(tamanio == 2){
    if (bola1 == codH1){
      posicion1 = 1;
    }else if (bola1 == codH2){
      posicion1 = 2;
    }else if(bola1 == codH3){
      posicion1 = 3;
    }else if(bola1 == codH4){
      posicion1 = 4;
    }else if(bola1 == codH5){
      posicion1 = 5;
    }

    if (bola2 == codH1){
      posicion2 = 1;
    }else if (bola2 == codH2){
      posicion2 = 2;
    }else if(bola2 == codH3){
      posicion2 = 3;
    }else if(bola2 == codH4){
      posicion2 = 4;
    }else if(bola2 == codH5){
      posicion2 = 5;
    }
    posicion3 = 0;
  }

  if(tamanio == 3){
    if (bola1 == codH1){
      posicion1 = 1;
    }else if (bola1 == codH2){
      posicion1 = 2;
    }else if(bola1 == codH3){
      posicion1 = 3;
    }else if(bola1 == codH4){
      posicion1 = 4;
    }else if(bola1 == codH5){
      posicion1 = 5;
    }

    if (bola2 == codH1){
      posicion2 = 1;
    }else if (bola2 == codH2){
      posicion2 = 2;
    }else if(bola2 == codH3){
      posicion2 = 3;
    }else if(bola2 == codH4){
      posicion2 = 4;
    }else if(bola2 == codH5){
      posicion2 = 5;
    }

    if (bola3 == codH1){
      posicion3 = 1;
    }else if (bola3 == codH2){
      posicion3 = 2;
    }else if(bola3 == codH3){
      posicion3 = 3;
    }else if(bola3 == codH4){
      posicion3 = 4;
    }else if(bola3 == codH5){
      posicion3 = 5;
    }
  }
  recorrerPos();
}

void funcionMotor()
{
  // MOTOR STEPPER - CERRAR
  step1();
  delay(delayMotor);
  //temporizador2(delayMotor);
  step3();
  delay(delayMotor);
  //temporizador2(delayMotor);
  step2();
  delay(delayMotor);
  //temporizador2(delayMotor);
  step4();
  delay(delayMotor);
  //temporizador2(delayMotor);
  step1();
  delay(delayMotor);
  //temporizador2(delayMotor);
  step3();
  delay(delayMotor);
  //temporizador2(delayMotor);
  step2();
  delay(delayMotor);
  //temporizador2(delayMotor);
  step4();
  delay(delayMotor);
  //temporizador2(delayMotor);
  //step1();
  //exit(0);
}

void step1()
{

  digitalWrite(a1, HIGH);
  digitalWrite(a2, LOW);
  digitalWrite(b1, LOW);
  digitalWrite(b2, LOW);
}

void step2()
{

  digitalWrite(a1, LOW);
  digitalWrite(a2, LOW);
  digitalWrite(b1, HIGH);
  digitalWrite(b2, LOW);
}

void step3()
{

  digitalWrite(a1, LOW);
  digitalWrite(a2, HIGH);
  digitalWrite(b1, LOW);
  digitalWrite(b2, LOW);
}

void step4()
{

  digitalWrite(a1, LOW);
  digitalWrite(a2, LOW);
  digitalWrite(b1, LOW);
  digitalWrite(b2, HIGH);
}

void entregar(){
  unsigned long tiempo1 = millis();
  unsigned long tiempo2 = millis();

  while (true) {
    unsigned long currentTimeM1 = millis();
    unsigned long currentTimeM2 = millis();
    if(currentTimeM1 - timeM1 > intervaloM1){
      obtenerDistancia();
      timeM1 = currentTimeM1;
    }
  
    if(currentTimeM2 - timeM2 > intervaloM2 && distancia > 200){
      moverMotores();
      timeM2 = currentTimeM2;
    }

    tiempo1 = millis();
    if (distancia < 200) {
      tiempo1 = 0;
      while(distancia < 200){
        tone(buzz, 1000,500);  //1000hz
        //temporizador(1);
        //noTone(buzz);
        obtenerDistancia();
        lcd.setCursor(0,0);
        lcd.print("Servido,");
        lcd.setCursor(0,1);
        lcd.print("Buen dia");
      }
      noTone(buzz);
      lcd.clear();
      break;
    }
  }
}

void temporizador(int tam){ //tam es el numero de segundos en que se establece el temporizador.
  int temporal = millis() / 1000;
  int tiempo = 0;
  while(tiempo <= temporal + tam){
    tiempo = millis() / 1000;
  }
}
void temporizador2(int tam){ //tam es el numero de segundos en que se establece el temporizador.
  int temporal = millis();
  int tiempo = 0;
  while(tiempo <= temporal + tam){
    tiempo = millis();
  }
}

void obtenerDistancia(){
  //Para estabilizar nuestro módulo ultrasónico
  digitalWrite(trig,LOW);
  delayMicroseconds(4);
  //disparo de un pulso en el trigger de longitud 10us
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);

  //Lectura de la duración del pulso HIGH generado hasta recibir el Echo
  duracion=pulseIn(echo,HIGH);

  //Calculo distancia
  distancia=duracion/58.4;// (cm)
}
