
// teste_str.c
// programa com testes do TAD str

#include "str.h"
#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

int main() 
{
    Str s = s_cria_número(32);
    s_imprime(s);
    
    double num = s_número(s);

    printf("\n%f\n", num);

    s_destroi(s);
    
   Lista l = l_cria();
Str a = s_cria("oi");
Str b = s_cria("mundo");
l_insere_fim(l, a);
l_insere_fim(l, b);

l_imprime(l);   // "oi mundo "
printf("\n");

s_destroi(a);
s_destroi(b);
l_destroi(l);

}