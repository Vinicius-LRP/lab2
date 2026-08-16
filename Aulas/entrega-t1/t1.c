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
    A_DIU = 20,
    A_NOT = 15
} Atacantes;

typedef struct timespec crono;

typedef struct
{
    bool terminou, terminouPartida, terminouOnda;
    int pontos, tiros, armaCorrente, escudos, ondaAtual;
    int ataquesAtivos, atacantesMortos;
    int ataques[A_DIU];
    double tempoMovimentacao;
    int diurnoOuNoturno, chanceDiurno;
    int ranking[3];
    int sorteio;
    bool estaNoRanking;
    crono c;
} Sistema;

// inicializa um cronômetro com a hora atual
void crono_inicia(crono *c)
{
    clock_gettime(CLOCK_MONOTONIC, c);
}

// retorna o tempo passado desde que o cronômetro *c foi iniciado, em segundos
double crono_parcial(crono *c)
{
    crono agora;
    clock_gettime(CLOCK_MONOTONIC, &agora);
    double segundos = agora.tv_sec - c->tv_sec;
    double nanosegundos = agora.tv_nsec - c->tv_nsec;
    return segundos + 1e-9 * nanosegundos;
}


// configura o terminal para o modo "cru", para permitir a leitura
//   de cada caractere digitado sem esperar pelo "enter".
void configuraTerminal()
{
    if (system("stty raw opost -echo min 0 time 1") != 0) {
        perror("erro na execução de system(\"stty\")");
        fprintf(stderr, "você tem o programa stty instalado?\n");
        exit(1);
    }
    if (setvbuf(stdin, NULL, _IONBF, 0) != 0) {
        perror("erro na execução de setvbuf()");
        exit(1);
    }
}

// normaliza terminal
void normalizaTerminal()
{
    system("stty sane");
}

// lê um caractere do teclado.
// retorna o código do caractere lido ou 0 casa nada tenha sido digitado.
// só funciona corretamente se o terminal estiver em modo "cru".
char lechar()
{
    fflush(stdout);
    char c;
    if (fread(&c, 1, 1, stdin) == 1) return c;
    return 0;
}

void tocaSom(int codigo) {
    char script[100];
    if (codigo == 10 || codigo == 11) {
        system("aplay -q 11.3.wav &");
    } else if (codigo == -1) {
        system("aplay -q x.3.wav &");
    } else if (codigo == -2) {
        system("aplay -q 12.3.wav &");
    } else if (codigo == -3) {
        system("aplay -q ondaConcluida.wav &");
    } else if (codigo == -4) {
        system("aplay -q escudoQuebrado.wav &");
    } else if (codigo == -5) {
        system("aplay -q finalizacao.wav &");
    } else if (codigo == -6) {
        system("aplay -q explosao.wav &");
    } else if (codigo == -7) {
        system("aplay -q inicioOndaNoturna.wav &");
    } else if (codigo == -8) { 
        system("aplay -q inicioOndaDiurna.wav &");
    } else {
        sprintf(script, "aplay -q %d.3.wav &", codigo);
        system(script);
    }
}


void verificaSeGanhou(Sistema *s)
{
    if (s->diurnoOuNoturno == 1 && s->atacantesMortos == A_DIU) {
        s->terminouOnda = true;
        s->ondaAtual++;
    } else if (s->diurnoOuNoturno == 2 && s->atacantesMortos == A_NOT) {
        s->terminouOnda = true;
        s->ondaAtual++;
    }
}

bool testeN(Sistema *s, int a)
{
    if (s->armaCorrente + 1 == 11 && s->ataques[a] == 11) return true;
    return false;
}

void calculaPontosAcerto(Sistema *s, int a)
{
    if (s->diurnoOuNoturno == 1) {
        s->pontos += 13 - a;
    } else if (s->diurnoOuNoturno == 2) {
        s->pontos += (8 - a) * 2;
    }
}

void calculaPontosAcertoN(Sistema *s, int a)
{
    if(s->diurnoOuNoturno == 1) {
        s->pontos += (13 - a) * 2;
    } else if (s->diurnoOuNoturno == 2) {
        s->pontos += ((8 - a) * 2) * 2;
    }
}

void verificaAcertou(bool a)
{
    if (!a) system("aplay -q erroTiro.wav &");
}

void tocaAcertoTiro(int a)
{
    if (a == 1) {
        system("aplay -q acertoTiroN.wav &");
    } else if (a == 0) {
        system("aplay -q acertoTiro.wav &");
    }
}

void verificaSeMatou(Sistema *s)
{
    bool acertou = false;
    for (int a = s->escudos ; a < 13 ; a++) {
        if (s->armaCorrente == s->ataques[a] || testeN(s, a)) {
            if (s->armaCorrente == 10 && s->ataques[a] == 10) {
                s->ataques[a] = 11;
                acertou = true;
                tocaAcertoTiro(1);
            } else {
                if (s->ataques[a] == 11) {
                    calculaPontosAcertoN(s, a);
                } else {
                    calculaPontosAcerto(s, a);
                }
                s->ataques[a] = -1;
                s->atacantesMortos++;
                acertou = true;
                tocaAcertoTiro(0);
            }
            break;
        }
    }
    verificaAcertou(acertou); 
}

void processaEsc(Sistema *s){
    s->terminouOnda = true;
    s->terminouPartida = true;
    s->terminou = true;
}

void processaTab(Sistema *s){
    if (s->armaCorrente < 10) {
        if (s->diurnoOuNoturno == 1) {
            s->armaCorrente++;
        } else if (s->diurnoOuNoturno == 2) {
            s->armaCorrente += 2;
        }
        tocaSom(s->armaCorrente);
    } else {
        s->armaCorrente = 0;
        tocaSom(s->armaCorrente);
    }
}

void processaEnter(Sistema *s){
    if (s->tiros > 0) {
        s->tiros--;
        verificaSeMatou(s);
    }
    verificaSeGanhou(s);
}

void processaEspaco(Sistema *s){
    struct timespec intervalo = {0, 500000000};
    if (s->diurnoOuNoturno == 2) {
        for (int a = 0 ; a < 8 ; a++) {
            tocaSom(s->ataques[a]);
            nanosleep(&intervalo, NULL);
        }
    } else if (s->diurnoOuNoturno == 1) {
        for (int a = 0 ; a < 13 ; a++) {
            tocaSom(s->ataques[a]);
            nanosleep(&intervalo, NULL);
        }
    }
}

void processaTeclado(Sistema *s)
{
    char tecla = lechar();
    if (tecla == ESC) {
        processaEsc(s);
    } else if (tecla == TAB) {
        processaTab(s);
    } else if (tecla == ENTER) {
        processaEnter(s);
    } else if (tecla == ESPACO) {
        processaEspaco(s);
    }
}

void verificaBaseEosEscudos(Sistema *s)
{
    if (s->ataques[s->escudos] != -1 && s->escudos != 0) {
        s->ataques[s->escudos] = -1;
        s->atacantesMortos++;
        s->escudos--;
        tocaSom(-4);
    } else if (s->ataques[0] != -1 && s->escudos == 0) {
        s->terminou = true;
        s->terminouOnda = true;
        s->terminouPartida = true;
        tocaSom(-6);
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
    if (s->diurnoOuNoturno == 1 && s->ataquesAtivos < A_DIU) {        
        s->ataques[12] = rand() % 11;
        if (s->terminou != true) tocaSom(s->ataques[12]);
        s->ataquesAtivos++;
    } else if (s->diurnoOuNoturno == 2 && s->ataquesAtivos < A_NOT) {
        s->ataques[7] = (rand() % 6) * 2;
        s->ataquesAtivos++;
        if (s->terminou != true) tocaSom(s->ataques[7]);
    } else {
        s->ataques[12] = -1;
    }
}

void processaTempo(Sistema *s)
{
    double cp = crono_parcial(&s->c);
    if (s->diurnoOuNoturno == 1 && cp < s->tempoMovimentacao) return;
    if (s->diurnoOuNoturno == 2 && cp < s->tempoMovimentacao * 3) return;
    crono_inicia(&s->c);
    movimentaAtaques(s);
}

void inicializarAtaques(Sistema *s)
{
    for (int a = 0; a < A_DIU; a++) {
        s->ataques[a] = -1;
    }
}

void inicializarEscudos(Sistema *s)
{
    for (int a = 0; a < s->escudos; a++) {
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
    printf("\r\033[KPontos: %d | R recarregar | ESC sair", s->pontos);
    fflush(stdout);
}

void sorteio(Sistema *s){
    s->sorteio = rand() % 100;
    if (s->sorteio <= s->chanceDiurno) {
        s->diurnoOuNoturno = 1;
    } else {
        s->diurnoOuNoturno = 2;
    }
    if (s->chanceDiurno >= 39) {
        s->chanceDiurno -= 20;
    }
}

void finalizaOnda(Sistema *s)
{
    sorteio(s);
    if (s->diurnoOuNoturno == 1) {
        if (s->terminou != true) {
            s->pontos += s->tiros * 2 + s->escudos * 10;
        }
    } else if (s->diurnoOuNoturno == 2) {
        if (s->terminou != true) {
            s->pontos += (s->tiros * 2 + s->escudos * 10) *2;
        }
    }
    char c = 'n';
    if (s->terminou != true) tocaSom(-3);
    while (c != 'r' && c != 'R' && s->terminou != true) {
        apresentaResumo(s);
        c = lechar();
        if (c == ESC) {
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
    if (s->armaCorrente == 10) {
        printf(" n");
    } else {
        printf(" %d", s->armaCorrente);
    }
    for (int a = 0; a < 13; a++) {
        if (s->ataques[a] == -1) {
            printf(" ");
        } else if (s->ataques[a] == 10) {
            printf("N");
        } else if (s->ataques[a] == 11) {
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
    if (s->armaCorrente == 10) {
        printf(" n");
    } else {
        printf(" %d", s->armaCorrente);
    }
    for (int a = 0; a < 13; a++) {
        if (s->ataques[a] == -1) {
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
    if (s->diurnoOuNoturno == 1) {
        printf("\r\033[KOnda numero %d | Diurna", s->ondaAtual);
    } else if (s->diurnoOuNoturno == 2) {
        printf("\r\033[KOnda numero %d | Noturna", s->ondaAtual);
    }
    fflush(stdout);
}

void jogaOnda(Sistema *s)
{
    inicializarOnda(s);
    apresentaOnda(s);
    if (s->diurnoOuNoturno == 1) {
        tocaSom(-8);
    } else if (s->diurnoOuNoturno == 2) {
        tocaSom(-7);
    }
    crono_inicia(&s->c);
    while (crono_parcial(&s->c) < 3) {
    }
    crono_inicia(&s->c);
    while (!s->terminouOnda) {
        processaTeclado(s);
        processaTempo(s);
        if (s->diurnoOuNoturno == 1) {
            apresentaDiurno(s); 
        } else if (s->diurnoOuNoturno == 2) {
            apresentaNoturno(s);
        }
    }
    finalizaOnda(s);
}

void leRanking(Sistema *s)
{
    FILE *arquivo = fopen("ranking.txt", "r");
    if (arquivo != NULL) {
        for (int a = 0; a < 3 ; a++) {
            fscanf(arquivo,"%d", &s->ranking[a]);
        }
        fclose(arquivo);
    } else {
        arquivo = fopen("ranking.txt", "w");
        if (arquivo != NULL) {
            for (int a = 0; a < 3; a++) {
                s->ranking[a] = 0;
            }
            fclose(arquivo);
        }
    }
}

void analisaRanking(Sistema *s)
{
    int a;
    for (a = 0 ; a < 3 ; a++) {
        if (s->pontos < s->ranking[a]) {
            a--;
            break;
        }
    }
    if (a == 0){
        s->ranking[0] = s->pontos;
        s->estaNoRanking = true;
    } else if (a == 1){
        s->ranking[0] = s->ranking[1]; 
        s->ranking[1] = s->pontos;
        s->estaNoRanking = true;
    } else if (a >= 2){
        s->ranking[0] = s->ranking[1];
        s->ranking[1] = s->ranking[2];
        s->ranking[2] = s->pontos;
        s->estaNoRanking = true;
    }
}

void escreveRanking(Sistema *s)
{
    FILE *arquivo = fopen("ranking.txt", "w");
    if (arquivo != NULL) {
        for (int a = 0; a < 3 ; a++) {
            fprintf(arquivo, "%d ", s->ranking[a]);
        }
        fprintf(arquivo, "\n");
        fclose(arquivo);
    } else {
        printf("Erro ao abrir arquivo para escrever Ranking!");
    }
}

void apresentaFinalizaPartida(Sistema *s)
{
    if (s->estaNoRanking == true) {
        printf("\r\033[K");
        printf("Seu resultado: %d | ", s->pontos);
        printf("TOP 3! | S jogar novamente | ESC fechar");
    } else {
        printf("\r\033[K");
        printf("Seu resultado: %d | ", s->pontos);
        printf("S jogar novamente | ESC fechar");
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
    s->tempoMovimentacao = 2;
    s->atacantesMortos = 0;
    s->diurnoOuNoturno = 1;
    s->chanceDiurno = 80;
    s->estaNoRanking = false;
    inicializarAtaques(s);
}

void finalizaPartida(Sistema *s)
{
    leRanking(s);
    analisaRanking(s);
    escreveRanking(s);
    char c = 'x';
    tocaSom(-5);
    while ((c != 's' && c != 'S') && (c != ESC)) {
        apresentaFinalizaPartida(s);
        c = lechar();
        if (c == 's' || c == 'S') {
            inicializarSistema(s);
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

int main()
{
    srand(time(NULL));
    configuraTerminal();
    Sistema sistema;
    inicializarSistema(&sistema);
    while (!sistema.terminou) {
        jogaPartida(&sistema);
    }
    printf("\r\033[K");
    normalizaTerminal();
}