#include <Wire.h>

//======================== CONFIGURACIONES RELOJ =========================
//Pines para multiplexación
const int Unidad = 8;
const int Decena = 9;
const int Centena = 10;
const int leds = 11;

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


//============================= CONFIGURACIONES ESCLAVO ====================================

unsigned int temp = 0;

//VARIABLE PARA RECIBIR DATOS
unsigned int datoRecibido = 0;

//Funcion que se ejecuta al recibir una peticion.
void respuestaEvento()                  
{
  Wire.write(temp);
}

//Funcion que se ejecuta al recibir datos.
void recibidoEvento(int recepcion)      
{

  while ( Wire.available())
  {
    datoRecibido = Wire.read();
  }
}

//=================================== SETUP ======================================
void setup() {
    //RELOJ
  DDRD = 255;
  pinMode(Unidad, OUTPUT);
  pinMode(Decena, OUTPUT);
  pinMode(Centena, OUTPUT);
  pinMode(leds, OUTPUT);
  
  //ESCLAVO
  Wire.begin(2);
  Wire.onRequest(respuestaEvento);
  Wire.onReceive(recibidoEvento);
  Serial.begin(9600);
  

  
}

//=================================== LOOP ======================================
void loop() {
  if (datoRecibido == 1)
  {
    reloj();
  }else{
    Serial.println("Apagando reloj");
  }
   
}

void reloj(){
    for (C = 0; C < 10; ++C) { //contador de centeneas
    for (D = 0; D < 6; ++D) { //contador de decenas
      for (U = 0; U < 10; ++U) { //contador de unidades

        for (int i = 0; i <= 10; ++i) { //contador para el tiempo de conteo

          //Encendiendo la unidad
          digitalWrite(Unidad, HIGH);
          digitalWrite(Decena, LOW);
          digitalWrite(Centena, LOW);
          digitalWrite(leds, HIGH);
          PORTD = Digitos[U];
         delay(20); //tiempo de multiplexacion

          //Encendiendo la decena
          digitalWrite(Unidad, LOW);
          digitalWrite(Decena, HIGH);
          digitalWrite(Centena, LOW);
          digitalWrite(leds, HIGH);
          PORTD = Digitos[D];
          delay(20); //tiempo de multiplexacion

          //Encendiendo la centena
          digitalWrite(Unidad, LOW);
          digitalWrite(Decena, LOW);
          digitalWrite(Centena, HIGH);
          digitalWrite(leds, LOW);
          PORTD = Digitos[C];
          delay(20); //tiempo de multiplexacion
        }
      }
    }
  }
}
