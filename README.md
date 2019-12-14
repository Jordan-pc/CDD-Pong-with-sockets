# CDD-Pong-with-sockets
Realización del juego pong, utilizando sockets, para la asignatura comunicación de datos, en la Universidad Tecnologica Metropolitana (Utem) solicitado por el profesor Sebastián Salazar Molina.

## Pre-requisitos :clipboard:
Requirement  | Version
------------- | -------------
[Ubuntu](https://ubuntu.com/download/desktop)    |  18.04 LTS
[C++](https://isocpp.org/)       |  std = 11 
[Make]       |  (opcional) 

## Como usarlo
Una vez cumplidos los pre-requisitos y despues de descargarlo o clonarlo con git (`git clone https://github.com/Jordan-pc/CDD-Pong-with-sockets`)

Se puede compilar con make, dirigiendo la terminal al lugar con todos los archivos y teniendo make instalado bastara con escribir `make`.

En caso de querer compilar por separado se puede realizar `make game` y despues `make server`.

O tambien se puede realizar de la siguiente forma: `g++ game.cpp -o game -pthread` y despues `g++ server.cpp -o server -pthread`

Para probarlo basta con ejecutar en orden el servidor primero y despues el juego  por ende seria de la siguiente forma, en una consola se ejecuta el servidor con `./server`.

En otra consola habra que ejecutar el juego de la siguiente forma `./game servidor puerto` en este caso el puerto default es 7777, el servidor es la direccion ipv4, la cual se puede ver en la configuracion de internet en ubuntu(en nuestro caso 10.0.2.15) por lo cual para ejecutarlo quedo de la siguiente forma: `./game 10.0.2.15 7777`

Una vez realizado esto se enviara un mensaje para avisar que aun falta el jugador dos, este puede entrar desde otra computadora conectada a la misma red o abriendo otra consola y realizando los mismos pasos mencionados anteriormente.

## Como jugar
Para moverse es necesario presionar la tecla `w` para subir, y la tecla `s` para bajar

## Contributors
Cabe destacar que este proyecto se realizaba en grupo de tres personas, mis compañeros fueron Renato Luco[[LucoQlo](https://github.com/LucoQlo)]. y Sergio Licanqueo [[Sergio-36](https://github.com/sergio-36)]