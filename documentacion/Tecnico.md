# [Manual Ténico]()
______

## Como Instalar Arduino

- For Windows
Debe de dirigirse a la [Página de arduino](https://www.arduino.cc/en/software) con el siguiente enlace.
___
## Metodos a Utilizar
____

### Arduino
Libería a Utilizar para EEPROM, encargada de almacenar tanto la cantidad de helados, como la cantidad de dinero recaudado, además de mostrar la temperatura ambiente; otra librería a implentar fue la de LiquidCrystal que sirve para la pantalla led; la librería keypad que sirve para manejar el teclado matricial.
```c++
/* Memoria EEPROM*/
#include <EEPROM.h>
/* Pantalla Led */
#include<LedControl.h>
#include <LiquidCrystal_I2C.h>
/* Teclado Matricial */
#include <Keypad.h>
```

Se define un pin para poder conectar el componente bluethoot; define un pin para poder conectar el sensor de temperatura.
```c++
/* bluetooth */
#define bluethoot 8
/* sensor de temperatura */
#define pinA1 1
```


Variables a utilizar para millis():
```c++
unsigned long time_matrix = 0;
unsigned long contador_1  = 0; 
```

Estructura para el teclado matricial
```c++
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
```




Utilizamos una variable float, para poder obtener el valor en °C.
```c++
/* Variables */
float temperature_C;
```

Para calcular la temperatura se uso está ecuación:
```c++
/* Temperatura */
temperature_C = analogRead(pinA1);                      // Leemos el sensor por medio de analogRead, se encuentra en un rango de 0 a 1023.
temperature_C = (5.0 * temperature_C * 100.0)/1024.0;   // Calculamos la temperatura con la fórmula.
MyStructValue.temperature = temperature_C;
```

Creación de Struct y Asignación.
```c++
/* Create Struct */
int eeAddress = 0;
struct MyStruct{
  int icecream0;
  int icecream1;
  int icecream2;
  int icecream3;
  int icecream4;
  float money;
  int temperature;
};

/* Assignment */
MyStruct MyStructValue = {10,10,10,10,10,0.0,20};
```


Iniciar Terminal y el componente HC06 - Bluetooth; además de agregar la configuración Inicial para la pantalla led.
```c++
void setup() {
  // put your setup code here, to run once:
  /** TERMINAL **/
  Serial.begin(9600);   // Terminal
  Serial1.begin(9600);  // Bluethooth

  /* EEPROM */
  EEPROM.put(eeAddress, MyStructValue);

  /* PANTALLA LED */
  lcd.begin(16,2);
  lcd.clear();
}

```

Comuniación entre Arduino y la APP, es la encargada de recibir por medio de un avilable, luego por medio de un read(); se puede obtener el dato recibido desde la app, que luego por medio de un Serial1.print(); se puede enviar datos a la app, en este caso se envio las cantidades de los helados, el dinero, y temeratura.

```c++
void loop() {
  // put your main code here, to run repeatedly:
  

  if (Serial1.available() > 0) {
      char readed = Serial1.read();

      if (readed == 'C'){ // conectar Bluethooth
       Serial.println("Bluethoot Conectado!"); 
      }else if (readed == 'U'){ // UPDATE
        Serial.println("Actualizando Datos");
        EEPROM.get(eeAddress, MyStructValue);     // getStruct
        boolean flag = false;
        if (MyStructValue.icecream0 == 0){ MyStructValue.icecream0 = 10; flag = true; }
        if (MyStructValue.icecream1 == 0){ MyStructValue.icecream1 = 10; flag = true; }
        if (MyStructValue.icecream2 == 0){ MyStructValue.icecream2 = 10; flag = true; }
        if (MyStructValue.icecream3 == 0){ MyStructValue.icecream3 = 10; flag = true; }
        if (MyStructValue.icecream4 == 0){ MyStructValue.icecream4 = 10; flag = true; }
        if (flag) { EEPROM.put(eeAddress, MyStructValue); flag = false; }
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
```

Metodo que es el encargado de obtener los datos del teclado matricial.
```c++
void metodoTeclado(){
   if (millis() > time_matrix + 10){
    time_matrix = millis();

    char key = keypad.getKey();
    if (key) {
      Serial.println(key);
    }
  }
}
```


Metodo encargado de mostrar el texto en la pantalla Led.
```c++
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
```
Metodo encargado de obtener el vaso que se desea seleccionar 
```c++
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
```

Metodo encargado de enviar los datos seleccionados del vaso 
```c++
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
    }
  }
  
}
```

Metodo encargado de resetear todas las banderas para poder escoger un vaso de helado nuevamente 
```c++
void reset(){
      bandera_vaso = false ; 
    
    flag_vaso = 0 ;
    precio_vaso_1 = false ; 
    vaso_limpiar = false;
    
    cerrar_vaso = false ;
}
```
Metodo encargado de obtener el char seleccionado en el teclado 
```c++
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
      metodoTeclado();
    }
  }
}
```
### SABORES
Metodo de choose code , para poder seleccionar el codigo que se esta escribiendo por medio del teclado 
```c++
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
```
Metodo de Switch_code sirve para iniciliazar las concatenaciones 
```c++
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


```

Mostrar Error de codigo de helado 
```c++
void show_message_code_mistake(){
  Serial.println("Codigo de Sabor: " + String(conca_matrix));
  lcd.setCursor(0,0);
  lcd.print("NO EXISTE ESE");
  lcd.setCursor(0,1);
  lcd.print("CODIGO DE SABOR");
}
```

Mostrar mensaje de falta de inventario de helado 
```c++
void show_message_inventary_mistake(){
  Serial.println("NO HAY STOCK");
  lcd.setCursor(0,0);
  lcd.print("NO HAY STOCK");
  lcd.setCursor(0,1);
  lcd.print("DE ESE SABOR");
}

```
### APP

La app fue desarrolada en [MIT App Inventor](https://appinventor.mit.edu/)

Declaracion de variables, para la lista y otros condiciones.

![app0](img/app/define.png)


Se inicio conectado por medio de bloques el bluetooth
Diseño en proteus.

![app1](img/app/Conexion.png)

Luego se por medio de un boton se puede conectar al bluetooth

![app2](img/app/button_connected.png)


Ademásse agrego un boton que permite desconectar al bluetooth.

![app3](img/app/button_disconected.png)

Se creo un boton encargado de actualizar los datos de la APP.

![app4](img/app/button_update.png)

Para poder recibir los datos se hizo la siguiente estructura, que es la encarga de recibir de arduino a app, primero se guarda en un texto, que por consigueinte, es separada por un split, separada por "|" y es guardada en la lista, que es recorrida desde la posición 1 hasta la 7.

![app5](img/app/data.png)

