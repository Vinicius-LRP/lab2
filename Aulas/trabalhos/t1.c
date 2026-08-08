#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct{
  bool terminou, terminouPartida, terminouOnda;
  int pontos, tiros, arma;
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

void joga_partida(Sistema *s)
{
  while (!s->terminouPartida) {
    jogaOnda(s);
  }
}

int main()
{
  Sistema sistema;
  inicializa_tela();
  inicializa_estado(&sistema);
  while (!sistema.terminou) {
    joga_partida(&sistema);
  }
  desinicializa_tela();
}