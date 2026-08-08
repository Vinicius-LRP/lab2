#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct{
    bool terminou, terminouPartida, terminouOnda;
    int pontos, tiros, arma, escudos;
    int inimigosInativos;
  
} Sistema;

void jogaOnda(Sistema *s)
{
    while (!s->terminouOnda) {
        processaTeclado(s);
        processaTempo(s);
        apresenta(s);
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
    setvbuf(stdin, NULL, _IONBF, 0);
    system("stty raw -echo min 0 time 1 opost");
    Sistema sistema;
    inicializa_tela();
    inicializaSistema(&sistema);
    while (!sistema.terminou) {
        jogaPartida(&sistema);
    }
    desinicializa_tela();
    system("stty sane");
}