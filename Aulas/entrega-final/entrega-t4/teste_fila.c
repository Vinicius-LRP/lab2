#include "fila.h"

#include <stdio.h>
#include <string.h>


void teste()
  {
      Fila f1 = f_cria(sizeof(int));
      Fila f2 = f_cria(10);
      char s[10];
      for (int i = 1; i <= 4; i++) {
          f_insere(f1, &i);
      }
      strcpy(s, "teste");
      f_insere(f2, s);
      strcpy(s, "fim\n");
      f_insere(f2, s);
      f_remove(f2, s);
      printf("%s ", s);
      while (!f_tá_vazia(f1)) {
          int a;
          f_remove(f1, &a);
          printf("%d ", a);
      }
      f_remove(f2, s);
      printf("%s", s);
      f_destrói(f2);
      f_destrói(f1);
  }

int main() {
    teste();
}