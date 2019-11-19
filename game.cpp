// int getch(void){
//     struct termios oldattr, newattr;
//     int ch;
//     tcgetattr( STDIN_FILENO, &oldattr );
//     newattr = oldattr;
//     newattr.c_lflag &= ~( ICANON | ECHO );
//     tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
//     ch = getchar();
//     tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
//     return ch;
// }

// int getche(void)
// {
//     struct termios oldattr, newattr;
//     int ch;
//     tcgetattr( STDIN_FILENO, &oldattr );
//     newattr = oldattr;
//     newattr.c_lflag &= ~( ICANON );
//     tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
//     ch = getchar();
//     tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
//     return ch;
// }

// int getchp(void) {
//   struct termios term_settings,term_settings_saved;
//   char c;
//   int x;
//   if (tcgetattr(STDIN_FILENO,&term_settings))
//     return -2;
//   term_settings_saved=term_settings;
//   term_settings.c_lflag &= ~ICANON ;
//   term_settings.c_lflag &= ~ECHO ;
//   term_settings.c_cc[VMIN]=0;
//   term_settings.c_cc[VTIME]=0;
//   if (tcsetattr (STDIN_FILENO, TCSANOW, &term_settings) < 0)
//     return -2;
//   x = getchar();
//   tcsetattr (STDIN_FILENO, TCSANOW, &term_settings_saved);
//   return x;
// }

// int kbhit(void){
//     struct timeval tv;
//     fd_set rdfs;
//     tv.tv_sec = 0;
//     tv.tv_usec = 0;
//     FD_ZERO(&rdfs);
//     FD_SET(STDIN_FILENO, &rdfs);
//     /*return*/select(STDIN_FILENO+1,&rdfs,NULL,NULL,&tv);
//     return FD_ISSET(STDIN_FILENO,&rdfs);
// }

// int kbhit2(void)
// {
//   struct termios oldt, newt;
//   int ch = 0;
//   int oldf;

//   tcgetattr(STDIN_FILENO, &oldt);
//   newt = oldt;
//   newt.c_lflag &= (~ICANON & ~ECHO);
//   tcsetattr(STDIN_FILENO, TCSANOW, &newt);
//   oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
//   fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

//   ch = getch();


//   tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
//   fcntl(STDIN_FILENO, F_SETFL, oldf);

//   if(ch != EOF)
//   {
//     ungetc(ch, stdin);
//     return 1;
//   }

//   return 0;
// }

// void enable_raw_mode()
// {
//     termios term;
//     tcgetattr(0, &term);
//     term.c_lflag &= ~(ICANON | ECHO); // Disable echo as well
//     tcsetattr(0, TCSANOW, &term);
// }

// void disable_raw_mode()
// {
//     termios term;
//     tcgetattr(0, &term);
//     term.c_lflag |= ICANON | ECHO;
//     tcsetattr(0, TCSANOW, &term);
// }

// bool _kbhit()
// {
//     int byteswaiting;
//     ioctl(0, FIONREAD, &byteswaiting);
//     return byteswaiting > 0;
// }


// class keyboard
// {
// public:

//     keyboard();
//     ~keyboard();
//     int kbhit();
//     int getch();

// private:

//     struct termios initial_settings, new_settings;
//     int peek_character;

// };

// keyboard::keyboard()
// {
//     tcgetattr(0,&initial_settings);
//     new_settings = initial_settings;
//     new_settings.c_lflag &= ~ICANON;
//     new_settings.c_lflag &= ~ECHO;
//     new_settings.c_lflag &= ~ISIG;
//     new_settings.c_cc[VMIN] = 1;
//     new_settings.c_cc[VTIME] = 0;
//     tcsetattr(0, TCSANOW, &new_settings);
//     peek_character=-1;
// }

// keyboard::~keyboard()
// {
//     tcsetattr(0, TCSANOW, &initial_settings);
//     printf("keyboard restaured");
// }

// int keyboard::kbhit()
// {
// unsigned char ch;
// int nread;

//     if (peek_character != -1) return 1;
//     new_settings.c_cc[VMIN]=0;
//     tcsetattr(0, TCSANOW, &new_settings);
//     nread = read(0,&ch,1);
//     new_settings.c_cc[VMIN]=1;
//     tcsetattr(0, TCSANOW, &new_settings);

//     if (nread == 1)
//     {
//         peek_character = ch;
//         return 1;
//     }
//     return 0;
// }

// int keyboard::getch()
// {
// char ch;

//     if (peek_character != 1)
//     {
//         ch = peek_character;
//         peek_character = -1;
//     } else read(0,&ch,1);

//     return ch;
// }

// static struct termios initial_settings, new_settings;
// static int peek_character = -1;


// void init_keyboard() {
//   tcgetattr(0, &initial_settings);

//   new_settings = initial_settings;
//   new_settings.c_lflag &= ~ICANON;
//   new_settings.c_lflag &= ~ECHO;
//   new_settings.c_lflag &= ~ISIG;
//   new_settings.c_cc[VMIN] = 1;
//   new_settings.c_cc[VTIME] = 0;
//   tcsetattr(0, TCSANOW, &new_settings);
// }

// void close_keyboard() {
//   tcsetattr(0, TCSANOW, &initial_settings);
// }

// int kbhit3() {
//   char ch;
//   int nread;

//   if (peek_character != -1) {
//     return 1;
//   }

//   new_settings.c_cc[VMIN] = 0;
//   tcsetattr(0, TCSANOW, &new_settings);
//   nread = read(0, &ch, 1);
//   new_settings.c_cc[VMIN] = 1;
//   tcsetattr(0, TCSANOW, &new_settings);

//   if (nread == 1) {
//     peek_character = ch;
//     return 1;
//   }
//   return 0;
// }

// int readch() {
//   char ch;

//   if (peek_character != 1) {
//     ch = peek_character;
//     peek_character = -1;
//     return ch;
//   }

//   read(0, &ch, 1);
//   return ch;
// }


#include <stdio.h>
#include <stdio_ext.h> //fpuge

#include <stdlib.h>
#include <string.h>  //nose para que esta, pero la dejo por si acaso por ahora

#include <unistd.h>  //sleep y read creo
#include <termios.h> //getch2

#include <fcntl.h>     //de aca para abajo cosas del copy paste de arriba
#include <sys/time.h>

#include <sys/select.h>
#include <stropts.h>
#include <sys/ioctl.h>    //fin funciones copy paste

#include <pthread.h>  //hilos

#include <iostream> //cout

using namespace std; //cout

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
    if(x + dx == 1 || x + dx == 79) {borrar();dx = -dx; setpos(39,11);}
    if(y + dy == 1 || y + dy == 24) dy = -dy;
    if(x + dx == a.RX() && y >= a.RY() && y <= a.RY() + 5 ) dx = -dx;
    if(x + dx == b.RX() && y >= b.RY() && y <= b.RY() + 5 ) dx = -dx;
}

jugador player_1(6,9);
jugador player_2(74,9);

void *movimiento_pelota(void *data){
    player_1.dibujar();
    //pelota ball2(39,11,1,1);
    //ball2.dibujar();
    while (true){
        char c = getch2();
            if (c == 'w' || c == 'W'){
                player_1.moverse(-1);
            }
            else if(c == 's' || c == 'S'){
                player_1.moverse(1);
            }
        //ball2.moverse();
        // usleep(500);
        fflush(NULL);
        // setbuf(stdin,NULL);
        // __fpurge;
    }
}


int main(int argc, char** argv) {
    pthread_t proceso_pelota;
    pelota ball(39,11,1,1);
    dibujar_tablero();
    //player_1.dibujar();
    player_2.dibujar();
    printf("\e[?25l");
    //keyboard teclado;
    //init_keyboard();
    pthread_create(&proceso_pelota,NULL, &movimiento_pelota ,NULL);
    while (true)
    {    
        //init_keyboard();
        //if(kbhit3()){
            // char c = getch2();
            // if (c == 'w' || c == 'W'){
            //     player_1.moverse(-1);
            // }
            // else if(c == 's' || c == 'S'){
            //     player_1.moverse(1);
            // }
        //}
        ball.moverse(player_1,player_2); 
        usleep(65000);
        fflush(NULL);
        // setbuf(stdin,NULL);
        // __fpurge;
        //close_keyboard();
    }
    //close_keyboard();
    pthread_join(proceso_pelota,NULL);
    return 0;
}