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

#define MAX_ENQUEUED 2
#define PORT_STR "7777"

void esparar_zombies(int socket) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void responder_lo_mismo(int socket_cliente) {
    char buf[256];
    int r;

    while ((r = read(socket_cliente, buf, 256)) > 0) {
        (void) write(socket_cliente, buf, r);
    }
    exit(EXIT_SUCCESS);
}

void escuchar_conexiones(int socket_servidor) {
    for (;;) {
        struct sockaddr addr;
        socklen_t addr_size = sizeof (addr);
        int socket_cliente;

        /* Se bloquea hasta que el servidor tome una conexión. */
        socket_cliente = accept(socket_servidor, &addr, &addr_size);

        /* Si la conexión es exitosa se levanta un proceso que lo atienda */
        if (socket_cliente == -1) {
            perror("accept");
        } else if (fork() == 0) {
            close(socket_servidor);
            responder_lo_mismo(socket_cliente);
        } else {
            close(socket_cliente);
        }
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

    /* Se escuchan conexiones hasta que el programa sea terminado (a la mala). */
    escuchar_conexiones(sock);
    return EXIT_SUCCESS;
}