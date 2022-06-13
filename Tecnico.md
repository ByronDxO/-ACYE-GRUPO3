## Cinta transportadora

### Configuraciones del motor con driver
```C
#include <Stepper.h>

int stepsPerRevolution = 100;
int motSpeed = 4;
Stepper myStepper(stepsPerRevolution,10,11,12,13);
```

Para mover el motor con driver se utilizó la libreria **Stepper.h** que facilita bastante el control de los motores stepper. Para utilizarlo se necesitan de dos variables importantes, el primero es los pasos que da por cada revolucion y el segundo es la velocidad en que se mueve el motor.
Se debe de crear una instancia de la libreria Stepper.h ingresando los parametros de pasosPorRevolucion y los pines que controlaran al motor por medio del controlador **L293D**.

![Motor](img/motor.png)

### Configuraciones para el motor sin diver
Para el motor sin driver se conectaron 4 pines directamente del Arduino al motor stepper para controlarlo. Estos se definieron el el archivo .ino junto con una variable que dentendra un breve tiempo la ejecucion para dar los pasos de forma correcta.

![Pines](img/motorSinD.png)

Luego de declarar los pines se deben inicializar en la parte del setup, indicando que serán pines de salida.

![PinSal](img/iniPinM.png)

### Configuraciones para los motores DC y sus luces led
Para controlar los motores DC y las luces led que lo acompañan, primero se declararon los pines que cada uno de ellos utilizaron.

![ledDC](img/dcLed.png)

Luego de declarar los pines se debe indicar que serán pines de salida en la parte del setup.

![LedDc](img/confLedDc.png)

### Configuraciones para el sensor ultrasonico y buzzer
Primero se declararon los pines que se utilizarían para cada uno de los dispositivos. Luego se indico en la parte del setup que define si los pines son de entrada o salida. Tambien se definen las variables que controlan los pulsos que manda el sensor ultrasonico y la distancia que recorre cada pulso

![BuzzerSensor](img/buzzSenso.png)

![BuzzConf](img/confSenso.png)

## Metodos para mover la cinta transportadora

### Recorrer posición
Para recorrer las posiciones por dond la cinta debe de pasar se primero se mueven los motores para simular el arrivo a la primera estacion, luego comienza a diferenciar los ciclos que se deben realizar dependiendo del tamaño del vaso que se seleccione, por cada ciclo (de 1 a 5) que se repita, el motor se moverá por 5 segundos hasta llegar a la siguiente estacion donde comprobará por medio de un if si la posicion del helado corresponde a la posicion del ciclo, si corresponde se prende una luz led y también comienza a girar el motor DC que simula que se esta sirviendo el helado. El motor y la led se deben de prender por 5 segundos para luego continuar con la siguiente estacion.
Este ciclo se repite dependiendo del tamaño del vaso que se desea comprar(una vez para el tamaño 1, 2 veces para el tamaño 2 y 3 veces para el tamaño 3).

![Recorrer pos](img/recorrerPos.png)

Al finalizar este metodo se llama al método entregar para finalizar con la ejecución.

### Seleccionar DC
En este metodo se selecciona el motor DC que se activará dependiendo de la posicion en que se encuentre el ciclo, manteniendolo encendido por 5 segundos y luego se apaga.
Se utiliza la funcion millis() para medir el tiempo que se mantine encendido el motor y se evita el uso de delay que detiene la ejecución de todos los porcesos.

![SeleccionarDC](img/seleccionarDC.png)

### Encender led
Este método enciende el pin de arduino dependiendo del valor que reciba como parametro, con el método digitalWrite que recibe como parametro el numero del pin de arduino y el estado que le asignamos ya sea high o low.

```C
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
```

### Apagar led
Este método apaga un pin digital de arduino dependiendo del numero que reciba como parametro, con em metodo digitalWrite pasandole como parametro el número del pin y el estado que deseamos.

```C
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
```

### Retroceso de motores
Este metodo se utiliza para mover los motores en reversa, utilizando la función millis() para poder mover los dos motores a la vez por 5 segundos. 
Recibe como parametro un numero que es el identificador de la led por el cual esta pasando mientras retrocede. Al ejecutarse por 5 segundos se detiene el ciclo y continua con las siguientes instrucciones.

```C
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
```

### Mover motores
Este metodo se utiliza para mover los motores hacia adelante, haciendo uso de la función millis() para poder mover los dos motores a la vez por 5 segundos.
Cuando ha pasado 1 segundo todas las luces led de los motores DC se apagan.

```C
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
```

### Pasos para el retroceso del motor
En este método se ejecutan los pasos necesarios para que el motor sin controlador sea capaz de moverse, ejecutando un pequeño delay entre cada paso para que se realice cada una de las acciones.
```C
void funcionMotorRetroceso(){
  step4();
  delay(delayMotor);
  step2();
  delay(delayMotor);
  step3();
  delay(delayMotor);
  step1();
  delay(delayMotor);
  step4();
  delay(delayMotor);
  step2();
  delay(delayMotor);
  step3();
  delay(delayMotor);
  step1();
  delay(delayMotor);
}
```

### Servir helado
En este método se analiza cada una de las variables para controlar el recorrido del helado por la cinta transportadora. Se analiza el codigo y se le asigna la posicion hasta la cual debera avanzar cuando se encuentre recorriendo la cinta, si el tamaño es de 3 se asignan hasta 3 posiciones.
Al finalizar este metodo se ejecuta el método recorrerPos() para comenzar a mover la cinta.

```C
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

```

### Mover motor
Para mover el motor stepper sin controlador se ejecutarn cada uno de los pasos necesarios para que sea capaz de dar una vuelta, también ejecuta un delay entre cada paso para que no sucedan errores.

```C
void funcionMotor()
{
  step1();
  delay(delayMotor);
  step3();
  delay(delayMotor);
  step2();
  delay(delayMotor);
  step4();
  delay(delayMotor);
  step1();
  delay(delayMotor);
  step3();
  delay(delayMotor);
  step2();
  delay(delayMotor);
  step4();
  delay(delayMotor);
}
```

### Pasos para el motor sin driver
En cada uno de los pasos que el motor da se activa un pin y los demas pines del motor se mantienen apagados, en este caso se utilizaron 4 pasos para poder mover cada uno de los pines y asi girar el motor.

```C
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
```

### Entregar
El método entregar utiliza la función millis() para poder ejecutar dos instrucciones a la vez, en la primera se activa el sensor ultrasonico para medir la distancia que tiene del vaso y la segunda parte se utiliza para mover los motores de la cinta transportadora mientras el helado no se acerque lo suficiente.
Cuando la distancia sea menor a 200 centimetros se activa el buzzer para alertar al usuario de que el helado llegó al final de la cinta y no se detiene hasta que el sensor detecte que se ha retirado el helado aumentando la distancia a mayor a 200.
Al detectar que se retiró el helado se detiene el buzzer y se muestra un mensaje en la pantalla lcd indicando que fue concluido el servicio.

```C
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
```

### Obtener distancia
Para obtener la distancia a la que se encuentra el helado se utiliza el sensor ultrasonico.
Primero se activa el triguer que lanza un pulso que luego es leido por el pin echo, con la funcion pulseIn se detecta lo que tardo la onda en chocar contra un objeto y regresar al sensor. Para finalizar la duración se divide dentro de 58.4 y se obtiene la distancia en cm.

```C
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
```