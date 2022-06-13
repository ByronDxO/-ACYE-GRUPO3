_____

## Diseño
_____
### Proteus
Diseño en proteus.
![mg1](img/1.png)

Terminal
![mg2](img/2.png)

Componente que se utilizo para el Bluetooth
![mg3](img/3.png)

____
### App

Diseño de la APP
![app1](img/app/Dising.png)

Parte para codificar en al APP
![app1](img/app/design.png)
____
### Teclado y Pantalla LCD 

#### Interfaz General de el circuito 
![teclado0](img/img_byron/Imagen2.png)
#### Mensaje inicial al iniciar la aplicacion 
![teclado1](img/img_byron/Imagen3.png)
#### Al pasar 5 segundos se podra seleccionar el vaso seleccionado existiendo la posibilidad de ecoger entre 3 vasos del 1 al 3 si no es ninguno de estos generara un error y lo devolvera al mensaje de inicio 
![teclado2](img/img_byron/Imagen4.png)
#### Al seleccionar el vaso se mostrara por un breve tiempo el valor de este vaso y se procedera a seleccionar los sabores 
![teclado3](img/img_byron/Imagen5.png)
#### los sabores podran ser seleccionados por medio de un codigo de 3 numeros para cada sabor , estos podran ser escogidos dependiendo del vaso que se haya seleccionado con anterioridad permitiendo seleccionar hasta 3 diferentes abores con el vaso 3 
![teclado4](img/img_byron/Imagen6.png)

## Cinta transportadora
Luego de seleccionar el tamaño del helado lo los sabores que desea, la cinta transportadora comienza a moverse hacia el primer sabor de helado, al pasar por cada sabor se enciende la luz led sobre los motores DC para indicar que el helado va pasando por esa posicion.

![T1](img/img_tomas/t1.png)

![T2](img/img_tomas/t2.png)

Al llegar a la posicion del helado que seleccionó el usuarion la cinta se detiene y el motor DC del sabor comienza a girar por 5 segundos para simular que se está sirviendo el helado, la luz led que se encuentra sobre el motor tambien se activa mientras el motor se mueve.

![T3](img/img_tomas/t3.png)

También es posible que la cinta retroceda hacia un sabor por el cual ya pasó, en este caso tambien se prenderá las luces por donde va pasando el helado hasta llegar al que el usuario selecciono para mover el motor DC simulando que el helado se esta sirviendo.

![T1.1](img/img_tomas/t1.png)

![T2.2](img/img_tomas/t2.png)

![T3.3](img/img_tomas/t3.png)

![T2.2](img/img_tomas/t2.png)

![T4](img/img_tomas/t4.png)

Al pasar por las 5 estaciones del helado y llegar al final de la cinta transportadora, el vaso se encuentra con un sensor ultrasonico que detecta la distancia a la que se encuentra, si la distancia es menor a 200 cm se detienen los motores de la cinta y se produce una alarma con el buzzer para indicarle al usuario que el servicio ha finalizado.
Con proteus se simula que la distancia es menor a 200 moviendo el potenciómetro a un valor menor a 15% lo que dará a entender que el vaso llego al final. Para detener la alarma del buzzer se tiene que retirar el vaso de la cinta transportadora, para eso se modifica nuevamente el valor del potenciómetro a un valor mayor al 20%, al hacer esto la alarma dejara de sonar y el sistema volverá a iniciar solicitandole de nuevo al usuario que seleccione el tamaño del helado que desea.

![T5](img/img_tomas/t5.png)

Para apagar el buzzer simulamor que se retira el vaso aumentando el varlor del potenciómetro.

![T6](img/img_tomas/t6.png)

Al hacer esto el programa vuelve a solicitarle al usuario un nuevo tamaño de helado y los sabores correspondientes a dicho tamaño. Volviendo a iniciar todo el sistema de nuevo.

Se puede escoger cualquier combinacion de sabores, desde 3 del mismo sabor o diferentes en diferente orden a las que aparecen en la cinta.