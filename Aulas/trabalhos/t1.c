#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct{
    bool terminou, terminouPartida, terminouOnda;
    int pontos, tiros, arma, escudos;
    int inimigosInativos;
  
} Sistema;


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