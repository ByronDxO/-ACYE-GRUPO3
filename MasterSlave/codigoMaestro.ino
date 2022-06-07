#include <Wire.h> 
#include <MatrizLed.h>
#include <Key.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Stepper.h>
#include <Servo.h>
#include<LedControl.h>

unsigned long previousMillis = 1000;
long interval = 3000;

//================ KEYPAD ================
const byte rows = 4;
const byte cols = 3;
bool contador_keypad = false;
char keys[rows][cols] = {{'1','2','3'},{'4','5','6'},{'7','8','9'},{'*','0','#'}};

byte rowPins[rows] = {30,31,32,33};
byte colPins[cols] = {34,35,36};
Keypad pad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

//WALTER
const int spr = 5;
Stepper stepper(spr, 7, 8, 9, 10);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//**************************DECLARACIONES PARA LA CINTA TRANSPORTADORA******************************
/*MOTOR STEPPER BARRA TRANSPORTADORA*/
String mensajeCintaTransportadora = "MOVIENDOSE";
bool validacionMotor = false;
bool inicioDelPrograma=false;
bool dibujoRealizado=false;
String cadenaDibujo="";
int contadorEstacionesCompletas=0;
// en mensaje se guarda la cadena de botones a encender desde la app

bool entrar;
char c;
void setup() {
  lcd.begin(16, 2);
  stepper.setSpeed(60);
  Serial1.begin(9600);
  Serial.begin(9600);
  Wire.begin();    
  // pines controladores para la matriz

}

void loop() {
 if (!inicioDelPrograma) {
      Serial.println("Esperando contrasenia");
      ejecutarContra();
      
 }else{
     Wire.beginTransmission(2);// encender el reloj
     Wire.write(1);
     Wire.endTransmission();
     delay(10);
    if(!dibujoRealizado){ 
      //Recibe el string para guardar el dibujo en la matriz
         while (Serial1.available() > 0) {
          c = Serial1.read();      
          Wire.beginTransmission(1);// encender el reloj
          Wire.write(c);
          Wire.endTransmission();
          
        }
        if(c=='w'){
          int numero=cadenaDibujo.toInt();
     
            dibujoRealizado=true;
            Wire.beginTransmission(1);
            Wire.write(numero);
            //Wire.write(cadena);
            Wire.endTransmission();
        }
      }

      if(Serial1.available()){
        c=Serial1.read();
      }
      Serial.print("caracter: ");
      Serial.println(c);
      if(c=='2'){
        lcd.clear();
        lcd.print("moviendose");
         validacionMotor=true;
      }
      else if(c=='1'){
        lcd.clear();
        validacionMotor=false;
      }
      else if(c=='a'){
        //activar estacion 1
        Wire.beginTransmission(1);
        Wire.write('a');
        Wire.endTransmission();
        contadorEstacionesCompletas++;
        
      }
      else if(c=='b'){
        //activar estacion 2
        Wire.beginTransmission(1);
        Wire.write('b');
        Wire.endTransmission();
        contadorEstacionesCompletas++;
        
      }else if(c=='c'){
        //activar estacion 3
        Wire.beginTransmission(1);
        Wire.write('c');
        Wire.endTransmission();
        contadorEstacionesCompletas++;
      }
      else if(contadorEstacionesCompletas==3){
        contadorEstacionesCompletas=0;
        Wire.beginTransmission(2);// apagar el reloj
        Wire.write(0);
        Wire.endTransmission();
      } 
      if(validacionMotor){
        stepper.step(spr);
      }
    
   }
}

void escribirContra() {
  String contra_pad = "";

  while (true) {
    char caracter = pad.getKey();
    if (caracter == '#') {
      break;
    }

    if (caracter != NO_KEY) {
      contra_pad += caracter;
      Serial.println(caracter);
      //delay(1);
    }
  }

  if (contra_pad == "123456") {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("CORRECTA");
    Serial1.println("1");
    inicioDelPrograma = true;
  } else {
    lcd.clear();
    lcd.setCursor(6, 0);
    lcd.print("ERROR");
    contra_pad = "";
    Serial1.println("0");

  }
}


void ejecutarContra() {
  if (contador_keypad == false) {
    lcd.setCursor(3, 0);
    lcd.print("INGRESE SU");
    lcd.setCursor(5, 1);
    lcd.print("USUARIO");
    contador_keypad = true;
  } else {
    escribirContra();
  }

}
