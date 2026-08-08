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
    int pontos, tiros, armaCorrente, escudos, ondaAtual, ataquesAtivos, espacos;
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
    s->terminouOnda = true;
    for (int a = 0; a < TOTAL_ATACANTES; a++){
        if(s->ataques[a] != -1) s->terminouOnda = false;
    }
}

void verificaSeMatou(Sistema *s)
{
    for (int a = 0 ; a < s->ataquesAtivos ; a++) {
        if (s->armaCorrente == s->ataques[a]) {
            s->ataques[a] = -1;
            s->pontos++;
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
            verificaSeGanhou(s);
        }
    }
}

void movimentaAtaques(Sistema *s)
{
    s->ataquesAtivos++;
    int x = rand();
    x = x % 11;
    s->ataques[s->ataquesAtivos - 1] = x;
    
    // verificar colisão com escudo, verificar colisão com base,
    // verificar se a onda terminou
}

void processaTempo(Sistema *s)
{
    if (crono_parcial(&s->c) < s->tempoMovimentação) return;
    crono_inicia(&s->c);
    if(s->ataquesAtivos < 13){
        movimentaAtaques(s);
    }
}

void inicializarOnda(Sistema *s)
{
    s->tiros = 30;
    s->tempoMovimentação =  s->tempoMovimentação / 2;
    crono_inicia(&s->c);
}

void finalizaOnda(Sistema *s)
{
    s->ondaAtual++;
}

void apresenta(Sistema *s)
{
    printf("%2d %2d", s->pontos, s->tiros);
    if(s->armaCorrente == 10){
        printf(" n");
    } else {
        printf(" %d", s->armaCorrente);
    }
    if (s->escudos == 1) {
        printf(") ");
    } else if (s->escudos == 2) {
        printf(")) ");
    } else if (s->escudos == 3) {
        printf("))) ");
    }
    for(int a = 0; a < s->espacos; a++){
        printf(" ");
    }
    printf("          ");
    for(int a = 0; a < s->ataquesAtivos; a++){
        if (s->ataques[a] == 10) {
            printf("N");
        } else if (s->ataques[a] == -1) {
            printf(" ");
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

void inicializarAtaques(Sistema *s)
{
    for (int a = 0; a < TOTAL_ATACANTES; a++){
        s->ataques[a] = -1;
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
    s->tempoMovimentação = 4;
    s->espacos = 10;
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