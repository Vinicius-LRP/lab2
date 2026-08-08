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

typedef enum {
    TOTAL_ATACANTES = 15
} Atacantes;

typedef struct timespec crono;

typedef struct
{
    bool terminou, terminouPartida, terminouOnda;
    int pontos, tiros, armaCorrente, escudos, ondaAtual, ataquesAtivos, atacantesMortos;
    int ataques[TOTAL_ATACANTES];
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

void verificaSeGanhou(Sistema *s)
{
    if(s->atacantesMortos == 15){
        s->terminouOnda = true;
    } 
}

void verificaSeMatou(Sistema *s)
{
    for (int a = s->escudos ; a < 13 ; a++) {
        if (s->armaCorrente == s->ataques[a]) {
            s->ataques[a] = -1;
            if(s->armaCorrente == 10){
                s->pontos += (13 - a) * 2;
            } else {
                s->pontos += 13 - a;
            }
            s->atacantesMortos++;
            break;
        }
    }
}

void processaTeclado(Sistema *s)
{
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
            verificaSeMatou(s);
        }
        verificaSeGanhou(s);
    }
}

void verificaBaseEosEscudos(Sistema *s){
    verificaSeGanhou(s);
    if(s->ataques[s->escudos] != -1 && s->escudos != 0){
        s->ataques[s->escudos] = -1;
        s->atacantesMortos++;
        s->escudos--;
    } else if (s->ataques[0] != -1 && s->escudos == 0) {
        s->terminou = true;
        s->terminouOnda = true;
        s->terminouPartida = true;
    }
}

void movimentaAtaques(Sistema *s)
{
    verificaBaseEosEscudos(s);
    for (int i = s->escudos; i < 13; i++) {
        s->ataques[i] = s->ataques[i + 1];
    }
    if (s->ataquesAtivos < TOTAL_ATACANTES) {        
        s->ataques[13] = rand() % 11;        
        s->ataquesAtivos++;    
    } else {
        s->ataques[13] = -1;
    }
}

void processaTempo(Sistema *s)
{
    if (crono_parcial(&s->c) < s->tempoMovimentação) return;
    crono_inicia(&s->c);
    if(s->ataquesAtivos < 25){
        movimentaAtaques(s);
    }
}

void inicializarAtaques(Sistema *s)
{
    for (int a = 0; a < TOTAL_ATACANTES; a++){
        s->ataques[a] = -1;
    }
}

void inicializarEscudos(Sistema *s){
    for(int a = 0; a < s->escudos; a++){
        s->ataques[a] = -2;
    }
}

void inicializarOnda(Sistema *s)
{
    inicializarAtaques(s);
    s->tiros = 30;
    s->tempoMovimentação =  s->tempoMovimentação - (s->tempoMovimentação * (10 / 100)) ;
    crono_inicia(&s->c);
    s->terminouOnda = false;
    s->armaCorrente = 0;
    s->ataquesAtivos = 0;
    s->atacantesMortos = 0;
    inicializarEscudos(s);
}

void finalizaOnda(Sistema *s)
{
    s->ondaAtual++;
}

void apresenta(Sistema *s)
{
    printf("%2d %2d ", s->pontos, s->tiros);
    if(s->armaCorrente == 10){
        printf(" n");
    } else {
        printf(" %d", s->armaCorrente);
    }
    for(int a = 0; a < 13; a++){
        if(s->ataques[a] == -1){
            printf(" ");
        } else if (s->ataques[a] == 10) {
            printf("N");
        } else if (s->ataques[a] == -2) {
            printf(")"); 
        } else {
            printf("%d", s->ataques[a]);
        }
    }
    
    printf("\r");
}

void jogaOnda(Sistema *s)
{
    inicializarOnda(s);
    while (!s->terminouOnda) {
        processaTeclado(s);
        processaTempo(s);
        apresenta(s);
    }
    finalizaOnda(s);
}

void jogaPartida(Sistema *s)
{
    while (!s->terminouPartida) {
        jogaOnda(s);
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
    s->ataquesAtivos = 0;
    s->tempoMovimentação = 2;
    s->atacantesMortos = 0;
    inicializarAtaques(s);
}

int main()
{
    configuraTerminal();
    Sistema sistema;
    //inicializa_tela();
    inicializarSistema(&sistema);
    while (!sistema.terminou) {
        jogaPartida(&sistema);
    }
    //desinicializa_tela();
    normalizaTerminal();
}