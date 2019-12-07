#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //sleep y read creo
#include <termios.h>
#include <pthread.h>  //hilos
#include <iostream>

#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>

using namespace std;

int sv,endgame = 0,puntos_p1 = 0,puntos_p2 = 0;

void gamestartmsg(){
    cout<<"Game start in: "<<endl;
    for (int i = 3; i > 0; i--){
        usleep(1000000);
        cout<<i<<" "<<endl;
    }
}

char getch2(void)
{
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if(tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if(tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if(read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if(tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
 }

void gotoxy(int x, int y){
    printf("%c[%d;%df",0x1B,y,x);
}

void dibujar_tablero(){
    std::system("clear");
    gotoxy(0,0);
    printf("################################################################################");
    for (int i = 1; i < 24; i++)
    {
        gotoxy(0,i);
        printf("#");
        gotoxy(80,i);
        printf("#");
    }
    gotoxy(0,24);
    printf("################################################################################");
    
}

class jugador{
    int x, y;
    public:
        jugador(int _x,int _y);
        ~jugador();
        void dibujar() const;
        void borrar() const;
        void moverse(int _y);
        int RX() {return x;}
        int RY() {return y;}
        void setY(int _y){borrar();y = _y;dibujar();}
};

jugador::jugador(int _x,int _y){
    x = _x; y = _y;
}

jugador::~jugador(){}

void jugador::dibujar() const{
    gotoxy(x,y); printf("#");
    gotoxy(x,y+1); printf("#");
    gotoxy(x,y+2); printf("#");
    gotoxy(x,y+3); printf("#");
    gotoxy(x,y+4); printf("#");
    gotoxy(x,y+5); printf("#");
    printf("\e[?25l");
}

void jugador::borrar() const{
    gotoxy(x,y); printf(" ");
    gotoxy(x,y+1); printf(" ");
    gotoxy(x,y+2); printf(" ");
    gotoxy(x,y+3); printf(" ");
    gotoxy(x,y+4); printf(" ");
    gotoxy(x,y+5); printf(" ");
    printf("\e[?25l");
}

void jugador::moverse(int _y){
    borrar();
    y += _y;
    if (y<2)
        y = 2;
    else if (y>18)
        y = 18;
    dibujar();
}

class pelota{
    int x, y, dx, dy;
    public:
        pelota(int _x,int _y, int _dx, int _dy);
        ~pelota();
        void setpos(int _x, int _y) { x = _x; y = _y;}
        void dibujar() const;
        void borrar() const;
        void moverse(jugador a, jugador b);
};

pelota::pelota(int _x,int _y, int _dx, int _dy){
    x = _x; y = _y; dx = _dx; dy = _dy;
}

pelota::~pelota(){}

void pelota::dibujar() const{
    gotoxy(x,y); printf("*"); printf("\e[?25l");
}

void pelota::borrar() const{
    gotoxy(x,y); printf(" "); printf("\e[?25l");
}

void pelota::moverse(jugador a, jugador b){
    borrar();
    x += dx;
    y += dy;
    dibujar();
    if(x + dx == 1 || x + dx == 79) {
        if(x+dx==1)puntos_p1++;
        else puntos_p2++;
        borrar();
        dx = -dx;
        setpos(39,11);
        endgame++;
        }
    if(y + dy == 1 || y + dy == 24) dy = -dy;
    if(x + dx == a.RX() && y >= a.RY() && y <= a.RY() + 1 ) {dx = 1;dy = -1;}
    if(x + dx == a.RX() && y >= a.RY()+2 && y <= a.RY() + 3 ) {dx = 1; dy = 0;}
    if(x + dx == a.RX() && y >= a.RY()+4 && y <= a.RY() + 5 ) {dx = 1;dy = 1;}
    if(x + dx == b.RX() && y >= b.RY() && y <= b.RY() + 1 ) {dx = -1;dy = -1;}
    if(x + dx == b.RX() && y >= b.RY()+2 && y <= b.RY() + 3 ) {dx = -1; dy = 0;}
    if(x + dx == b.RX() && y >= b.RY()+4 && y <= b.RY() + 5 ) {dx = -1;dy = 1;}
}

jugador player_1(6,9);
jugador player_2(74,9);

void *movimiento_jugador1(void *data){
    while (endgame<3){
        char c = getch2();
            if (c == 'w' || c == 'W'){
                player_1.moverse(-1);
            }
            else if(c == 's' || c == 'S'){
                player_1.moverse(1);
            }
        fflush(NULL);
        string s = to_string(player_1.RY());
        const char* coordenadas = s.c_str();

        char* mensaje = (char *) calloc(strlen(coordenadas) + 1, sizeof (char));
        sprintf(mensaje, "%s", coordenadas);
        int largo_mensaje = strlen(mensaje);
        send(sv, mensaje, largo_mensaje, 0);
    }
    std::system("clear");
    gotoxy(0,0);
    printf("Presione alguna tecla");
}

void *movimiento_jugador2(void *data){
    while (endgame<3){
        char c = getch2();
            if (c == 'w' || c == 'W'){
                player_2.moverse(-1);
            }
            else if(c == 's' || c == 'S'){
                player_2.moverse(1);
            }
        fflush(NULL);
        string s = to_string(player_2.RY());
        const char* coordenadas = s.c_str();

        char* mensaje = (char *) calloc(strlen(coordenadas) + 1, sizeof (char));
        sprintf(mensaje, "%s", coordenadas);
        int largo_mensaje = strlen(mensaje);
        send(sv, mensaje, largo_mensaje, 0);
    }
    std::system("clear");
    gotoxy(0,0);
    printf("Presione alguna tecla");
}

void *recibir_p1(void *data){
    while (endgame<3){
        int aux;
        int n = 0;
        char buffer[256];
        char* pbuffer = buffer;
        if ((n = recv(sv, pbuffer, 256, 0)) > 0) {
            buffer[256] = '\0';
            buffer[n] = '\0';
            aux = stoi(buffer);
            if(aux == 90) aux = 9;
            player_1.setY(aux);
            usleep(50000);
        }
    }
    std::system("clear");
    gotoxy(0,0);
    printf("Presione alguna tecla");
}

void *recibir_p2(void *data){
    while (endgame<3){
        int aux;
        int n = 0;
        char buffer[256];
        char* pbuffer = buffer;
        if ((n = recv(sv, pbuffer, 256, 0)) > 0) {
            buffer[256] = '\0';
            buffer[n] = '\0';
            aux = stoi(buffer);
            if(aux == 90) aux = 9;
            player_2.setY(aux);
            usleep(50000);
        }
    }
    std::system("clear");
    gotoxy(0,0);
    printf("Presione alguna tecla");
}

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
    sv = sock;

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

    pthread_t proceso_jugador;
    pthread_t proceso_jugador_opuesto;

    int n = 0;
    char buffer[256];
    char* pbuffer = buffer;
    if ((n = recv(sock, pbuffer, 256, 0)) > 0) {
        buffer[256] = '\0';
    }
    if (buffer[0]=='1'){
        for (int  i = 0; i < 2; i++)
        {
            if ((n = recv(sock, pbuffer, 256, 0)) > 0) {
                buffer[256] = '\0';
                fprintf(stdout, "Se ha recibido: '%s'\n", buffer);
            }
        }
        gamestartmsg();
        usleep(1000000);
        pthread_create(&proceso_jugador,NULL, &movimiento_jugador1 ,NULL);
        pthread_create(&proceso_jugador_opuesto,NULL, &recibir_p2 ,NULL);
    }
    else{
        if(buffer[0]=='2'){
            if ((n = recv(sock, pbuffer, 256, 0)) > 0) {
                buffer[256] = '\0';
                fprintf(stdout, "Se ha recibido: '%s'\n", buffer);
            }
        }
        gamestartmsg();
        usleep(1000000);
        pthread_create(&proceso_jugador,NULL, &movimiento_jugador2 ,NULL);
        pthread_create(&proceso_jugador_opuesto,NULL, &recibir_p1 ,NULL);
    }
    
    pelota ball(39,11,1,1);
    dibujar_tablero();
    player_1.dibujar();
    player_2.dibujar();
    printf("\e[?25l");
    while (endgame<3)
    {    
        ball.moverse(player_1,player_2); 
        usleep(85000);
        fflush(NULL);
    }
    pthread_join(proceso_jugador,NULL);
    pthread_join(proceso_jugador_opuesto,NULL);
    usleep(100000);
    std::system("clear");
    gotoxy(0,0);
    printf("################################################################################");
    printf("Puntos jugador 1: %d   puntos jugador 2: %d \n",puntos_p1,puntos_p2);
    return 0;
}