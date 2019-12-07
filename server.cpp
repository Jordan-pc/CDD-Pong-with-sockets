#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include <iostream>
#include <vector>
#include <pthread.h>  //hilos
#include <string>

#define MAX_ENQUEUED 2
#define PORT_STR "7777"

std::vector <int> clientes;

int p1y = 9;
int p2y = 9;

void esparar_zombies(int socket) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void *movimientos_p1(void *data){
    int n = 0;
    char buffer[256];
    char* pbuffer = buffer;
    while (true){    
        if ((n = recv(clientes[0], pbuffer, 256, 0)) > 0) {
            buffer[256] = '\0';
            fprintf(stdout, "Se ha recibido del jugador 1: '%s'\n", buffer);
        }
        p1y = std::stoi(buffer);
        buffer[n] = '\0';
        usleep(5000);
    }
}

void *movimientos_p2(void *data){
    int n = 0;
    char buffer[256];
    char* pbuffer = buffer;
    while (true){    
        if ((n = recv(clientes[1], pbuffer, 256, 0)) > 0) {
            buffer[256] = '\0';
            fprintf(stdout, "Se ha recibido del jugador 2: '%s'\n", buffer);
        }
        p2y = std::stoi(buffer);
        buffer[n] = '\0';
        usleep(5000);
    }
}

void *send_p1(void *data){
    while (true){
        std::string s = std::to_string(p2y);
        const char* coordenadas = s.c_str(); 
        write(clientes[0], coordenadas, 2);
        fprintf(stdout, "Se ha enviado al jugador 1: '%s'\n", coordenadas);
        usleep(50000);
    }
}

int main(int argc, char** argv) {
    /* Se definen estructuras para manejar las conexiones. */
    /* La estrucutra que escuchará las peticiones */
    struct addrinfo escuchador;
    /* La estructura que responderá las peticiones */
    struct addrinfo *respondedor;
    /*  Estructura para el manejo de señales. */
    struct sigaction sa;
    int sock;

    /* Limipamos la estructura a usar con 0 */
    memset(&escuchador, 0, sizeof (struct addrinfo));
    /* Familia de protocolos a los que responderá el Socket */
    escuchador.ai_family = AF_UNSPEC;

    /* Tipo de Socket */
    escuchador.ai_socktype = SOCK_STREAM;
    /*  Indicadores que modifican el comportamiento del socket */
    escuchador.ai_flags = AI_PASSIVE;
    if (getaddrinfo(NULL, PORT_STR, &escuchador, &respondedor) != 0) {
        /* 
         * Muestra un mensaje correspondiente a un error del sistema, en 
         * particular un error al levantar el puerto en la máquina local
         */
        perror("getaddrinfo");
        /* El programa termina con error */
        exit(EXIT_FAILURE);
    }

    /* Realiza la conexión del socket */
    if ((sock = socket(respondedor->ai_family, respondedor->ai_socktype, respondedor->ai_protocol)) == -1) {
        /* 
         * Muestra un mensaje correspondiente a un error del sistema, en 
         * particular un error establecer la conexión del socket
         */
        perror("socket");
        /* El programa termina con error */
        exit(EXIT_FAILURE);
    }

    int option = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    /* Se levanta el manejador de conexiones muertas (zombies). */
    sa.sa_handler = esparar_zombies;

    /* Se procesa la señal que manejará la aplicación. */
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        /* 
         * Muestra un mensaje correspondiente a un error del sistema, en 
         * particular un error por una señal no procesada por el sistema operativo.
         */
        perror("sigaction");
        /* El programa termina con error */
        exit(EXIT_FAILURE);
    }

    /* Se asocia el socket con su puerto. */
    if (bind(sock, respondedor->ai_addr, respondedor->ai_addrlen) != 0) {
        /* 
         * Muestra un mensaje correspondiente a un error del sistema, en 
         * particular un error por la incapacidad de asociar el puerto.
         */
        perror("bind");
        /* El programa termina con error */
        exit(EXIT_FAILURE);
    }
    /* Se liberan los recursos asociados a la respuesta. */
    freeaddrinfo(respondedor);


    /* Se levanta un estado en el cual establecemos la cantidad de conexiones encoladas. */
    if (listen(sock, MAX_ENQUEUED) != 0) {
        /* 
         * Muestra un mensaje correspondiente a un error del sistema, en 
         * particular un error por llegar al límite de clientes encolados.
         */
        perror("listen");
        /* El programa termina con error */
        exit(EXIT_FAILURE);
    }

    /* Se escuchan solamente dos conexiones, la primera se le asigna al jugador 1
    la segunda se le asigna al jugador 2. */
    struct sockaddr addr;
    socklen_t addr_size = sizeof (addr);
    int socket_cliente;
    socket_cliente = accept(sock, &addr, &addr_size);
    if (socket_cliente==-1){
        perror("accept");
    }
    std::cout<<"Se ha conectado el jugador 1"<<std::endl;
    char p1[2] = "1";
    write(socket_cliente, p1, 2);
    usleep(10000);
    char msg[23] = "Esperando al jugador 2";
    write(socket_cliente, msg, 23);
    clientes.push_back(socket_cliente);
    socket_cliente = accept(sock, &addr, &addr_size);
    if (socket_cliente==-1){
        perror("accept");
    }
    std::cout<<"Se ha conectado el jugador 2"<<std::endl;
    char p2[2] = "2";
    write(socket_cliente, p2, 2);
    usleep(10000);
    char msg2[29] = "Se ha conectado el jugador 2";
    clientes.push_back(socket_cliente); // vector que contiene los descriptores de los clientes
    for (int i = 0; i < clientes.size(); i++){
        write(clientes[i], msg2, 29);
    }
    usleep(3500000);
    pthread_t escuchar_p1,escuchar_p2,enviar_p1;
    pthread_create(&escuchar_p1,NULL, &movimientos_p1 ,NULL);
    pthread_create(&escuchar_p2,NULL, &movimientos_p2 ,NULL);
    pthread_create(&enviar_p1,NULL, &send_p1 ,NULL);
    while (true){
        for (int i = 0; i < clientes.size(); i++){
            std::string s = std::to_string(p1y);
            const char* coordenadas = s.c_str(); 
            write(clientes[1], coordenadas, 2);
            fprintf(stdout, "Se ha enviado al jugador 2: '%s'\n", coordenadas);
            usleep(50000);
        }
    }
    for (int i = 0; i < clientes.size(); i++){
        close(clientes[i]);
    }
    pthread_join(escuchar_p1,NULL);
    pthread_join(escuchar_p2,NULL);
    pthread_join(enviar_p1,NULL);
    return EXIT_SUCCESS;
}