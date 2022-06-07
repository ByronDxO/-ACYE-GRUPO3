//ESCLAVO 1
#include <Wire.h> 
#include <MatrizLed.h>

MatrizLed pantalla;
MatrizLed pantalla1;
MatrizLed pantalla2;
MatrizLed pantalla3;


unsigned int temp=0;  
int matriz[8][8] = {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}};
char c=0;
int row=0;
int col=0;
int enEjecucion = 0;

//VARIABLE PARA RECIBIR DATOS
String mensaje=""; 
String mensaje2="";
int contador = 0; 

void respuestaEvento()                  //Funcion que se ejecuta al recibir una peticion.      
{
    Wire.write(temp);                 
}



void recibidoEvento(int recepcion)      //Funcion que se ejecuta al recibir datos.
{

    while(Wire.available())
    {
      char c= Wire.read();
      Serial.println(c);
                 
        if(c=='a'){
          enEjecucion=1;
        }
        else if(c=='b'){
          enEjecucion=2;
        }
        else if(c=='c'){
          enEjecucion=3;
        }
        mensaje = mensaje + c;
                                                             
    }
    Serial.println(mensaje);

}

void setup()
{             
    Wire.begin(1);                     
    Wire.onRequest(respuestaEvento); 
    Wire.onReceive(recibidoEvento);  
    Serial.begin(9600);
  // pines controladores para la matriz
  pantalla.begin(11, 12, 10, 1); // dataPin, clkPin, csPin, numero de matrices de 8x8
  pantalla.rotar(true);
  pantalla1.begin(9, 12, 10, 1); // dataPin, clkPin, csPin, numero de matrices de 8x8
  pantalla1.rotar(true);
  pantalla2.begin(8, 12, 10, 1); // dataPin, clkPin, csPin, numero de matrices de 8x8
  pantalla2.rotar(true);
  pantalla3.begin(7, 12, 10, 1); // dataPin, clkPin, csPin, numero de matrices de 8x8
  pantalla3.rotar(true);
  
}
bool terminarMatriz = false;

void loop()
{   
  //desplazarFraseDerecha();
if(mensaje.length()>0){
      MatrizApp();
            
      //si encaso viene a, b, c esto significa qe se activa una estacion de lo contrario enciende los led
      if (enEjecucion == 1) {
        //Serial.println("guarda pantalla1");
        pantalla.escribirCaracter('1', -8);
        ejecutarProcesos(pantalla1);
        //dibujandoMatriz("pantalla1");

      } else if (enEjecucion == 2) {
        //Serial.println("guarda pantalla2");
        pantalla.escribirCaracter('2', -8);
         ejecutarProcesos(pantalla2);
        //dibujandoMatriz("pantalla2");
        
      } else if (enEjecucion == 3) {
        //Serial.println("guarda pantalla3");
        pantalla.escribirCaracter('3', -8);
        //dibujandoMatriz("pantalla3");
         ejecutarProcesos(pantalla3);
      } else {
        //guarda en la matriz los datos que se estan ingresando
        //mensaje = "";
      }
      enEjecucion = 0;
    }
  
}
/******************************************Codigo de la matriz*****************************************/
//matriz de la estacion 1
void Matriz35(MatrizLed pantallas) {
  //Este ciclo sirve apagar las led
  for (int x = 0; x < 8; x++) { //recorrer las filas
    for (int y = 0; y < 3; y++) { // recorrer las columnas
      if (matriz[x][y] == 1) {
        pantallas.setLed(0, x, y, false);
        delay(17);
      }
    }
  }
  //enciende las led
  for (int x = 0; x < 8; x++) { //recorrer las filas
    for (int y = 0; y < 3; y++) { // recorrer las columnas
      if (matriz[x][y] == 1) {
        delay(20);
        pantallas.setLed(0, x, y, true);
      }
    }
  }
}
//matriz de la estacion 2
void Matriz70(MatrizLed pantallas) {
  //Este ciclo sirve apagar las led
  for (int x = 0; x < 8; x++) { //recorrer las filas
    for (int y = 0; y < 6; y++) { // recorrer las columnas
      if (matriz[x][y] == 1) {
        pantallas.setLed(0, x, y, false);
        delay(20);
      }
    }
  }
  //enciende las led
  for (int x = 0; x < 8; x++) { //recorrer las filas
    for (int y = 0; y < 6; y++) { // recorrer las columnas
      if (matriz[x][y] == 1) {
        delay(15);
        pantallas.setLed(0, x, y, true);
      }
    }
  }
}

//matriz de la estacion3
void MatrizCompletas(MatrizLed pantallas) {
  //Este ciclo sirve apagar las led
  for (int x = 0; x < 8; x++) { //recorrer las filas
    for (int y = 0; y < 8; y++) { // recorrer las columnas
      if (matriz[x][y] == 1) {
        pantallas.setLed(0, x, y, false);
        delay(15);
      }
    }
  }
  //enciende las led
  for (int x = 0; x < 8; x++) { //recorrer las filas
    for (int y = 0; y < 8; y++) { // recorrer las columnas
      if (matriz[x][y] == 1) {
        delay(15);
        pantallas.setLed(0, x, y, true);
      }
    }
  }
}

//metodo para encender la matriz inicial, cuando ya se termino el proceso
void MatrizFinal(MatrizLed pantallas) {
  for (int x = 0; x < 8; x++) { //recorrer las filas
    for (int y = 0; y < 8; y++) { // recorrer las columnas
      if (matriz[x][y] == 1) {
        pantalla.setLed(0, x, y, true);
      }
    }
  }
}

//se selecciona la matriz que se estara encendiendo
void dibujandoMatriz(String pantallas) {
  if (pantallas == "pantalla1") {
    //escribe en la matriz incio el numero 1
    pantalla.escribirCaracter('1', -8);
    //ciclo para encender y apagar leds
    for (int i = 0; i < 3; i++) {
      Matriz35(pantalla1);
      i++;
    }
    delay(200);
    for (int i = 0; i < 3; i++) {
      Matriz70(pantalla1);
      i++;
    }
    delay(200);
    for (int i = 0; i < 3; i++) {
      MatrizCompletas(pantalla1);
      i++;
    }
  } else if (pantallas == "pantalla2") {
    pantalla.borrar();
    //escribe en la matriz incio el numero 2
    pantalla.escribirCaracter('2', -8);
    //ciclo para encender y apagar leds
    for (int i = 0; i < 3; i++) {
      Matriz35(pantalla2);
      i++;
    }
    delay(200);
    for (int i = 0; i < 3; i++) {
      Matriz70(pantalla2);
      i++;
    }
    delay(200);
    for (int i = 0; i < 3; i++) {
      MatrizCompletas(pantalla2);
      i++;
    }
  } else if (pantallas == "pantalla3") {
    pantalla.borrar();
    //escribe en la matriz incio el numero 3
    pantalla.escribirCaracter('3', -8);
    //ciclo para encender y apagar leds
        for (int i = 0; i < 3; i++) {
      Matriz35(pantalla3);
      i++;
    }
    delay(200);
    for (int i = 0; i < 3; i++) {
      Matriz70(pantalla3);
      i++;
    }
    delay(200);
    for (int i = 0; i < 3; i++) {
      MatrizCompletas(pantalla3);
      i++;
    }
    
    pantalla.borrar();
    MatrizFinal(pantalla);

  }
}

// metodo donde guarda la matriz de la app en un array
void MatrizApp() {
  int contador = 1;
  for (int i = 0; i < mensaje.length(); i++) {
    if (contador == 1) {
      //se lee la fila
      int convertido = int(mensaje.charAt(i));
      row = convertido;
      contador++;  //2
    } else if (contador = 2) {
      int convertido = int(mensaje.charAt(i));
      col = convertido;
      //se agrega un uno en la posicion matriz[fila][columna] para indicar que se debe encender
      matriz[row][col] = 1;
      Serial.print("led guardado: " );
       Serial.println(row);     
       Serial.println(col);
      contador = 1;
    }

  }
}

//metodo para escribir la frace del grupo
void escribirFraseDesp(const char* frase, int posicion) {
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

    delay(5);

  }
}

void ejecutarProcesos(MatrizLed pantallas){
  for (int x = 0; x < 8; x++) { //recorrer las filas
    for (int y = 0; y < 3; y++) { // recorrer las columnas
      if (matriz[x][y] == 1) {
        pantallas.setLed(0, x, y, true);
        delay(17);
      }
    }
  }
  delay(2000);
   for (int x = 0; x < 8; x++) { //recorrer las filas
    for (int y = 0; y < 6; y++) { // recorrer las columnas
      if (matriz[x][y] == 1) {
        pantallas.setLed(0, x, y, true);
        delay(17);
      }
    }
  }
 delay(2000);
  for (int x = 0; x < 8; x++) { //recorrer las filas
    for (int y = 0; y < 8; y++) { // recorrer las columnas
      if (matriz[x][y] == 1) {
        pantallas.setLed(0, x, y, true);
        delay(17);
      }
    }
  }
}
