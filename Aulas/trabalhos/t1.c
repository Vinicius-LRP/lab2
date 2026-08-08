#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef enum {
    ENTER = 13,
    ESC = 27,
    TAB = 9
} Controle;

typedef struct timespec crono;

typedef struct{
    bool terminou, terminouPartida, terminouOnda;
    int pontos, tiros, armaCorrente, escudos, ondaAtual, ataquesInativos;
    int ataquesAtivos[15];
    double tempoMovimentação;
    crono c;
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

void processaTeclado(Sistema *s){
    char tecla = lechar();
    if (tecla == ESC) {
        s->terminouOnda = true;
        s->terminouPartida = true;
        s->terminou = true;
    } else if (tecla == TAB) {
        if (s->armaCorrente < 10) {
            s->armaCorrente++;
        } else {
            s->armaCorrente = 0;
        }
    } else if (tecla == ENTER) {
        if (s->tiros > 0) {
            s->tiros--;
            verificaSeMatou();
        }
    }
}
void inicializarOnda(Sistema *s){
    s->ataquesInativos = 0;
    s->tiros = 30;
}

void jogaOnda(Sistema *s)
{
    inicializarOnda(s);
    while (!s->terminouOnda) {
        processaTeclado(s);
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

void iniciarAtaqueAtivos(Sistema *s)
{
    for (int a = 0; a < 15; a++) {
        int x = rand();
        x = x % 11;
        s->ataquesAtivos[a] = x;
    }
}

void inicializarSistema(Sistema *s)
{
    s->terminou = false;
    s->terminouOnda = false;
    s->terminouPartida = false;
    s->pontos = 0;
    s->tiros = 30;
    s->armaCorrente = 0;
    s->escudos = 3;
    s->ondaAtual = 1;
    iniciarAtaquesAtivos(s);
}

int main()
{
    configuraTerminal();
    Sistema sistema;
    //inicializa_tela();
    inicializaSistema(&sistema);
    while (!sistema.terminou) {
        jogaPartida(&sistema);
    }
    //desinicializa_tela();
    normalizaTerminal();
}