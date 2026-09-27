
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
    l_insere_fim(l, s_cria("a"));
    l_insere_fim(l, s_cria("b"));
    l_insere_fim(l, s_cria("c"));

}