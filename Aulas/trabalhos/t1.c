#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct timespec crono;

typedef struct{
    bool terminou, terminouPartida, terminouOnda;
    int pontos, tiros, arma, escudos;
    int inimigosInativos;
  
} Sistema;

void crono_inicia(crono *c)
{
    clock_gettime(CLOCK_MONOTONIC, c);
}

double crono_parcial(crono *c)
{
    crono agora;
    clock_gettime(CLOCK_MONOTONIC, &agora);
    double segundos = agora.tv_sec - c->tv_sec;
    double nanosegundos = agora.tv_nsec - c->tv_nsec;
    return segundos + 1e-9 * nanosegundos;
}

void configuraTerminal()
{
    if (system("stty raw opost -echo min 0 time 1") != 0) {
        perror("erro na execução de system(\"stty\")");
        fprintf(stderr, "você tem o programa stty instalado?\n");
        exit(1);
    };
    if (setvbuf(stdin, NULL, _IONBF, 0) != 0) {
        perror("erro na execução de setvbuf()");
        exit(1);
    }
}

void normalizaTerminal()
{
    system("stty sane");
}

char lechar()
{
    fflush(stdout);
    char c;
    if (fread(&c, 1, 1, stdin) == 1) return c;
    return 0;
}

void jogaOnda(Sistema *s)
{
    while (!s->terminouOnda) {
        //processaTeclado(s);
        //processaTempo(s);
        //apresenta(s);
    }
}

void jogaPartida(Sistema *s)
{
    while (!s->terminouPartida) {
        jogaOnda(s);
    }
}

int main()
{
    configuraTerminal();
    Sistema sistema;
    //inicializa_tela();
    //inicializaSistema(&sistema);
    while (!sistema.terminou) {
        jogaPartida(&sistema);
    }
    //desinicializa_tela();
    normalizaTerminal();
}