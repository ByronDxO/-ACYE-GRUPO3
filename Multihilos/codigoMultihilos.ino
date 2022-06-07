#include <MatrizLed.h>

#include <Key.h>

#include <Keypad.h>

#include <LiquidCrystal.h>

#include <Stepper.h>

#include <Servo.h>

#include<LedControl.h>

#include<pt.h>


//Hilos para la ejecucion de funcionalidades
struct pt hilo1; // matrices
struct pt hilo2; // pantalla lcd
struct pt hilo3; // banda transportadora

/*  USAR PARA DETENER UN PROCESO TEMPORALMENTE, NO USAR DELY
 *  t = millis();
    PT_WAIT_UNTIL(pt, (millis()-t)>=1000);
 * 
 */
unsigned long previousMillis = 1000;
long interval = 3000;

//================ RELOJ ================
//Pines para multiplexación
const int Unidad = 50;
const int Decena = 51;
const int Centena = 52;
const int leds = 53;

//Array de numeros de 0 a 9
char Digitos[] = {
  0x7E,
  0x30,
  0x6D,
  0x79,
  0x33,
  0x5B,
  0x5F,
  0x70,
  0x7F,
  0x7B
};

// Contandores
int U = 0; //Unidad
int D = 0; //Decena
int C = 0; //Centena

//================ KEYPAD ================
const byte rows = 4;
const byte cols = 3;
bool contador_keypad = false;
char keys[rows][cols] = {
  {
    '1',
    '2',
    '3'
  },
  {
    '4',
    '5',
    '6'
  },
  {
    '7',
    '8',
    '9'
  },
  {
    '*',
    '0',
    '#'
  }
};

byte rowPins[rows] = {
  30,
  31,
  32,
  33
};
byte colPins[cols] = {
  34,
  35,
  36
};
Keypad pad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

//WALTER
const int spr = 5;
Stepper stepper(spr, 7, 8, 9, 10);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//configuraciones de una matriz de leds
//se crea un objeto de tipo pantalla para cada matriz
MatrizLed pantalla;
MatrizLed pantalla1;
MatrizLed pantalla2;
MatrizLed pantalla3;

int row = 0;
int col = 0;

char c = 0;

//**************************DECLARACIONES PARA LA CINTA TRANSPORTADORA******************************
/*MOTOR STEPPER BARRA TRANSPORTADORA*/
String mensajeCintaTransportadora = "MOVIENDOSE";
bool validacionMotor = false;

// en mensaje se guarda la cadena de botones a encender desde la app

bool entrar;

void setup() {
  PT_INIT( & hilo1);
  PT_INIT( & hilo2);
  PT_INIT( & hilo3);

  Serial.begin(9600);

  lcd.begin(16, 2);
  stepper.setSpeed(60);
  // pines controladores para la matriz
  pantalla.begin(15, 14, 16, 1); // dataPin, clkPin, csPin, numero de matrices de 8x8
  pantalla.rotar(true);
  pantalla1.begin(17, 14, 16, 1); // dataPin, clkPin, csPin, numero de matrices de 8x8
  pantalla1.rotar(true);
  pantalla2.begin(20, 14, 16, 1); // dataPin, clkPin, csPin, numero de matrices de 8x8
  pantalla2.rotar(true);
  pantalla3.begin(21, 14, 16, 1); // dataPin, clkPin, csPin, numero de matrices de 8x8
  pantalla3.rotar(true);
  //RELOJ
  DDRA = B11111111;
  pinMode(Unidad, OUTPUT);
  pinMode(Decena, OUTPUT);
  pinMode(Centena, OUTPUT);
  pinMode(leds, OUTPUT);
}

void loop() {
  EJECUCIONMATRIZ( & hilo1);
  BANDATRANSPORTADORA( & hilo2);

}

//variables para controlar el flujo del programa
bool dibujoGrabado = false;
bool inicioDelPrograma = false;

//HILO PARA LA EJECUCION DE LA MATRIZ DE LEDS
void EJECUCIONMATRIZ(struct pt * pt) {
  PT_BEGIN(pt);
    // void setup() {
  //matriz para almacenar los leds que deben encenderse segun el dibujo del usuario
  static int matriz[8][8] = {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}};
  Serial1.begin(9600);

  static String mensaje = "";
  static String dibujandoMatriz = "";
  static int numeroPantalla = 0;
  static MatrizLed pantallas;
  static int temporizador = 0;


  static long t = 0;
  unsigned long currentMillis = 0;

  // }
  do {

    // void loop() {
    if (!inicioDelPrograma) {
      Serial.println("Esperando contrasenia");
      ejecutarContra();

    }

    if (!dibujoGrabado && inicioDelPrograma == true) {
      desplazarFraseDerecha();
      while (Serial1.available() > 0) {
        c = Serial1.read();
        mensaje += c;
      }
      /****************CODIGO PARA GUARDAR EL DIBUJO ENVIADO DESDE LA APP***********************/
      Serial.println(mensaje);
      int contador = 1;
      for (int i = 0; i < mensaje.length(); i++) {
        if (contador == 1) {
          //se lee la fila
          int convertido = String(mensaje.charAt(i)).toInt();
          row = convertido;
          contador++; //2
        } else if (contador = 2) {
          int convertido = String(mensaje.charAt(i)).toInt();
          col = convertido;
          //se agrega un uno en la posicion matriz[fila][columna] para indicar que se debe encender
          matriz[row][col] = 1;
          String pos="fila: " +(String)row + "col: "+ (String)col; 
          Serial.println(pos);
          contador = 1;
        }
      }
      
      mensaje = "";
      /****************************************************************************************/
      dibujoGrabado = true;
      Serial.println("dibujo guardado");
    } else if (inicioDelPrograma && dibujoGrabado) {

      if (Serial1.available()) {
        c = Serial1.read();

        //si encaso viene a, b, c esto significa qe se activa una estacion de lo contrario enciende los led
        if (c == 'a') {
          Serial.println("guarda pantalla1");
          //dibujandoMatriz = "pantalla1";
          numeroPantalla = 1;
          //pantallas=pantalla1;

        } else if (c == 'b') {
          Serial.println("guarda pantalla2");
          //dibujandoMatriz = "pantalla2";
          numeroPantalla = 2;
          //pantallas=pantalla1;

        } else if (c == 'c') {
          Serial.println("guarda pantalla3");
          //dibujandoMatriz = "pantalla3";
          //pantallas=pantalla1;
          numeroPantalla = 3;
          //delay(1);
        } else {
          if (c == 'y') {
            validacionMotor = true;
            t = millis();
            PT_WAIT_WHILE(pt, (millis() - t) < 10);
          }
          if (c == 'z') {
            validacionMotor = false;
            t = millis();
            PT_WAIT_WHILE(pt, (millis() - t) < 10);
          }
        }
      }

      /************************************ SECCION DE CODIGO PARA MOSTRAR EN LAS ESTACIONES EL DIBUJO AL 35,70 Y 100% DE LA PRODUCCION *******************************************/
      //se selecciona la matriz que se estara encendiendo

      switch (numeroPantalla) {
        //se ejecuta el proceso de la pantalla 1 
      case 1:
        // enciende a 35 por ciento
        pantalla.escribirCaracter('1', -8);
        for (int x = 0; x < 8; x++) { //recorrer las filas
          for (int y = 0; y < 3; y++) { // recorrer las columnas
            if (matriz[x][y] == 1) {
              pantalla1.setLed(0, x, y, true);
            }
          }
        }

        while (temporizador <= 100) {
          t = millis();
          temporizador = temporizador + 1;
          PT_WAIT_UNTIL(pt, (millis() - t) >= 1);
        }
        temporizador = 0;
        //**************************Enciende a 70 por ciento*****************
        pantalla1.borrar();
        for (int x = 0; x < 8; x++) { //recorrer las filas
          for (int y = 0; y < 6; y++) { // recorrer las columnas
            if (matriz[x][y] == 1) {
              pantalla1.setLed(0, x, y, true);
            }
          }
        }

        while (temporizador <= 100) {
          t = millis();
          temporizador = temporizador + 1;
          PT_WAIT_UNTIL(pt, (millis() - t) >= 1);
        }
        temporizador = 0;

        //************************Enciende a 100 por ciento*******************
        pantalla.borrar();
        for (int x = 0; x < 8; x++) { //recorrer las filas
          for (int y = 0; y < 8; y++) { // recorrer las columnas
            if (matriz[x][y] == 1) {
              pantalla1.setLed(0, x, y, true);
            }
          }
        }
        case 2:
          pantalla.escribirCaracter('2', -8);
          // enciende a 35 por ciento
          for (int x = 0; x < 8; x++) { //recorrer las filas
            for (int y = 0; y < 3; y++) { // recorrer las columnas
              if (matriz[x][y] == 1) {
                pantalla2.setLed(0, x, y, true);
              }
            }
          }
          while (temporizador <= 100) {
            t = millis();
            temporizador = temporizador + 1;
            PT_WAIT_UNTIL(pt, (millis() - t) >= 1);
          }
          temporizador = 0;
          //**************************Enciende a 70 por ciento*****************
          for (int x = 0; x < 8; x++) { //recorrer las filas
            for (int y = 0; y < 6; y++) { // recorrer las columnas
              if (matriz[x][y] == 1) {
                pantalla2.setLed(0, x, y, true);
              }
            }
          }
          while (temporizador <= 100) {
            t = millis();
            temporizador = temporizador + 1;
            PT_WAIT_UNTIL(pt, (millis() - t) >= 1);
          }
          temporizador = 0;
          //************************Enciende a 100 por ciento*******************
          pantalla.borrar();
          for (int x = 0; x < 8; x++) { //recorrer las filas
            for (int y = 0; y < 8; y++) { // recorrer las columnas
              if (matriz[x][y] == 1) {
                pantalla2.setLed(0, x, y, true);
              }
            }
          }
          break;
        case 3:
          pantalla.escribirCaracter('3', -8);
          // enciende a 35 por ciento
          for (int x = 0; x < 8; x++) { //recorrer las filas
            for (int y = 0; y < 3; y++) { // recorrer las columnas
              if (matriz[x][y] == 1) {
                pantalla3.setLed(0, x, y, true);
              }
            }
          }
          while (temporizador <= 100) {
            t = millis();
            temporizador = temporizador + 1;
            PT_WAIT_UNTIL(pt, (millis() - t) >= 1);
          }
          temporizador = 0;
          //**************************Enciende a 70 por ciento*****************
          for (int x = 0; x < 8; x++) { //recorrer las filas
            for (int y = 4; y < 6; y++) { // recorrer las columnas
              if (matriz[x][y] == 1) {
                pantalla3.setLed(0, x, y, true);
              }
            }
          }
          while (temporizador <= 100) {
            t = millis();
            temporizador = temporizador + 1;
            PT_WAIT_UNTIL(pt, (millis() - t) >= 1);
          }
          temporizador = 0;
          //************************Enciende a 100 por ciento*******************
          pantalla.borrar();
          for (int x = 0; x < 8; x++) { //recorrer las filas
            for (int y = 6; y < 8; y++) { // recorrer las columnas
              if (matriz[x][y] == 1) {
                pantalla3.setLed(0, x, y, true);
              }
            }
          }

          break;
      }
    }
    // }
  } while (true);

  PT_END(pt);
}
// Hilo para la banda transportado..
void BANDATRANSPORTADORA(struct pt * pt) {
  PT_BEGIN(pt);
  // void setup() {

  static long t = 0;
  //}
  do {
    // void loop() {
    if (validacionMotor) {
      stepper.step(spr);
      t = millis();
      PT_WAIT_WHILE(pt, (millis() - t) <= 1);
    } else {
      t = millis();
      PT_WAIT_WHILE(pt, (millis() - t) <= 1);
    }

  } while (true);
  PT_END(pt);
}
//=========================== INGRESO DE EMPLEADOS ===========================
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

//================================ RELOJ =====================================
void iniciarReloj() {
  for (C = 0; C < 10; ++C) { //contador de centeneas   
    for (D = 0; D < 6; ++D) { //contador de decenas
      for (U = 0; U < 10; ++U) { //contador de unidades

        for (int i = 0; i <= 10; ++i) { //contador para el tiempo de conteo

          //Encendiendo la unidad
          digitalWrite(Unidad, HIGH);
          digitalWrite(Decena, LOW);
          digitalWrite(Centena, LOW);
          digitalWrite(leds, HIGH);
          PORTA = Digitos[U];
          delay(20); //tiempo de multiplexacion

          //Encendiendo la decena
          digitalWrite(Unidad, LOW);
          digitalWrite(Decena, HIGH);
          digitalWrite(Centena, LOW);
          digitalWrite(leds, HIGH);
          PORTA = Digitos[D];
          delay(20); //tiempo de multiplexacion

          //Encendiendo la centena
          digitalWrite(Unidad, LOW);
          digitalWrite(Decena, LOW);
          digitalWrite(Centena, HIGH);
          digitalWrite(leds, LOW);
          PORTA = Digitos[C];
          delay(20); //tiempo de multiplexacion
        }
      }
    }
  }
}

void escribirFraseDesp(const char * frase, int posicion) {
  int caracter = strlen(frase) - 1;
  int pos = 0;
  for (int i = 0; i < strlen(frase); i++) {
    pos = (i * 8) + posicion;
    //se colocan las pantallas para que se escrban en las matrices
    pantalla1.escribirCaracter(frase[caracter], pos);
    pantalla2.escribirCaracter(frase[caracter], pos);
    pantalla3.escribirCaracter(frase[caracter], pos);
    caracter--;
  }
}

//

// Metodo para desplazar a la derecha
void desplazarFraseDerecha() {
  pantalla.borrar();
  for (int i = 0; i < 400; i++) {
    escribirFraseDesp("5", i - 8);
    escribirFraseDesp("G", i - 16);
    escribirFraseDesp(" ", i - 24);
    escribirFraseDesp("1", i - 32);
    escribirFraseDesp(" ", i - 40);
    escribirFraseDesp("E", i - 48);
    escribirFraseDesp("Y", i - 56);
    escribirFraseDesp("C", i - 64);
    escribirFraseDesp("A", i - 72);

  }
}
