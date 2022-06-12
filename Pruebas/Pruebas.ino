#include <Stepper.h>

//Configuraciones del motor con driver
int stepsPerRevolution = 100;
int motSpeed = 4;
Stepper myStepper(stepsPerRevolution,10,11,12,13);

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

void setup(){
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

void loop(){
  tamanio = 3;
  bola1 = 333;
  bola2 = 333;
  bola3 = 333;
  
  servirHelado();
}

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
      }
      noTone(buzz);
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
