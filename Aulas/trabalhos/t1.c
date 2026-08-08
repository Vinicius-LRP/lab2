#include <stdio.h>
#include <stdlib.h>

typedef struct {
  bool terminou;
  int pontos;
  int inimigos_inativos;
  int tiros;
  // ...
} estado_t;

void joga_onda(estado_t *est)
{
  repete enquanto nao terminar a onda {
    processa_teclado(est);
    processa_tempo(est);
    apresenta(est);
  }
}

void joga_partida(estado_t *est)
{
  while (!est->terminou_partida) {
    joga_onda(est);
  }
}

int main()
{
  estado_t estado;
  inicializa_tela();
  inicializa_estado(&estado);
  while (!estado.terminou) {
    joga_partida(&estado);
  }
  desinicializa_tela();
}