#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>

/**
 * 
 * @param argc Cantidad de Argumentos
 * @param argv Matriz asociativa con los argumentos.
 * @return El estado de salida de la ejecución del programa.
 */
int main(int argc, char** argv) {

    if (argc < 3) {
        fprintf(stderr, "\nArgumentos insufientes");
        fprintf(stderr, "\n%s servidor puerto\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* nombre_servidor = (char *) calloc(strlen(argv[1]) + 1, sizeof (char));
    sprintf(nombre_servidor, "%s", argv[1]);

    int puerto_servidor = atoi(argv[2]);
    fprintf(stdout, "\nServidor: %s:%d\n", nombre_servidor, puerto_servidor);

    struct sockaddr_in servidor;
    struct hostent *host;
    if ((host = gethostbyname(nombre_servidor)) == NULL) {
        /* Se muestra un mensaje de error. */
        fprintf(stderr, "Nombre del servidor inválido\n");
        /* El programa termina con error. */
        return EXIT_FAILURE;
    }

    memset(&servidor, 0, sizeof (servidor));
    servidor.sin_family = AF_INET;
    
    /*
     * Transformo desde el host (que resuelve nombre)
     * a la estructura que lo usa
     */
    memcpy(&servidor.sin_addr, host->h_addr_list[0], host->h_length);

    /* La función htons transforma el puerto a formato socket */
    servidor.sin_port = htons(puerto_servidor);

    /* Se abre el socket */
    int sock;
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        /* Se muestra un mensaje de error. */
        fprintf(stderr, "No fue posible crear el socket\n");
        /* El programa termina con error. */
        return EXIT_FAILURE;
    }

    /*
     * Como el socket es TCP, la conexión debe establecerse primero,
     * antes de compartir datos.
     */
    if (connect(sock, (struct sockaddr*) &servidor, sizeof (servidor)) < 0) {
        /* Se muestra un mensaje de error. */
        fprintf(stderr, "No se pudo establecer la comunicacion\n");
        /* El programa termina con error */
        return EXIT_FAILURE;
    }

    /*
     * Preparación de datos a enviar
     * Datos a enviar
     */
    //testing de aca para abajo

    while (true){

        // const char* coordenadas = "1#2#3#4#5#6";


        // char* mensaje = (char *) calloc(strlen(coordenadas) + 1, sizeof (char));
        // sprintf(mensaje, "%s", coordenadas);
        // int largo_mensaje = strlen(mensaje);
        // send(sock, mensaje, largo_mensaje, 0);


        /* Datos para recibir. */
        int n = 0;
        //int largo = 0;
        //int largo_recibido = (largo_mensaje + 1);
        char buffer[256];
        char* pbuffer = buffer;

        /* Estaremos esperando datos. */
        if ((n = recv(sock, pbuffer, 256, 0)) > 0) {
            //pbuffer += n;
            //largo_recibido -= n;
            //largo += n;

            /* Fin de linea */
            buffer[256] = '\0';
            fprintf(stdout, "Se ha recibido: '%s'\n", buffer);
        }
        // usleep(5000000);
    }

    /* Cerramos el socket */
    close(sock);

    /* Terminamos el programa */
    return EXIT_SUCCESS;
}