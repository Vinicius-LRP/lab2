#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef enum 
{
    ENTER = 13,
    ESC = 27,
    TAB = 9,
    ESPACO = 32
} Controle;

typedef enum 
{
    TOTAL_ATACANTES_DIURNO = 20,
    TOTAL_ATACANTES_NOTURNO = 15
} Atacantes;

typedef struct timespec crono;

typedef struct
{
    bool terminou, terminouPartida, terminouOnda;
    int pontos, tiros, armaCorrente, escudos, ondaAtual;
    int ataquesAtivos, atacantesMortos;
    int ataques[TOTAL_ATACANTES_DIURNO];
    double tempoMovimentacao;
    int diurnoOuNoturno, chanceDiurno;
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

void tocaSom(int codigo)
{
    char script[100];

    if (codigo == 10 || codigo == 11) {
        system("aplay -q 11.3.wav &");
    } else if (codigo == -1) {
        system("aplay -q x.3.wav &");
    } else if (codigo == -2) {
        system("aplay -q 12.3.wav &");
    } else {
        sprintf(script, "aplay -q %d.3.wav &", codigo);
        system(script);
    }
}


void verificaSeGanhou(Sistema *s)
{
    if(s->diurnoOuNoturno == 1 && s->atacantesMortos == TOTAL_ATACANTES_DIURNO){
        s->terminouOnda = true;
        s->ondaAtual++;
    } else if (s->diurnoOuNoturno == 2 && s->atacantesMortos == TOTAL_ATACANTES_NOTURNO){
        s->terminouOnda = true;
        s->ondaAtual++;
    }
}

bool testeN(Sistema *s, int a){
    if (s->armaCorrente + 1 == 11 && s->ataques[a] == 11) return true;
    return false;
}

void verificaSeMatou(Sistema *s)
{
    bool acertou = false;
    for (int a = s->escudos ; a < 13 ; a++) {
        if (s->armaCorrente == s->ataques[a] || testeN(s, a)) {
            if(s->armaCorrente == 10 && s->ataques[a] == 10){
                s->ataques[a] = 11;
                acertou = true;
                if (s->diurnoOuNoturno == 2) system("aplay -q acertoTiro.wav &");
            } else {
                if(s->ataques[a] == 11) {
                    if(s->diurnoOuNoturno == 1){
                        s->pontos += (13 - a) * 2;
                    } else if (s->diurnoOuNoturno == 2){
                        s->pontos += ((13 - a) * 2) * 2;
                    }
                } else {
                    if(s->diurnoOuNoturno == 1){
                        s->pontos += 13 - a;
                    } else if(s->diurnoOuNoturno == 2){
                        s->pontos += (13 - a) * 2;
                    }
                }
                s->ataques[a] = -1;
                s->atacantesMortos++;
                acertou = true;
                if (s->diurnoOuNoturno == 2) system("aplay -q acertoTiro.wav &");
            }
            break;
        }
    }
    if(!acertou) {
        if(s->diurnoOuNoturno == 2) system("aplay -q erroTiro.wav &");
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
        if (s->diurnoOuNoturno == 1) {
            if (s->armaCorrente < 10) {
                s->armaCorrente++;
            } else {
                s->armaCorrente = 0;
            }
        } else if (s->diurnoOuNoturno == 2) {
            if (s->armaCorrente < 10) {
                s->armaCorrente += 2;
                tocaSom(s->armaCorrente);
            } else {
                s->armaCorrente = 0;
                tocaSom(s->armaCorrente);
            }
        }
    } else if (tecla == ENTER) {
        if (s->tiros > 0) {
            s->tiros--;
            verificaSeMatou(s);
        }
        verificaSeGanhou(s);
    } else if (tecla == ESPACO){
        struct timespec intervalo = {0, 500000000};
        for (int a = 0 ; a < 8 ; a++){
            tocaSom(s->ataques[a]);
            nanosleep(&intervalo, NULL);                                                                                                                                            
        }
    }
}

void verificaBaseEosEscudos(Sistema *s)
{
    if(s->ataques[s->escudos] != -1 && s->escudos != 0){
        s->ataques[s->escudos] = -1;
        s->atacantesMortos++;
        s->escudos--;
    } else if (s->ataques[0] != -1 && s->escudos == 0) {
        s->terminou = true;
        s->terminouOnda = true;
        s->terminouPartida = true;
        return;
    }
    verificaSeGanhou(s);
}

void movimentaAtaques(Sistema *s)
{
    verificaBaseEosEscudos(s);
    for (int i = s->escudos; i < 13; i++) {
        s->ataques[i] = s->ataques[i + 1];
    }
    if (s->diurnoOuNoturno == 1 && s->ataquesAtivos < TOTAL_ATACANTES_DIURNO) {        
        s->ataques[12] = rand() % 11;
        s->ataquesAtivos++;
    } else if (s->diurnoOuNoturno == 2 && s->ataquesAtivos < TOTAL_ATACANTES_NOTURNO) {        
        s->ataques[7] = (rand() % 6) * 2;
        s->ataquesAtivos++;
        tocaSom(s->ataques[7]);
    } else {
        s->ataques[12] = -1;
    }
}

void processaTempo(Sistema *s)
{
    if (s->diurnoOuNoturno == 1 && crono_parcial(&s->c) < s->tempoMovimentacao) return;
    if (s->diurnoOuNoturno == 2 && crono_parcial(&s->c) < s->tempoMovimentacao * 3) return;
    crono_inicia(&s->c);
    movimentaAtaques(s);
}

void inicializarAtaques(Sistema *s)
{
    for (int a = 0; a < TOTAL_ATACANTES_DIURNO; a++){
        s->ataques[a] = -1;
    }
}

void inicializarEscudos(Sistema *s)
{
    for(int a = 0; a < s->escudos; a++){
        s->ataques[a] = -2;
    }
}

void inicializarOnda(Sistema *s)
{
    inicializarAtaques(s);
    s->tiros = 30;
    if (s->ondaAtual > 1) {
        s->tempoMovimentacao *= 0.90;
    }
    crono_inicia(&s->c);
    s->terminouOnda = false;
    s->armaCorrente = 0;
    s->ataquesAtivos = 0;
    s->atacantesMortos = 0;
    inicializarEscudos(s);
}

void apresentaResumo(Sistema *s)
{
    printf("\r\033[KPontos: %d Tiros: %d | Para recarregar pressione R", s->pontos, s->tiros);
    fflush(stdout);
}

void finalizaOnda(Sistema *s)
{
    /*int sorteio = rand() % 100;
    if(sorteio <= s->chanceDiurno){
        s->diurnoOuNoturno = 1;
    } else {
        s->diurnoOuNoturno = 2;
    }
    if(s->chanceDiurno >= 39) {
        s->chanceDiurno -= 20;
    }*/
    if(s->diurnoOuNoturno == 1){
        s->pontos += s->tiros * 2 + s->escudos * 10;
    } else if (s->diurnoOuNoturno == 2){
        s->pontos += (s->tiros * 2 + s->escudos * 10) *2;
    }
    char c = 'n';
    while(c != 'r' && s->terminou != true){
        apresentaResumo(s);
        c = lechar();
        if(c == ESC){
            s->terminou = true;
            s->terminouPartida = true;  
            s->terminouOnda = true;
            break;
        }
    }
}

void apresentaDiurno(Sistema *s)
{
    printf("\r\033[K%3d %2d ", s->pontos, s->tiros);
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
        } else if (s->ataques[a] == 11){
            printf("n");
        } else if (s->ataques[a] == -2) {
            printf(")"); 
        } else {
            printf("%d", s->ataques[a]);
        }
    }
    printf("\r");
}

void apresentaNoturno(Sistema *s)
{
    printf("\r\033[K%3d %2d ", s->pontos, s->tiros);
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
        } else if (s->ataques[a] == 11){
            printf("n");
        } else if (s->ataques[a] == -2) {
            printf(")"); 
        } else {
            printf("%d", s->ataques[a]);
        }
    }
    printf("\r");
}

void apresentaOnda(Sistema *s)
{
    printf("\r\033[KOnda numero %d Noturna ou diurna: %d", s->ondaAtual, s->diurnoOuNoturno);
    fflush(stdout);
}

void jogaOnda(Sistema *s)
{
    inicializarOnda(s);
    apresentaOnda(s);
    crono_inicia(&s->c);
    while (crono_parcial(&s->c) < 2.0) {
    }
    crono_inicia(&s->c);
    while (!s->terminouOnda) {
        processaTeclado(s);
        processaTempo(s);
        if(s->diurnoOuNoturno == 1){
            apresentaDiurno(s); 
        } else if (s->diurnoOuNoturno == 2){
            apresentaNoturno(s);
        }
    }
    finalizaOnda(s);
}
void apresentaFinalizaPartida(Sistema *s){
    printf("\r\033[K Pontuação: %d | Digite S para jogar novamente ou N para sair", s->pontos);
}

void finalizaPartida(Sistema *s){
    char c = 'x';
    while ((c != 's' && c != 'S') && (c != 'n' && c != 'N')) {
        apresentaFinalizaPartida(s);
        c = lechar();
        if(c == 's' || c == 'S') {
            s->terminou = false;
            s->terminouPartida = false;
            s->terminouOnda = false;
        }
    }
}

void jogaPartida(Sistema *s)
{
    while (!s->terminouPartida) {
        jogaOnda(s);
    }
    finalizaPartida(s);
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
    s->tempoMovimentacao = 2;
    s->atacantesMortos = 0;
    s->diurnoOuNoturno = 2;
    s->chanceDiurno = 80;
    inicializarAtaques(s);
}

int main()
{
    srand(time(NULL));
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